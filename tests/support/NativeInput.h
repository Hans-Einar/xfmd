#pragma once
#include "DrainEvents.h"
#include "TestSupport.h"
#include <X11/Xatom.h>
#include <chrono>
#include <thread>
inline void settleNative(FX::FXApp& app, int milliseconds = 180) {
  auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
  do {
    drainEvents(app);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  } while (std::chrono::steady_clock::now() < until);
}
inline Time nativeServerTime(Display* display) {
  auto window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
  XSelectInput(display, window, PropertyChangeMask);
  auto atom = XInternAtom(display, "XFMD_TEST_CLOCK", False);
  unsigned long tick = 0;
  XChangeProperty(display, window, atom, XA_INTEGER, 32, PropModeReplace,
                  reinterpret_cast<unsigned char*>(&tick), 1);
  XEvent event{};
  XWindowEvent(display, window, PropertyChangeMask, &event);
  XDestroyWindow(display, window);
  return event.xproperty.time;
}
inline void nativeClick(FX::FXWindow* window, int x, int y, unsigned state = 0,
                        unsigned button = Button1, bool drain = true) {
  CHECK(window->id() && window->shown() && window->getWidth() > 1 && window->getHeight() > 1);
  // The hit point must survive every ancestor's clipping. FOX tabs deliberately
  // extend their bottom border beyond the tab bar; that does not hide the label.
  int visibleX = x, visibleY = y;
  for (auto* child = window; child != window->getApp()->getRootWindow();
       child = child->getParent()) {
    CHECK(child && child->shown());
    CHECK(visibleX >= 0 && visibleY >= 0);
    CHECK(visibleX < child->getWidth() && visibleY < child->getHeight());
    visibleX += child->getX();
    visibleY += child->getY();
  }
  auto* display = static_cast<Display*>(window->getApp()->getDisplay());
  static Time lastTime = 0;
  static Window lastWindow = 0;
  static unsigned lastButton = 0;
  Time time = nativeServerTime(display);
  if (lastWindow == window->id() && lastButton == button &&
      time - lastTime <= window->getApp()->getClickSpeed()) {
    settleNative(*window->getApp(), window->getApp()->getClickSpeed() - (time - lastTime) + 5);
    time = nativeServerTime(display);
  }
  lastTime = time;
  lastWindow = window->id();
  lastButton = button;
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
  XWarpPointer(display, None, window->id(), 0, 0, 0, 0, x, y);
  XSync(display, False);
  settleNative(*window->getApp(), 10);
  event.xbutton.time = time;
  event.xbutton.state = state;
  event.type = ButtonPress;
  CHECK(XSendEvent(display, window->id(), False, ButtonPressMask, &event));
  event.type = ButtonRelease;
  CHECK(button >= Button1 && button <= Button5);
  event.xbutton.state |= Button1Mask << (button - Button1);
  CHECK(XSendEvent(display, window->id(), False, ButtonReleaseMask, &event));
  XFlush(display);
  if (drain)
    settleNative(*window->getApp());
}
inline void nativeKey(FX::FXWindow* window, KeySym key, unsigned state = 0) {
  auto* display = static_cast<Display*>(window->getApp()->getDisplay());
  XEvent event{};
  event.xkey.display = display;
  event.xkey.window = window->id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.same_screen = True;
  event.xkey.keycode = XKeysymToKeycode(display, key);
  event.xkey.state = state;
  event.type = KeyPress;
  CHECK(XSendEvent(display, window->id(), False, KeyPressMask, &event));
  event.type = KeyRelease;
  CHECK(XSendEvent(display, window->id(), False, KeyReleaseMask, &event));
  XFlush(display);
  settleNative(*window->getApp());
}
