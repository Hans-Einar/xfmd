#include "application/Application.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/PathMenus.h"
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void rendered(Application& app) {
  for (int i = 0; i < 400; ++i) {
    settleNative(app.app, 20);
    if (app.host->interactive() && app.host->frame()->key.token == app.session.view().token)
      for (const auto& run : app.host->frame()->runs)
        if (dynamic_cast<const DiagramScene*>(run.visual.get()))
          return;
  }
  throw std::runtime_error("Standalone Mermaid diagram did not render");
}
void run() {
  fs::path home(FXSystem::getHomeDirectory().text());
  auto path = home / "feature æ space.MMD", other = home / "other.txt";
  fs::copy_file(XFMD_MMD_FIXTURE, path);
  std::ofstream(other) << "other";
  int argc = 1;
  char name[] = "standalone-mermaid";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(20, 20, 1200, 800);
  auto* workspace = app.window->workspacePanel;
  std::string error;
  app.documents.error = [&](const auto& message) { error = message; };
  app.views->setMode(ViewMode::Editor);
  CHECK(app.startPath(path.string())); // Same entry used by main's CLI dispatch.
  CHECK(app.views->mode() == ViewMode::Preview);
  rendered(app);
  if (const char* output = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (fs::path(output) / "standalone-preview.png").c_str());
  const auto original = app.session.view().text;
  app.views->setMode(ViewMode::Split);
  app.edits.applyEdit({original.size(), original.size(), "\n%% Edited in XFMD\n"});
  rendered(app);
  CHECK(app.session.dirty());
  const auto dirty = app.session.snapshot();
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(!app.openTarget(other.string()));
  CHECK(app.session.view().token == dirty.token && app.views->mode() == ViewMode::Split);
  CHECK(app.documents.save());
  CHECK(LocalFileStore().read(path.string()).text == dirty.text);
  CHECK(app.openTarget(other.string()));
  CHECK(app.views->mode() == ViewMode::Editor);
  app.execute(CommandRouter::Back);
  rendered(app);
  CHECK(app.session.view().path == path && app.views->mode() == ViewMode::Preview);
  app.execute(CommandRouter::Forward);
  CHECK(app.session.view().path == other);
  CHECK(app.openDialogPath(path.string()));
  rendered(app);
  CHECK(app.session.view().text == dirty.text);
  CHECK(app.openTarget(other.string()));
  // Actual Files-tree pointer activation, with the existing .md-only filter off.
  if (workspace->markdown->isChecked())
    nativeClick(workspace->markdown, 10, 10);
  auto* tree = app.window->sidebar;
  auto* item = tree->getPathnameItem(path.c_str());
  CHECK(item);
  tree->makeItemVisible(item);
  settleNative(app.app);
  bool clicked = false;
  for (int y = 0; y < tree->getViewportHeight(); ++y)
    if (tree->getItemAt(90, y) == item) {
      nativeClick(tree, 90, y + 5);
      clicked = true;
      break;
    }
  CHECK(clicked);
  rendered(app);
  CHECK(app.session.view().path == path);
  CHECK(app.openTarget(other.string()));
  workspace->recentFiles->open(path.string());
  rendered(app);
  CHECK(workspace->favorites->add(path.string()));
  CHECK(app.openTarget(other.string()));
  workspace->favorites->open(workspace->favorites->entries().front());
  rendered(app);
  CHECK(app.openTarget(other.string()));
  app.followLink((home / "index.md").string(), "feature%20%C3%A6%20space.MMD", false);
  rendered(app);
  CHECK(app.session.view().path == path);
  app.execute(CommandRouter::A4);
  rendered(app);
  CHECK(app.host->frame()->pages.slices.size() >= 1);
  CHECK(error.empty());
}
TEST_MAIN(run)
