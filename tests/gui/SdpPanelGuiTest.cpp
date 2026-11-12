#include "application/Application.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
using namespace xfmd;
SdpReply graph() {
  SdpReply value;
  value.project = "test";
  value.roots = {"kanban", "sdl", "sdui"};
  for (auto name : {"kanban", "sdl", "sdui"}) {
    auto& node = value.nodes[name];
    node.id = node.label = name;
    node.kind = "tab";
  }
  return value;
}
int expandAll(NavigationTree* tree, FX::FXTreeItem* first) {
  int rows = 0;
  for (auto* item = first; item; item = item->getNext()) {
    tree->expandTree(item);
    rows += 1 + expandAll(tree, item->getFirst());
  }
  return rows;
}
void run() {
  int argc = 1;
  char name[] = "sdp-panel-test";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.sdpNavigation->stop();
  auto* workspace = app.window->workspacePanel;
  auto* panel = workspace->sdp;
  auto value = graph();
  value.nodes["sdl"].children = {"cycle"};
  auto& cycle = value.nodes["cycle"];
  cycle.id = cycle.label = "cycle";
  cycle.reference = "cycle";
  panel->present(value);
  CHECK(expandAll(panel->trees[1], panel->trees[1]->getFirstItem()) == 0);
  value.nodes["sdl"].children = {"0"};
  for (int i = 0; i < 100; ++i) {
    auto& node = value.nodes[std::to_string(i)];
    node.id = node.label = std::to_string(i);
    if (i < 99)
      node.children = {std::to_string(i + 1)};
  }
  panel->present(value);
  CHECK(expandAll(panel->trees[1], panel->trees[1]->getFirstItem()) <= 32);
  CHECK(panel->status->getText().contains("depth limit"));
  value.nodes["sdl"].children.clear();
  for (int i = 0; i < 5000; ++i) {
    auto& node = value.nodes[std::to_string(i)];
    node.id = node.label = std::to_string(i);
    node.children.clear();
    value.nodes["sdl"].children.push_back(node.id);
  }
  panel->present(value);
  CHECK(expandAll(panel->trees[1], panel->trees[1]->getFirstItem()) <= 4097);
  CHECK(panel->trees[1]->getLastItem()->getText().contains("Display limit"));
  workspace->showSdp(true);
  settleNative(app.app);
  auto* tab = workspace->tabs->getLast();
  CHECK(tab->getX() + tab->getWidth() <= workspace->tabs->getWidth());
  nativeClick(tab, 15, 8);
  CHECK(workspace->tabs->getCurrent() == 2);
  CHECK(panel->status->getWidth() >= 200);
  workspace->setWidth(260);
  workspace->getParent()->recalc();
  settleNative(app.app);
  nativeClick(workspace->tabs->getFirst(), 15, 8);
  CHECK(workspace->tabs->getCurrent() == 0 && workspace->markdown->shown());
  CHECK(tab->getX() + tab->getWidth() <= workspace->tabs->getWidth());
  nativeClick(tab, 15, 8);
  CHECK(workspace->tabs->getCurrent() == 2);

  // Exact producer order/labels and independent primary activation versus loading.
  value = graph();
  value.presentation = "producer-reading/1";
  value.nodes["sdl"].children = {"source"};
  auto& readingSource = value.nodes["source"];
  readingSource.id = "source"; readingSource.label = "System entry";
  readingSource.kind = "source"; readingSource.childrenDeferred = true;
  readingSource.target = {"select", "test", "m", "opaque-primary", "r1"};
  readingSource.expansionTarget = {"tree", "test", "m", "", "r1"};
  int activated = 0, expanded = 0;
  panel->activated = [&](const SdpTarget& t) { CHECK(t.uri == "opaque-primary"); ++activated; };
  panel->modelRequested = [&](const SdpTarget& t) { CHECK(t.operation == "tree" && t.uri.empty()); ++expanded; };
  panel->present(value);
  panel->tabs->setCurrent(1);
  settleNative(app.app);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent, CurrentTime);
  auto* readingTree = panel->tree("sdl");
  auto* readingRow = readingTree->getFirstItem();
  readingTree->setCurrentItem(readingRow); readingTree->setFocus();
  nativeKey(readingTree, XK_Right); settleNative(app.app);
  CHECK(expanded == 1 && activated == 0);
  nativeKey(readingTree, XK_Return); settleNative(app.app);
  CHECK(expanded == 1 && activated == 1);

  readingSource.childrenDeferred = false;
  readingSource.expansionTarget = {};
  readingSource.children = {"vp-late", "phase", "vp-early"};
  for (const auto& id : readingSource.children) {
    auto& n = value.nodes[id]; n.id = id; n.label = id;
    n.kind = id == "phase" ? "phase" : "viewpoint";
    n.target = readingSource.target;
    n.overviewTarget = {"select", "test", "m", "legacy-overview-must-not-win", "r1"};
    n.cardType = "hidden-internal-id";
  }
  panel->present(value);
  readingTree = panel->tree("sdl");
  readingRow = readingTree->getFirstItem();
  readingTree->expandTree(readingRow);
  auto* child = readingRow->getFirst();
  for (const auto& id : readingSource.children) {
    CHECK(child && child->getText() == id.c_str()); child = child->getNext();
  }
  CHECK(!child);
  readingTree->setCurrentItem(readingRow->getFirst()); readingTree->setFocus();
  nativeKey(readingTree, XK_Return); settleNative(app.app);
  CHECK(activated == 2 && expanded == 1);
}
TEST_MAIN(run)
