#pragma once
#include "DrainEvents.h"
#include "TestSupport.h"
#include <chrono>
#include <thread>
inline void settleNative(FX::FXApp& app, int milliseconds = 180) {
  auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
  do {
    drainEvents(app);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  } while (std::chrono::steady_clock::now() < until);
}
inline void nativeClick(FX::FXWindow* window, int x, int y, unsigned state = 0,
                        unsigned button = Button1, bool drain = true) {
  CHECK(window->id() && window->shown() && window->getWidth() > 1 && window->getHeight() > 1);
  // Sending an X event to an invisible/off-parent widget is not a user click.
  for (auto* child = window;
       child->getParent() && child->getParent() != window->getApp()->getRootWindow();
       child = child->getParent()) {
    auto* parent = child->getParent();
    CHECK(parent->shown());
    CHECK(child->getX() >= 0 && child->getY() >= 0);
    CHECK(child->getX() + child->getWidth() <= parent->getWidth());
    CHECK(child->getY() + child->getHeight() <= parent->getHeight());
  }
  static Time time = 1000;
  time += 1000; // Independent clicks, not double-clicks.
  auto* display = static_cast<Display*>(window->getApp()->getDisplay());
  XEvent event{};
  event.xbutton.display = display;
  event.xbutton.window = window->id();
  event.xbutton.root = DefaultRootWindow(display);
  event.xbutton.button = button;
  event.xbutton.same_screen = True;
  event.xbutton.x = x;
  event.xbutton.y = y;
  Window child;
  XTranslateCoordinates(display, window->id(), DefaultRootWindow(display), x, y,
                        &event.xbutton.x_root, &event.xbutton.y_root, &child);
  event.xbutton.time = time;
  event.xbutton.state = state;
  event.type = ButtonPress;
  CHECK(XSendEvent(display, window->id(), False, ButtonPressMask, &event));
  event.type = ButtonRelease;
  event.xbutton.state |= button == Button1 ? Button1Mask : Button3Mask;
  CHECK(XSendEvent(display, window->id(), False, ButtonReleaseMask, &event));
  XFlush(display);
  if (drain)
    settleNative(*window->getApp());
}
