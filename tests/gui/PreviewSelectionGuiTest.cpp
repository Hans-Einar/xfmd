#include "application/Application.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <thread>
using namespace xfmd;
void pump(Application& app) {
  for (int i = 0; i < 20; ++i) {
    drainEvents(app.app);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}
void pointer(Application& app, int type, Point point) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  event.type = type;
  event.xbutton.display = display;
  event.xbutton.window = app.host->id();
  event.xbutton.root = DefaultRootWindow(display);
  event.xbutton.same_screen = True;
  event.xbutton.x = point.x;
  event.xbutton.y = point.y;
  long mask;
  if (type == MotionNotify) {
    event.xmotion.state = Button1Mask;
    mask = PointerMotionMask;
  } else {
    event.xbutton.button = Button1;
    event.xbutton.state = type == ButtonRelease ? Button1Mask : 0;
    mask = type == ButtonPress ? ButtonPressMask : ButtonReleaseMask;
  }
  CHECK(XSendEvent(display, app.host->id(), False, mask, &event));
  XFlush(display);
  pump(app);
}
void key(Application& app, KeySym symbol) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  XSetInputFocus(display, app.window->id(), RevertToParent, CurrentTime);
  event.type = KeyPress;
  event.xkey.display = display;
  event.xkey.window = app.host->id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.same_screen = True;
  event.xkey.state = ControlMask;
  event.xkey.keycode = XKeysymToKeycode(display, symbol);
  CHECK(XSendEvent(display, app.host->id(), False, KeyPressMask, &event));
  XFlush(display);
  pump(app);
}
void run() {
  int argc = 1;
  char name[] = "xfmd-selection-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.edits.applyEdit({0, 0, "# Tittel\n\n[Blåbær](next.md) og **tekst**.\n\nNeste avsnitt."});
  for (int i = 0; i < 30; ++i)
    pump(app);
  CHECK(app.host->interactive());
  auto frame = app.host->frame();
  const DrawRun* link = nullptr;
  for (auto& run : frame->runs)
    if (run.text == "Blåbær")
      link = &run;
  CHECK(link);
  int activations = 0;
  app.host->linkActivated = [&](const std::string&) { ++activations; };
  auto begin = app.host->documentToView({link->bounds.x, link->bounds.y + 3});
  auto end = app.host->documentToView({link->bounds.x + link->bounds.width, link->bounds.y + 3});
  pointer(app, ButtonPress, begin);
  pointer(app, MotionNotify, end);
  pointer(app, ButtonRelease, end);
  CHECK(activations == 0);
  CHECK(app.host->selectedText() == "Blåbær");
  CHECK(!app.host->grabbed());
  FX::FXString clipboard;
  auto utf8 = app.app.registerDragType("UTF8_STRING");
  CHECK(app.window->editor->getDNDData(FX::FROM_SELECTION, utf8, clipboard));
  CHECK(clipboard == "Blåbær");
  key(app, XK_c);
  CHECK(app.window->editor->getDNDData(FX::FROM_CLIPBOARD, utf8, clipboard));
  CHECK(clipboard == "Blåbær");
  CHECK(app.window->editor->getDNDData(FX::FROM_CLIPBOARD, FX::FXWindow::stringType, clipboard));
  CHECK(clipboard == "Bl\345b\346r");
  key(app, XK_a);
  CHECK(app.host->selectedText() == "Tittel\nBlåbær og tekst.\nNeste avsnitt.");
  key(app, XK_c);
  auto copied = app.host->selectedText();
  app.host->setReadingColors(app.host->readingColors());
  CHECK(app.host->selectedText() == copied);
  app.edits.applyEdit({0, 0, "Changed\n\n"});
  for (int i = 0; i < 30; ++i)
    pump(app);
  CHECK(app.host->selectedText().empty());
  CHECK(app.window->editor->getDNDData(FX::FROM_CLIPBOARD, utf8, clipboard));
  CHECK(std::string(clipboard.text(), clipboard.length()) == copied);
}
TEST_MAIN(run)
