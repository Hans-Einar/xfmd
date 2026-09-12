#include "application/Application.h"
#include "support/TestSupport.h"
#include <chrono>
#include <thread>
using namespace xfmd;
void run() {
  int argc = 1;
  char name[] = "xfmd-presentation";
  char* argv[] = {name, nullptr};
  Application application;
  application.initialize(argc, argv);
  application.documents.error = [](const std::string& error) { throw std::runtime_error(error); };
  CHECK(application.open(XFMD_FIXTURE));
  application.views->setMode(ViewMode::Split);
  application.window->setFocus();
  for (int i = 0; i < 100; ++i) {
    application.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  CHECK(application.host->frame());
  CHECK(application.host->interactive());
  CHECK(application.host->frame()->token == application.session.view().token);
  bool heading = false, monospace = false;
  for (auto& run : application.host->frame()->runs) {
    heading |= run.font.points == 26;
    monospace |= run.font.mono;
    CHECK(application.metrics->measure(run.text, run.font).width == run.bounds.width);
  }
  CHECK(heading && monospace);
  application.window->editor->setFocus();
  CHECK(application.window->editor->hasFocus());
  application.window->editor->appendText(FX::FXString("\nLive edit"), true);
  auto cursor = application.window->editor->getCursorPos();
  CHECK(!application.host->interactive());
  for (int i = 0; i < 100; ++i) {
    application.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  CHECK(application.host->interactive());
  CHECK(application.host->frame()->token == application.session.view().token);
  CHECK(application.window->editor->getCursorPos() == cursor);
  CHECK(application.window->editor->hasFocus());
  auto old = application.host->frame();
  application.host->expect({999, 0});
  application.host->present(old);
  CHECK(!application.host->interactive());
}
TEST_MAIN(run)
