#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <fxkeys.h>
#include <thread>
using namespace xfmd;
void events(Application& app, int ms = 300) {
  auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
  do {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  } while (std::chrono::steady_clock::now() < end);
}
void key(Application& app, FX::FXWindow* window, KeySym sym) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  event.xkey.type = KeyPress;
  event.xkey.display = display;
  event.xkey.window = window->id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.keycode = XKeysymToKeycode(display, sym);
  event.xkey.same_screen = True;
  XSendEvent(display, window->id(), False, KeyPressMask, &event);
  XFlush(display);
}
void test() {
  int argc = 1;
  char name[] = "fullscreen-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  events(app, 700);
  app.window->position(180, 140, 800, 600);
  app.views->setMode(ViewMode::Split);
  app.edits.applyEdit({0, 0, "# Fullscreen\n\nUnsaved text\n"});
  events(app, 400);
  int x = app.window->getX(), y = app.window->getY(), w = app.window->getWidth(),
      h = app.window->getHeight();
  key(app, app.window, XK_F11);
  events(app);
  CHECK(app.windowMode->fullscreen());
  CHECK(app.views->mode() == ViewMode::Split && app.session.dirty() && app.edits.canUndo());
  app.execute(CommandRouter::A4);
  events(app);
  CHECK(app.windowMode->fullscreen() && app.host->frame()->key.profile.mode == LayoutMode::Paged);
  FX::FXDialogBox dialog(app.window, "Modal first", FX::DECOR_ALL, 0, 0, 300, 150);
  new FX::FXLabel(&dialog, "Escape closes this dialog only.");
  app.scheduler->restart(90, 150, [&] {
    CHECK(app.app.getModalWindow() == &dialog);
    app.execute(CommandRouter::LeaveFullScreen);
    CHECK(app.windowMode->fullscreen());
    key(app, &dialog, XK_Escape);
  });
  dialog.execute(FX::PLACEMENT_OWNER);
  CHECK(app.windowMode->fullscreen());
  key(app, app.window, XK_Escape);
  events(app, 500);
  CHECK(!app.windowMode->fullscreen());
  CHECK(std::abs(app.window->getX() - x) < 3 && std::abs(app.window->getY() - y) < 3);
  CHECK(app.window->getWidth() == w && app.window->getHeight() == h);
  app.window->maximize();
  events(app);
  CHECK(app.window->isMaximized());
  app.execute(CommandRouter::FullScreen);
  events(app);
  CHECK(app.windowMode->fullscreen());
  app.execute(CommandRouter::FullScreen);
  events(app, 500);
  CHECK(!app.windowMode->fullscreen() && app.window->isMaximized());
  // Monitor removal and negative monitor coordinates, independent of physical outputs.
  auto moved = FoxWindowMode::ensureVisible({2100, 100, 800, 600}, {{0, 0, 1920, 1080}});
  CHECK(moved.x == 1120 && moved.y == 100 && moved.width == 800);
  auto left = FoxWindowMode::ensureVisible({-1600, 100, 800, 600},
                                           {{-1920, 0, 1920, 1080}, {0, 0, 1920, 1080}});
  CHECK(left.x == -1600);
  auto small = FoxWindowMode::ensureVisible({4000, 2000, 1100, 900}, {{0, 0, 800, 600}});
  CHECK(small.x == 0 && small.y == 0 && small.width == 800 && small.height == 600);
}
TEST_MAIN(test)
