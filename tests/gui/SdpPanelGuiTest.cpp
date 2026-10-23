#include "application/Application.h"
#include "support/NativeInput.h"
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
}
TEST_MAIN(run)
