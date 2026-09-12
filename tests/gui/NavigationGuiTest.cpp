#include "application/Application.h"
#include "support/TestSupport.h"
#include <chrono>
#include <thread>
using namespace xfmd;
void events(Application& app) {
  for (int i = 0; i < 100; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
}
void run() {
  int argc = 1;
  char name[] = "xfmd-navigation";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.documents.error = [](const std::string& message) { throw std::runtime_error(message); };
  CHECK(app.open(XFMD_FIXTURE));
  events(app);
  CHECK(app.navigation->history.size() == 1 && app.host->interactive());
  const DrawRun* link = nullptr;
  for (const auto& run : app.host->frame()->runs)
    if (run.link == "next.md" && run.text != " ") {
      link = &run;
      break;
    }
  CHECK(link);
  FX::FXEvent event{};
  event.win_x = link->bounds.x + 1 + app.host->getXPosition();
  event.win_y = link->bounds.y + 1 + app.host->getYPosition();
  app.host->onPointer(nullptr, 0, &event);
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
}
TEST_MAIN(run)
