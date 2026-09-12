#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void events(Application& app) {
  for (int i = 0; i < 100; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
}
void click(Application& app, const DrawRun& link) {
  XEvent event{};
  auto* display = static_cast<Display*>(app.app.getDisplay());
  event.xbutton.display = display;
  event.xbutton.window = app.host->id();
  event.xbutton.root = DefaultRootWindow(display);
  event.xbutton.button = Button1;
  event.xbutton.same_screen = True;
  event.xbutton.x = link.bounds.x + 1 + app.host->getXPosition();
  event.xbutton.y = link.bounds.y + 1 + app.host->getYPosition();
  event.type = ButtonPress;
  CHECK(XSendEvent(display, app.host->id(), False, ButtonPressMask, &event));
  event.type = ButtonRelease;
  CHECK(XSendEvent(display, app.host->id(), False, ButtonReleaseMask, &event));
  XFlush(display);
}
void run() {
  int argc = 1;
  char name[] = "xfmd-navigation";
  char* argv[] = {name, nullptr};
  char pattern[] = "/tmp/xfmd-native-links-XXXXXX";
  std::filesystem::path directory = mkdtemp(pattern);
  struct Cleanup {
    std::filesystem::path directory;
    ~Cleanup() { std::filesystem::remove_all(directory); }
  } cleanup{directory};
  std::filesystem::create_directories(directory / "source");
  std::filesystem::create_directories(directory / "target folder");
  auto source = directory / "source/basics.md";
  auto target = directory / "target folder/next.md";
  std::ofstream(target) << "# Target\n";
  std::ofstream(source) << "# Source\n\n[Relative](../target%20folder/next.md)\n\n"
                        << "[Absolute](<" << target.string() << ">)\n";
  Application app;
  app.initialize(argc, argv);
  app.documents.error = [](const std::string& message) { throw std::runtime_error(message); };
  CHECK(app.open(source.string()));
  CHECK(app.host->isEnabled());
  events(app);
  CHECK(app.navigation->history.size() == 1 && app.host->interactive());
  const DrawRun* link = nullptr;
  for (const auto& run : app.host->frame()->runs)
    if (run.link == "../target%20folder/next.md" && run.text == "# ") {
      link = &run;
      break;
    }
  CHECK(link);
  click(app, *link);
  events(app);
  CHECK(app.navigation->history.size() == 2 &&
        app.session.view().path.find("next.md") != std::string::npos);
  CHECK(app.navigation->goBack());
  events(app);
  CHECK(app.session.view().path.find("basics.md") != std::string::npos && app.host->interactive());
  CHECK(app.navigation->goForward());
  events(app);
  CHECK(app.session.view().path.find("next.md") != std::string::npos);
  app.edits.applyEdit({0, 0, "dirty "});
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(!app.navigation->goBack() && app.navigation->history.position() == 1);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(app.navigation->goBack());
  events(app);
  link = nullptr;
  for (const auto& run : app.host->frame()->runs)
    if (run.link == target.string() && run.text == "Absolute") {
      link = &run;
      break;
    }
  CHECK(link);
  click(app, *link);
  events(app);
  CHECK(app.session.view().path == target.string());
}
TEST_MAIN(run)
