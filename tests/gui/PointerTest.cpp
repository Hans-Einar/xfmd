#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <thread>
using namespace xfmd;
void pump(Application& app) {
  for (int i = 0; i < 15; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}
void button(Application& app, unsigned code, bool press, unsigned state = 0, int x = 30,
            int y = 30) {
  auto* d = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  event.xbutton.display = d;
  event.xbutton.window = app.host->id();
  event.xbutton.root = DefaultRootWindow(d);
  event.xbutton.button = code;
  event.xbutton.state = state;
  event.xbutton.same_screen = True;
  event.xbutton.x = x;
  event.xbutton.y = y;
  event.type = press ? ButtonPress : ButtonRelease;
  CHECK(XSendEvent(d, app.host->id(), False, press ? ButtonPressMask : ButtonReleaseMask, &event));
  XFlush(d);
  pump(app);
}
void run() {
  int argc = 1;
  char name[] = "xfmd-pointer-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  pump(app);
  button(app, Button1, true);
  button(app, Button1, false, Button1Mask);
  CHECK(!app.host->grabbed()); // Regression: overridden release bypassed FXWindow::ungrab.
  for (unsigned b : {Button1, Button2, Button3}) {
    for (int count = 1; count <= 3; ++count) {
      for (int i = 0; i < count; ++i) {
        button(app, b, true);
        button(app, b, false);
        CHECK(!app.host->grabbed());
      }
    }
  }
  // Chords may arrive when touchpad fingers change during a tap/drag.
  for (unsigned first : {Button1, Button2, Button3})
    for (unsigned second : {Button1, Button2, Button3}) {
      if (first == second)
        continue;
      button(app, first, true);
      button(app, second, true);
      button(app, first, false);
      button(app, second, false, 0, -10, -10);
      CHECK(!app.host->grabbed());
    }
  // Pending layout must not bypass release cleanup.
  button(app, Button1, true);
  app.host->invalidate();
  button(app, Button1, false);
  CHECK(!app.host->grabbed());
  CHECK(app.session.snapshot().text.empty());
}
TEST_MAIN(run)
