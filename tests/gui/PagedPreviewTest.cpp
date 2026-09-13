#include "application/Application.h"
#include "support/Capture.h"
#include "support/TestSupport.h"
#include <chrono>
#include <thread>
using namespace xfmd;
void events(Application& app, int milliseconds = 100) {
  auto stop = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
  do {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  } while (std::chrono::steady_clock::now() < stop);
}
void run() {
  int argc = 1;
  char name[] = "paged-preview";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.views->setMode(ViewMode::Split);
  std::string source;
  for (int i = 0; i < 100; ++i)
    source +=
        "## Section " + std::to_string(i) + "\n\nA paragraph with **bold** and æøå content.\n\n";
  app.edits.applyEdit({0, 0, source});
  events(app, 600);
  CHECK(app.host->interactive());
  auto continuous = app.host->frame();
  auto anchor = source.find("## Section 40\n");
  app.window->editor->setTopLine(int(anchor));
  events(app);
  auto before = app.scrolling.captureAnchor();
  app.execute(CommandRouter::A4);
  events(app);
  auto paged = app.host->frame();
  CHECK(app.host->interactive());
  CHECK(paged->key.profile.mode == LayoutMode::Paged && paged->pages.slices.size() > 3);
  CHECK(app.session.dirty() && app.edits.canUndo());
  auto expected = AnchorMapper::map(before, *paged);
  CHECK(std::abs(-app.host->getYPosition() - app.host->documentToView({0, expected.y}).y) < 3);
  if (const auto* path = std::getenv("XFMD_CAPTURE_FILE"))
    captureDesktop(app.app, path);
  auto key = paged->key;
  app.window->resize(900, 680);
  events(app);
  CHECK(app.host->interactive() && app.host->frame()->key == key);
  app.execute(CommandRouter::ActualSize);
  events(app);
  CHECK(app.host->frame()->key == key && !app.host->fitWidth());
  app.host->present(continuous);
  CHECK(app.host->frame()->key == key);
  CHECK(!app.preview->acceptFrame(continuous));
  for (std::size_t i = 0; i < paged->pages.slices.size(); ++i) {
    Point p{100, i * paged->pages.paper.height + 100};
    auto q = app.host->viewToDocument(app.host->documentToView(p));
    CHECK(std::abs(q.x - p.x) < .001 && std::abs(q.y - p.y) < .001);
  }
  app.preview->refresh();
  events(app, 200);
  CHECK(app.host->interactive());
  app.execute(CommandRouter::WindowWrap);
  events(app);
  CHECK(app.host->interactive() && app.host->frame()->key.profile.mode == LayoutMode::Continuous);
  auto next = source.find("## Section 60\n");
  app.window->editor->setTopLine(int(next));
  events(app);
  CHECK(app.host->getYPosition() < 0);
}
TEST_MAIN(run)
