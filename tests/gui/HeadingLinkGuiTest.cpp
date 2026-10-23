#include "application/Application.h"
#include "support/NativeInput.h"
#include <filesystem>
#include <fstream>
using namespace xfmd;
namespace fs = std::filesystem;
void ready(Application& app) {
  for (int i = 0; i < 300; ++i) {
    settleNative(app.app, 20);
    if (app.host->interactive() && app.host->frame()->token == app.session.view().token)
      return;
  }
  throw std::runtime_error("Heading preview did not become ready");
}
void run() {
  fs::path home(FX::FXSystem::getHomeDirectory().text());
  auto source = home / "source.md", target = home / "target # æ.md";
  std::string text = "# Start\n\n";
  for (int i = 0; i < 60; ++i)
    text += "Paragraph before target " + std::to_string(i) + "\n\n";
  auto destination = text.size();
  text += "## SomeLocationInTheFile\n\n";
  for (int i = 0; i < 30; ++i)
    text += "Paragraph after target\n\n";
  const auto absolute = (home / "target%20%23%20%C3%A6.md").string() + "#SomeLocationInTheFile";
  std::ofstream(target) << text;
  std::ofstream(source) << "# Source\n\n[Absolute](" << absolute << ")\n\n";
  int argc = 1;
  char name[] = "heading-links";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(10, 10, 1200, 800);
  std::string error;
  app.documents.error = [&](const auto& message) { error = message; };
  CHECK(app.open(source.string()));
  ready(app);
  bool clicked = false;
  for (const auto& run : app.host->frame()->runs)
    if (run.link == absolute && run.text == "Absolute") {
      auto point = app.host->documentToView({run.bounds.x + 2, run.bounds.y + 2});
      nativeClick(app.host, int(point.x) + app.host->getXPosition(),
                  int(point.y) + app.host->getYPosition());
      clicked = true;
      break;
    }
  CHECK(clicked);
  ready(app);
  CHECK(app.session.view().path == target);
  CHECK(app.scrolling.captureAnchor().byte == destination);
  CHECK(app.host->getYPosition() < -100);
  CHECK(app.navigation->goBack());
  ready(app);
  CHECK(app.session.view().path == source);
  CHECK(app.navigation->goForward());
  ready(app);
  CHECK(app.scrolling.captureAnchor().byte == destination);
  app.edits.applyEdit({text.size(), text.size(), "\n## New heading\n"});
  ready(app);
  auto dirty = app.session.view().token;
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  app.followLink(target.string(), "#new-heading");
  ready(app);
  CHECK(app.session.dirty() && app.session.view().token == dirty);
  CHECK(app.scrolling.captureAnchor().byte == text.size() + 1);
  const auto before = app.scrolling.captureAnchor().byte;
  const auto history = app.navigation->history.position();
  app.followLink(target.string(), "source.md#Source");
  CHECK(app.session.view().token == dirty && app.navigation->history.position() == history);
  CHECK(app.scrolling.captureAnchor().byte == before);
  app.followLink(target.string(), "#missing");
  CHECK(error.find("Heading anchor not found") != std::string::npos);
  CHECK(app.scrolling.captureAnchor().byte == before);
  error.clear();
  app.followLink(target.string(), "#");
  CHECK(app.scrolling.captureAnchor().byte == 0);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  app.followLink(target.string(), "source.md#Source");
  ready(app);
  CHECK(app.session.view().path == source && app.scrolling.captureAnchor().byte == 0);
  app.followLink(source.string(), "target%20%23%20%C3%A6.md#somelocationinthefile");
  // A second open before parsing finishes must cancel the first pending fragment.
  CHECK(app.open(source.string()));
  ready(app);
  CHECK(app.session.view().path == source && app.scrolling.captureAnchor().byte == 0);
  app.followLink(source.string(), "target%20%23%20%C3%A6.md#somelocationinthefile");
  ready(app);
  CHECK(app.session.view().path == target && app.scrolling.captureAnchor().byte == destination);
  app.followLink(target.string(), "missing.md#Heading");
  CHECK(!error.empty() && app.session.view().path == target);
}
TEST_MAIN(run)
