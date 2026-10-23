#include "application/Application.h"
#include "application/ui/SdpTree.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/NativeInput.h"
#include "support/PathMenus.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
using namespace std::chrono_literals;
void until(Application& app, const std::function<bool()>& done) {
  for (int i = 0; i < 2000; ++i) {
    app.app.runWhileEvents();
    if (done())
      return;
    std::this_thread::sleep_for(5ms);
  }
  throw std::runtime_error("SDP GUI timeout: " +
                           std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
NavigationItem* find(NavigationTree* tree, FXTreeItem* first, const std::string& match) {
  for (auto* item = first; item; item = item->getNext()) {
    auto* row = static_cast<NavigationItem*>(item);
    if (row->action.target.find(match) != std::string::npos)
      return row;
    tree->expandTree(item);
    if (auto* child = find(tree, item->getFirst(), match))
      return child;
  }
  return nullptr;
}
void keyActivate(Application& app, NavigationTree* tree, NavigationItem* item) {
  CHECK(item);
  tree->setCurrentItem(item);
  tree->makeItemVisible(item);
  tree->setFocus();
  settleNative(app.app, 50);
  nativeKey(tree, XK_Return);
}
void tab(Application& app, FXTabBook* tabs, int index) {
  auto* item = tabs->getFirst();
  for (int i = 0; i < index; ++i)
    item = item->getNext()->getNext();
  nativeClick(item, 15, 8);
  CHECK(tabs->getCurrent() == index);
  settleNative(app.app, 30);
}
void run() {
  const char* engine = std::getenv("XFMD_SDP_TOOL");
  CHECK(engine && *engine);
  std::string actualEngine = engine;
  fs::path home = FXSystem::getHomeDirectory().text();
  auto project = home / "project with spaces", area = project / "SDP";
  fs::create_directories(area / "KanBan/backlog");
  fs::create_directories(area / "SDL");
  for (auto folder :
       {"backlog", "active", "onHold", "completed", "canceled", "superseded", "irrelevant"})
    fs::create_directories(area / "KanBan" / folder);
  std::ofstream(area / "KanBan/board.json")
      << R"({"schemaVersion":"0.2","projectId":"TEST","namespaces":["TEST"],"profile":"sdp-project-management/0.2","ledger":"Ledger.ndjson"})";
  std::ofstream(area / "KanBan/Ledger.ndjson")
      << R"({"eventId":"EVT-KB-TEST-000001","eventType":"x-kanban:created","subjectId":"KB-TEST-001","payload":{"schemaVersion":"0.2","previousEventId":null,"toPath":"backlog/#001--Proposal--Example.md"}})"
      << '\n';
  std::ofstream(area / "SDL/Example.design") << "language design-core version 0.5.\nfunctionality "
                                                "Browse.\nunit Desktop.\nDesktop owns Browse.\n";
  std::ofstream(area / "SDL/Example.sdui") << "sdui 0.2;\npage = [\"Hello from SDP\"];\n";
  std::ofstream(area / "SDL/Invalid.sdui") << "not SDUI\n";
  std::ofstream(area / "SDL/Invalid.design") << "not SDL\n";
  auto card = area / "KanBan/backlog/#001--Proposal--Example.md";
  std::ofstream(card)
      << "# A real process card\n\n| Field | Value |\n| --- | --- |\n"
         "| id | KB-TEST-001 |\n| project | TEST |\n| type | Proposal |\n| CardState | backlog |\n";
  auto other = home / "other.md";
  std::ofstream(other) << "# Other document\n";
  // Delay real generation to deterministically exercise admission races.
  auto wrapper = home / "delayed-sdptool";
  std::ofstream(wrapper)
      << "#!/usr/bin/python3\nimport os,sys,time\n"
         "if 'select' in sys.argv or 'sdui-preview' in sys.argv: time.sleep(0.35)\n"
         "os.execv(os.environ['XFMD_TEST_REAL_SDP'],[os.environ['XFMD_TEST_REAL_SDP']]+sys.argv[1:]"
         ")\n";
  fs::permissions(wrapper, fs::perms::owner_all);
  setenv("XFMD_TEST_REAL_SDP", actualEngine.c_str(), 1);
  setenv("XFMD_SDP_TOOL", wrapper.c_str(), 1);
  int argc = 1;
  char name[] = "sdp-navigation-test";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(20, 20, 1200, 800);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  auto* workspace = app.window->workspacePanel;
  auto* panel = workspace->sdp;
  CHECK(!workspace->tabs->getLast()->shown());
  CHECK(app.startPath(project.string()));
  until(app, [&] { return panel->tree("kanban") && panel->tree("kanban")->getFirstItem(); });
  CHECK(!fs::exists(fs::path(getenv("XDG_RUNTIME_DIR")) / "xfmd"));
  CHECK(workspace->tabs->getLast()->shown());
  settleNative(app.app, 1100); // Native layout and initial watch/discovery reconciliation.
  nativeClick(workspace->tabs->getLast(), 15, 8);
  CHECK(workspace->tabs->getCurrent() == 2);
  CHECK(!workspace->markdown->shown() && !workspace->upButton->shown());
  CHECK(panel->status->getWidth() > 200);
  tab(app, panel->tabs, 1); // Producer order is SDL, KanBan, SDUI.
  auto* cardRow = find(panel->tree("kanban"), panel->tree("kanban")->getFirstItem(), "KB-TEST-001");
  SdpTarget selectedCardTarget;
  const auto activate = panel->activated;
  panel->activated = [&](const SdpTarget& target) {
    selectedCardTarget = target;
    activate(target);
  };
  keyActivate(app, panel->tree("kanban"), cardRow);
  until(app, [&] { return app.session.view().path == card; });
  panel->activated = activate;
  // A typed SDP open target uses the same standalone-input transaction.
  auto mermaid = area / "standalone.mmd";
  std::ofstream(mermaid) << "flowchart LR\n A --> B\n";
  SdpTarget mermaidTarget;
  mermaidTarget.project = selectedCardTarget.project;
  mermaidTarget.operation = "open";
  mermaidTarget.path = mermaid.string();
  app.sdpNavigation->activate(mermaidTarget);
  until(app, [&] {
    return app.host->interactive() && app.host->frame()->token == app.session.view().token;
  });
  CHECK(app.session.view().path == mermaid && app.views->mode() == ViewMode::Preview);
  bool diagram = false;
  for (const auto& run : app.host->frame()->runs)
    diagram |= dynamic_cast<const DiagramScene*>(run.visual.get()) != nullptr;
  CHECK(diagram);
  CHECK(app.open(card.string()));
  tab(app, panel->tabs, 0);
  NavigationItem* viewRow = nullptr;
  until(app, [&] {
    viewRow = find(panel->tree("sdl"), panel->tree("sdl")->getFirstItem(), "/VP02/diagram/VP02-roots");
    return viewRow != nullptr;
  });
  CHECK(viewRow && viewRow->action.kind == IndexActionKind::Command);
  const auto viewAction = viewRow->action;
  keyActivate(app, panel->tree("sdl"), viewRow);
  until(app, [&] { return app.session.view().path != card; });
  CHECK(app.session.view().path.find("xfmd-sdp-") != std::string::npos);
  CHECK(fs::path(app.session.view().path).parent_path().parent_path().parent_path() ==
        fs::path(getenv("XDG_RUNTIME_DIR")) / "xfmd");
  CHECK(app.session.view().text.find("```mermaid") != std::string::npos);
  auto generated = app.session.view().path;
  until(app, [&] { return app.host->interactive(); });
  if (const auto* evidence = std::getenv("XFMD_UI_EVIDENCE")) {
    settleNative(app.app, 150);
    captureDesktop(app.app, (fs::path(evidence) / "sdp-sdl-view.png").c_str());
  }
  CHECK(app.open(other.string()));
  app.execute(CommandRouter::Back);
  until(app, [&] { return app.session.view().path == generated && app.host->interactive(); });
  CHECK(fs::exists(generated));
  app.execute(CommandRouter::Forward);
  until(app, [&] { return app.session.view().path == other; });
  app.execute(CommandRouter::Back);
  until(app, [&] { return app.session.view().path == generated; });
  // Freeze the watcher while exercising source menus; the dedicated dynamic and
  // deferred tests cover refresh races. Pending file-inventory reconciliation
  // must not replace the native row between locating it and injecting a click.
  app.sdpNavigation->stop();
  // Actual producer source catalog: descendant -> original .design, with native menu.
  auto* sourceTree = panel->tree("sdl");
  viewRow = find(sourceTree, sourceTree->getFirstItem(), "/VP02/diagram/VP02-roots");
  CHECK(viewRow && !static_cast<SdpItem*>(viewRow)->source.empty());
  SdpTarget openedSource;
  auto openSource = panel->sourceRequested;
  panel->sourceRequested = [&](const SdpTarget& target) {
    openedSource = target;
    openSource(target);
  };
  auto sourceMenu = [&](NavigationItem* row) {
    viewRow = row;
    sourceTree->makeItemVisible(viewRow);
    settleNative(app.app, 30);
    for (int y = 0; y < sourceTree->getViewportHeight(); ++y)
      if (sourceTree->getItemAt(90, y) == viewRow) {
        pathMenu(app, sourceTree, 90, y + 5, "View source");
        return;
      }
    throw std::runtime_error("Source row not visible");
  };
  app.edits.applyEdit({0, 0, "keep dirty "});
  auto sourceDirty = app.session.snapshot();
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  sourceMenu(find(sourceTree, sourceTree->getFirstItem(), "/VP02/diagram/VP02-roots"));
  CHECK(app.session.dirty() && app.session.view().token == sourceDirty.token);
  CHECK(app.session.view().path == generated);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  sourceMenu(find(sourceTree, sourceTree->getFirstItem(), "/VP02/diagram/VP02-roots"));
  CHECK(app.session.view().path == area / "SDL/Example.design");
  CHECK(app.views->mode() == ViewMode::Editor && !app.session.dirty());
  CHECK(app.session.view().text.find("language design-core") == 0);
  if (const auto* output = getenv("XFMD_UI_EVIDENCE")) {
    settleNative(app.app, 100);
    captureDesktop(app.app, (fs::path(output) / "sdp-original-source-editor.png").c_str());
  }
  // Source selection is revalidated independently of menu presentation.
  const auto originalSource = openedSource;
  CHECK(app.open(other.string()));
  auto staleSource = originalSource;
  staleSource.revision = "obsolete";
  app.sdpNavigation->openSource(staleSource);
  CHECK(app.session.view().path == other && panel->status->getText().contains("changed"));
  staleSource = originalSource;
  staleSource.path = other.string();
  app.sdpNavigation->openSource(staleSource);
  CHECK(app.session.view().path == other && panel->status->getText().contains("outside"));
  app.sdpNavigation->stop(); // Stabilize catalog while testing filesystem changes.
  auto originalPath = area / "SDL/Example.design", backupPath = area / "SDL/Backup.design";
  fs::rename(originalPath, backupPath);
  app.sdpNavigation->openSource(originalSource);
  CHECK(app.session.view().path == other);
  fs::create_symlink(other, originalPath);
  app.sdpNavigation->openSource(originalSource);
  CHECK(app.session.view().path == other);
  fs::remove(originalPath);
  fs::rename(backupPath, originalPath);
  // Invalid SDL still offers its catalog source without a generation target.
  NavigationItem* invalidRow = nullptr;
  for (auto* row = sourceTree->getFirstItem(); row; row = row->getNext())
    if (row->getText().contains("Invalid.design"))
      invalidRow = static_cast<NavigationItem*>(row);
  CHECK(invalidRow && invalidRow->action.kind != IndexActionKind::Command);
  sourceMenu(invalidRow);
  CHECK(app.session.view().path == area / "SDL/Invalid.design");
  CHECK(app.session.view().text == "not SDL\n" && app.views->mode() == ViewMode::Editor);
  panel->sourceRequested = openSource;
  workspace->sdpRefresh();
  until(app, [&] { return panel->status->getText().contains("Navigation updated"); });
  settleNative(app.app, 1100); // Complete restarted watch baseline before diagnostic assertions.
  CHECK(app.open(generated));
  // Dirty at selection: no unsaved dialog, no implicit save/discard, no tool delivery.
  app.documents.chooseUnsaved = []() -> UnsavedChoice {
    throw std::runtime_error("Unexpected dirty prompt");
  };
  app.edits.applyEdit({0, 0, "dirty "});
  auto dirty = app.session.snapshot();
  panel->tree("sdl")->activated(viewAction);
  settleNative(app.app, 500);
  CHECK(app.session.view().token == dirty.token && app.session.dirty());
  CHECK(panel->status->getText().contains("Save or discard"));
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(app.open(card.string()));
  // Editing after generation starts invalidates the captured document token.
  panel->tree("sdl")->activated(viewAction);
  app.edits.applyEdit({0, 0, "keep these edits "});
  dirty = app.session.snapshot();
  until(app, [&] { return panel->status->getText().contains("document changed"); });
  CHECK(app.session.view().token == dirty.token && app.session.dirty());
  CHECK(app.open(card.string()));
  // Switching documents while a request runs also rejects late delivery.
  panel->tree("sdl")->activated(viewAction);
  CHECK(app.open(other.string()));
  until(app, [&] { return panel->status->getText().contains("document changed"); });
  CHECK(app.session.view().path == other);
  // Newer selection wins even with the same source revision.
  panel->tree("sdl")->activated(viewAction);
  panel->tree("sdl")->activated(viewAction);
  until(app, [&] { return app.session.view().path != other; });
  CHECK(fs::exists(generated)); // Retained for Back/Forward.
  // SDUI is a real producer-generated Markdown/SVG document, not a widget host.
  tab(app, panel->tabs, 2);
  auto* frame = find(panel->tree("sdui"), panel->tree("sdui")->getFirstItem(), "/frame/page");
  auto previous = app.session.view().path;
  keyActivate(app, panel->tree("sdui"), frame);
  until(app, [&] { return app.session.view().path != previous; });
  CHECK(app.session.view().text.find("page") != std::string::npos);
  until(app, [&] { return app.host->interactive(); });
  if (const auto* evidence = std::getenv("XFMD_UI_EVIDENCE")) {
    settleNative(app.app, 150);
    captureDesktop(app.app, (fs::path(evidence) / "sdp-sdui-view.png").c_str());
  }
  // SDUI frame/source context uses the same real menu and document transaction.
  sourceTree = panel->tree("sdui");
  panel->sourceRequested = [&](const SdpTarget& target) {
    openedSource = target;
    openSource(target);
  };
  app.edits.applyEdit({0, 0, "keep prototype notes "});
  sourceDirty = app.session.snapshot();
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  sourceMenu(frame);
  CHECK(app.session.dirty() && app.session.view().token == sourceDirty.token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  sourceMenu(frame);
  CHECK(app.session.view().path == area / "SDL/Example.sdui");
  CHECK(app.views->mode() == ViewMode::Editor && !app.session.dirty());
  CHECK(app.session.view().text.find("sdui 0.2;") == 0);
  staleSource = openedSource;
  staleSource.revision = "obsolete";
  const auto uiSourceToken = app.session.view().token;
  app.sdpNavigation->openSource(staleSource);
  CHECK(app.session.view().token == uiSourceToken && panel->status->getText().contains("changed"));
  staleSource = openedSource;
  staleSource.path = other.string();
  app.sdpNavigation->openSource(staleSource);
  CHECK(app.session.view().token == uiSourceToken && panel->status->getText().contains("outside"));
  if (const auto* evidence = std::getenv("XFMD_UI_EVIDENCE")) {
    settleNative(app.app, 100);
    captureDesktop(app.app, (fs::path(evidence) / "sdp-sdui-source-editor.png").c_str());
  }
  // Invalid SDUI also remains inspectable without a preview target.
  invalidRow = nullptr;
  for (auto* row = sourceTree->getFirstItem(); row; row = row->getNext())
    if (row->getText().contains("Invalid.sdui"))
      invalidRow = static_cast<NavigationItem*>(row);
  CHECK(invalidRow && invalidRow->action.kind != IndexActionKind::Command);
  sourceMenu(invalidRow);
  CHECK(app.session.view().path == area / "SDL/Invalid.sdui");
  CHECK(app.session.view().text == "not SDUI\n" && app.views->mode() == ViewMode::Editor);
  panel->sourceRequested = openSource;
  previous = app.session.view().path;
  panel->tree("sdl")->activated(viewAction);
  CHECK(workspace->setWorkPath(home.string()));
  settleNative(app.app, 600);
  CHECK(app.session.view().path == previous && workspace->tabs->getCurrent() == 0);
  CHECK(!workspace->tabs->getLast()->shown() && workspace->markdown->shown());
  // Direct SDP root works; moved cards refresh automatically and keep selection.
  CHECK(workspace->setWorkPath(area.string()));
  until(app, [&] { return panel->tree("kanban") && panel->tree("kanban")->getFirstItem(); });
  settleNative(app.app, 1100);
  auto* selectedCard =
      find(panel->tree("kanban"), panel->tree("kanban")->getFirstItem(), "KB-TEST-001");
  CHECK(selectedCard);
  panel->tree("kanban")->setCurrentItem(selectedCard);
  app.edits.applyEdit({0, 0, "preserve during refresh "});
  const auto watchedToken = app.session.view().token;
  fs::create_directories(area / "KanBan/active");
  fs::rename(card, area / "KanBan/active" / card.filename());
  std::ofstream(area / "KanBan/Ledger.ndjson", std::ios::app)
      << R"({"eventId":"EVT-KB-TEST-000002","eventType":"x-kanban:moved","subjectId":"KB-TEST-001","payload":{"schemaVersion":"0.2","previousEventId":"EVT-KB-TEST-000001","toPath":"active/#001--Proposal--Example.md"}})"
      << '\n';
  {
    std::ofstream moved(area / "KanBan/active" / card.filename());
    moved << "# Active card\n| Field | Value |\n| --- | --- |\n| id | KB-TEST-001 |\n| CardState | "
             "in-progress |\n";
  }
  until(app, [&] {
    auto* row = panel->tree("kanban")->getCurrentItem();
    return row && row->getParent() && row->getParent()->getText() == "active";
  });
  CHECK(app.session.view().path == previous);
  auto* moved = find(panel->tree("kanban"), panel->tree("kanban")->getFirstItem(), "KB-TEST-001");
  CHECK(moved && std::string(moved->getParent()->getText().text()) == "active");
  CHECK(app.session.dirty() && app.session.view().token == watchedToken);
  CHECK(app.open(other.string()));
  previous = other.string();
  // Load the model before changing it; deferred discovery has no stale detail
  // target until expansion finishes. Freeze the watcher to test producer refusal.
  NavigationItem* stale = nullptr;
  until(app, [&] {
    stale = find(panel->tree("sdl"), panel->tree("sdl")->getFirstItem(), "/VP02/diagram/VP02-roots");
    return stale != nullptr;
  });
  app.sdpNavigation->stop();
  const auto staleAction = stale->action;
  // Changed model revision is diagnosed by the producer, keeping current document.
  std::ofstream(area / "SDL/Example.design")
      << "language design-core version 0.5.\nfunctionality Browse.\nunit Desktop.\nunit "
         "Later.\nDesktop owns Browse.\n";
  panel->tree("sdl")->activated(staleAction);
  until(app, [&] { return panel->status->getText().contains("stale"); });
  CHECK(app.session.view().path == previous);
  app.sdpNavigation->stop();
  // Unavailable producer remains an SDP diagnostic; normal Files/Index survive.
  setenv("XFMD_SDP_TOOL", "/missing/sdptool", 1);
  workspace->sdpRefresh();
  until(app, [&] { return panel->status->getText().contains("Cannot start"); });
  CHECK(app.session.view().path == previous);
  unsetenv("XFMD_TEST_REAL_SDP");
}
TEST_MAIN(run)
