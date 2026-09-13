#include "application/Application.h"
#include "support/Capture.h"
#include "support/TestSupport.h"
#include <chrono>
#include <fstream>
#include <iterator>
#include <thread>
using namespace xfmd;
void events(Application& app, int ms = 150) {
  auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
  do {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  } while (std::chrono::steady_clock::now() < end);
}
void run() {
  int argc = 1;
  char name[] = "xfmd-table-preview";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  std::ifstream file(XFMD_TABLE_FIXTURE);
  std::string text{std::istreambuf_iterator<char>(file), {}};
  app.edits.applyEdit({0, 0, text});
  app.views->setMode(ViewMode::Split);
  events(app, 700);
  CHECK(app.host->interactive() && app.host->frame()->decorations.size() > 20);
  if (auto path = std::getenv("XFMD_CAPTURE_FILE"))
    captureDesktop(app.app, path);
  auto before = app.host->frame()->height;
  app.window->resize(720, 620);
  events(app);
  CHECK(app.host->interactive() && app.host->frame()->height >= before);
  app.execute(CommandRouter::A4);
  events(app);
  CHECK(app.host->interactive() && app.host->frame()->pages.slices.size() == 1);
  auto frame = app.host->frame();
  app.execute(CommandRouter::ActualSize);
  events(app);
  CHECK(app.host->frame() == frame);
  CHECK(app.session.snapshot().text == text && app.session.dirty());
}
TEST_MAIN(run)
