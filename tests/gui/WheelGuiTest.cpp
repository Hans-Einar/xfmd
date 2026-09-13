#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void events(Application& app, int count = 60) {
  for (int i = 0; i < count; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void run() {
  char pattern[] = "/tmp/xfmd-wheel-XXXXXX";
  std::filesystem::path dir = mkdtemp(pattern);
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() { std::filesystem::remove_all(path); }
  } cleanup{dir};
  auto file = dir / "content.md";
  {
    std::ofstream out(file);
    for (int i = 0; i < 200; ++i)
      out << "Paragraph " << i << "\n\n";
  }
  std::ofstream(dir / (std::string(200, 'x') + ".md"));
  for (int i = 0; i < 100; ++i)
    std::ofstream(dir / ("item" + std::to_string(i) + ".md"));
  int argc = 1;
  char name[] = "wheel-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  CHECK(app.open(file.string()));
  app.views->setMode(ViewMode::Split);
  CHECK(app.window->workspacePanel->setWorkPath(dir.string()));
  events(app);
  for (int i = 0; i < 100 && app.window->sidebar->scanning; ++i)
    events(app, 2);
  CHECK(!app.window->sidebar->scanning);
  CHECK(app.window->sidebar->getPathnameItem(file.c_str()));
  events(app);
  app.scrolling.setSplit(false); // Check each physical viewport independently.
  for (auto* area : {static_cast<FX::FXScrollArea*>(app.window->sidebar),
                     static_cast<FX::FXScrollArea*>(app.window->editor),
                     static_cast<FX::FXScrollArea*>(app.host)}) {
    auto* bar = area->verticalScrollBar();
    int maximum = bar->getRange() - bar->getPage();
    std::cerr << area->getClassName() << " initial maximum=" << maximum << '\n';
    CHECK(maximum > 20);
    for (int delta : {120, 30, 1}) {
      for (bool bottom : {false, true}) {
        area->setPosition(0, -(bottom ? maximum - 7 : 7));
        for (int i = 0; i < 120; ++i) {
          FX::FXEvent event{};
          event.code = bottom ? -delta : delta;
          area->handle(area, FXSEL(FX::SEL_MOUSEWHEEL, 0), &event);
          events(app, 1);
        }
        events(app);
        std::cerr << area->getClassName() << " delta=" << delta << " bottom=" << bottom
                  << " pos=" << bar->getPosition() << " max=" << maximum << '\n';
        CHECK(bar->getPosition() == (bottom ? maximum : 0));
        CHECK(area->getYPosition() == -(bottom ? maximum : 0));
        CHECK(!app.app.hasTimeout(bar, FX::FXScrollBar::ID_TIMEWHEEL));
      }
    }
    // Reverse immediately after repeated outward input at the bottom edge.
    FX::FXEvent reverse{};
    reverse.code = 30;
    area->handle(area, FXSEL(FX::SEL_MOUSEWHEEL, 0), &reverse);
    events(app);
    CHECK(bar->getPosition() < maximum);
    // Exercise the X11 -> FOX -> scroll-area path as well as fractional FOX events.
    area->setPosition(0, -7);
    XEvent native{};
    auto* display = static_cast<Display*>(app.app.getDisplay());
    native.xbutton.display = display;
    native.xbutton.window = area->id();
    native.xbutton.root = DefaultRootWindow(display);
    native.xbutton.same_screen = True;
    native.xbutton.x = 20;
    native.xbutton.y = 20;
    native.xbutton.button = Button4;
    native.type = ButtonPress;
    CHECK(XSendEvent(display, area->id(), False, ButtonPressMask, &native));
    native.type = ButtonRelease;
    CHECK(XSendEvent(display, area->id(), False, ButtonReleaseMask, &native));
    XFlush(display);
    events(app);
    CHECK(bar->getPosition() == 0 && area->getYPosition() == 0);
  }
  // Horizontal wheel motion uses the same adapter, including wheel-over-scrollbar.
  auto* tree = app.window->sidebar;
  auto* horizontal = tree->horizontalScrollBar();
  CHECK(horizontal->getRange() > horizontal->getPage());
  for (bool right : {false, true}) {
    int maximum = horizontal->getRange() - horizontal->getPage();
    tree->setPosition(-(right ? maximum - 3 : 3), 0);
    for (int i = 0; i < 120; ++i) {
      FX::FXEvent event{};
      event.code = right ? -1 : 1;
      horizontal->handle(horizontal, FXSEL(FX::SEL_MOUSEWHEEL, 0), &event);
      events(app, 1);
    }
    events(app);
    CHECK(horizontal->getPosition() == (right ? maximum : 0));
    CHECK(tree->getXPosition() == -(right ? maximum : 0));
  }
  auto* bar = app.host->verticalScrollBar();
  const int center = (bar->getRange() - bar->getPage()) / 2;
  const auto style = bar->getScrollBarStyle();
  bar->setScrollBarStyle(style | FX::SCROLLBAR_WHEELJUMP);
  for (auto modifier : {FX::ALTMASK, FX::CONTROLMASK}) {
    app.host->setPosition(0, -center);
    FX::FXEvent event{};
    event.code = 120;
    event.state = modifier;
    bar->handle(bar, FXSEL(FX::SEL_MOUSEWHEEL, 0), &event);
    CHECK(bar->getPosition() ==
          center - (modifier == FX::ALTMASK ? bar->getLine() : bar->getPage()));
  }
  bar->setScrollBarStyle(style);
}
TEST_MAIN(run)
