#include "application/Application.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void stable(Application& app) {
  settleNative(app.app, 200);
  for (int i = 0; i < 100; ++i) {
    if (app.host->interactive() && app.host->frame()->token == app.session.view().token)
      return;
    settleNative(app.app, 50);
  }
  CHECK(false);
}
const DrawRun& diagram(Application& app) {
  for (const auto& run : app.host->frame()->runs)
    if (dynamic_cast<const DiagramScene*>(run.visual.get()))
      return run;
  CHECK(false);
  throw std::runtime_error("No diagram");
}
void capture(Application& app, const char* name) {
  if (const auto* directory = getenv("XFMD_UI_EVIDENCE")) {
    fs::create_directories(directory);
    captureDesktop(app.app, (fs::path(directory) / name).c_str());
  }
}
void run() {
  int argc = 1;
  char name[] = "diagram-reading";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->resize(1100, 780);
  app.views->setMode(ViewMode::Preview);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(),
                 RevertToParent, CurrentTime);
  std::string source =
      "# Wide diagram\n\n```mermaid\nflowchart LR\n"
      "A[Input document] --> B[Interpret document] --> C[Validate semantic model]"
      " --> D[Generate diagram scene] --> E[Compose document] --> F[Display preview]\n"
      "```\n\nOrdinary text after the diagram.\n";
  if (const auto* path = getenv("XFMD_DIAGRAM_READING_SOURCE")) {
    std::ifstream in(path);
    CHECK(in.good());
    source.assign(std::istreambuf_iterator<char>(in), {});
  }
  app.edits.applyEdit({0, 0, source});
  stable(app);
  const auto token = app.session.view().token;
  const auto scene = diagram(app).visual;
  const auto* measured = dynamic_cast<const DiagramScene*>(scene.get());
  CHECK(measured && measured->width > diagram(app).bounds.width);
  const auto reading = app.host->frame()->readingText;
  app.window->editor->setCursorPos(2);
  app.window->editor->setSelection(0, 2);
  capture(app, "fit.png");

  auto* bar = app.window->previewControls->getParent()->getFirst();
  auto* title = dynamic_cast<FXMenuTitle*>(bar->getFirst()->getNext()->getNext());
  CHECK(title);
  FXMenuCheck* item = nullptr;
  for (auto* child = title->getMenu()->getFirst(); child; child = child->getNext())
    if (child->getSelector() == CommandRouter::FullSizeDiagrams)
      item = dynamic_cast<FXMenuCheck*>(child);
  CHECK(item && app.commands.enabled(CommandRouter::FullSizeDiagrams));
  auto toggle = [&] {
    nativeClick(title, 12, 10);
    CHECK(title->getMenu()->shown() && item->isEnabled());
    nativeClick(item, 16, item->getHeight() / 2);
    stable(app);
  };
  toggle();
  CHECK(app.commands.checked(CommandRouter::FullSizeDiagrams));
  CHECK(diagram(app).visual == scene && diagram(app).bounds.width == measured->width);
  CHECK(app.host->getContentWidth() > app.host->contentViewportWidth());
  CHECK(app.host->frame()->readingText == reading);
  const auto stale = app.host->frame();
  capture(app, "full-size.png");
  if (getenv("XFMD_DIAGRAM_READING_SOURCE")) {
    CHECK(!measured->nodes.empty());
    const auto& node = measured->nodes.front().bounds;
    const auto point = app.host->documentToView(
        {diagram(app).bounds.x + node.x, diagram(app).bounds.y + node.y});
    app.host->setPosition(-int(point.x) + 40, -int(point.y) + 40);
    stable(app);
    capture(app, "full-size-detail.png");
  }
  app.host->setPosition(-app.host->getContentWidth(), app.host->getYPosition());
  stable(app);
  CHECK(app.host->getXPosition() < 0);
  const auto right = app.host->documentToView(
      {diagram(app).bounds.x + diagram(app).bounds.width, diagram(app).bounds.y});
  CHECK(right.x + app.host->getXPosition() <= app.host->contentViewportWidth() + 1);
  app.execute(CommandRouter::Zoom200);
  stable(app);
  CHECK(diagram(app).visual == scene && diagram(app).bounds.width == measured->width);
  const auto origin = app.host->documentToView({0, 0});
  const auto edge = app.host->documentToView({measured->width, 0});
  CHECK(std::abs(edge.x - origin.x - measured->width * app.host->screenScale() * 2) < .01);
  capture(app, "full-size-200.png");
  app.window->resize(900, 700);
  stable(app);
  CHECK(diagram(app).bounds.width == measured->width);
  app.execute(CommandRouter::A4);
  stable(app);
  CHECK(!app.commands.enabled(CommandRouter::FullSizeDiagrams));
  const auto pagedWidth = diagram(app).bounds.width;
  CHECK(pagedWidth < measured->width);
  app.execute(CommandRouter::WindowWrap);
  app.execute(CommandRouter::ActualSize);
  stable(app);
  CHECK(diagram(app).bounds.width == measured->width);
  toggle();
  CHECK(!app.commands.checked(CommandRouter::FullSizeDiagrams));
  CHECK(!app.preview->acceptFrame(stale));
  CHECK(diagram(app).visual == scene && diagram(app).bounds.width < measured->width);
  // Companion tables can independently overflow; the focused fixture cannot.
  if (!getenv("XFMD_DIAGRAM_READING_SOURCE"))
    CHECK(app.host->getContentWidth() <= app.host->contentViewportWidth() + 2);
  app.execute(CommandRouter::A4);
  stable(app);
  CHECK(diagram(app).bounds.width == pagedWidth);
  CHECK(app.session.view().token == token && app.session.view().text == source);
  CHECK(app.session.dirty() && app.edits.canUndo());
  CHECK(app.window->editor->getCursorPos() == 2);
  CHECK(app.window->editor->getSelStartPos() == 0 && app.window->editor->getSelEndPos() == 2);
}
TEST_MAIN(run)
