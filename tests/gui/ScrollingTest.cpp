#include "application/Application.h"
#include "support/TestSupport.h"
#include <chrono>
#include <thread>
using namespace xfmd;
void events(Application& app, int milliseconds) {
  auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
  do { app.app.runWhileEvents(); std::this_thread::sleep_for(std::chrono::milliseconds(2)); } while (std::chrono::steady_clock::now() < end);
}
void run() {
  int argc = 1; char name[] = "xfmd-scroll"; char* argv[] = {name, nullptr};
  Application app; app.initialize(argc, argv); app.views->setMode(ViewMode::Split);
  std::string source;
  for (int i=0;i<80;++i) source += "## Section " + std::to_string(i) + "\n\nParagraph with **bold** and long content to wrap in a narrower window.\n\n";
  app.edits.applyEdit({0, 0, source});
  events(app, 700);
  CHECK(app.host->interactive());
  auto anchor = source.find("## Section 30\n");
  // Invoke an actual FOX scroll command via setTopLine (not a fake scroll callback).
  app.window->editor->setTopLine(int(anchor));
  events(app, 30);
  auto target = AnchorMapper::map({anchor}, *app.host->frame());
  CHECK(std::abs(-app.host->getYPosition() - target.y) < 60);
  auto next = source.find("## Section 40\n");
  auto nextY = AnchorMapper::map({next}, *app.host->frame()).y;
  app.host->setPosition(0, -nextY);
  events(app, 30);
  CHECK(std::abs(long(app.window->editor->sourceAnchor()) - long(next)) < 120);
  auto before = app.scrolling.captureAnchor();
  app.window->resize(850, 650);
  events(app, 100);
  CHECK(app.host->interactive());
  auto expected = AnchorMapper::map(before, *app.host->frame()).y;
  CHECK(std::abs(-app.host->getYPosition() - expected) < 70);
}
TEST_MAIN(run)
