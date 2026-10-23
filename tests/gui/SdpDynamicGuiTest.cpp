#include "application/Application.h"
#include "application/ui/SdpTree.h"
#include "support/PathMenus.h"
using namespace xfmd;
using namespace FX;
SdpReply graph() {
  SdpReply r;
  r.root = "/project";
  r.project = "TEST";
  r.roots = {"files", "sdl", "kanban"};
  for (auto id : {"files", "sdl", "kanban", "extension-tab"}) {
    auto& n = r.nodes[id];
    n.id = n.label = id;
    n.kind = std::string(id) == "files" ? "directory" : "tab";
    n.state = "available";
  }
  r.nodes["extension-tab"].label = "Sessions";
  r.sources["model"] = {"model", "sdl", "SDP/Entry.design", "r1"};
  auto& source = r.nodes["sdl/model"];
  source.id = "sdl/model";
  source.label = "Model";
  source.kind = "source";
  source.children = {"a0", "a1"};
  auto& a1 = r.nodes["a1"];
  a1.id = a1.label = "a1";
  a1.kind = "abstraction";
  a1.children = {"alias"};
  auto& a0 = r.nodes["a0"];
  a0.id = "a0";
  a0.label = "A0";
  a0.kind = "abstraction";
  a0.children = {"alias"};
  auto& alias = r.nodes["alias"];
  alias.id = "alias";
  alias.label = "VP02";
  alias.reference = "vp";
  alias.kind = "reference";
  auto& vp = r.nodes["vp"];
  vp.id = "vp";
  vp.label = "VP02 — Architecture and logical decomposition";
  vp.kind = "viewpoint";
  vp.children = {"focused"};
  vp.target = {"select", "TEST", "model", "bulk", "r1"};
  auto& object = r.nodes["object"];
  object.id = object.label = "object";
  object.kind = "functionality";
  object.target = {"select", "TEST", "model", "VP11-generic", "r1"};
  auto& focused = r.nodes["focused"];
  focused.id = focused.label = "focused";
  focused.kind = "reference";
  focused.reference = "object";
  focused.target = {"select", "TEST", "model", "VP02-focused", "r1"};
  r.nodes["sdl"].children = {"sdl/model"};
  return r;
}
int rowY(Application& app, NavigationTree* tree, FXTreeItem* row) {
  tree->makeItemVisible(row);
  settleNative(app.app, 30);
  for (int y = 0; y < tree->getViewportHeight(); ++y)
    if (tree->getItemAt(90, y) == row)
      return y + 5;
  throw std::runtime_error("Missing visible SDP row");
}
struct RefreshMenu : PathMenuDriver {
  SdpPanel* panel;
  SdpReply reply;
  bool refreshed = false;
  explicit RefreshMenu(Application& app, SdpReply r)
      : PathMenuDriver(app), panel(app.window->workspacePanel->sdp), reply(std::move(r)) {}
  long handle(FXObject* s, FXSelector sel, void* d) override {
    if (!refreshed) {
      panel->clear();
      panel->present(reply);
      refreshed = true;
    }
    return PathMenuDriver::handle(s, sel, d);
  }
};
void run() {
  int argc = 1;
  char name[] = "dynamic-sdp-test";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.sdpNavigation->stop();
  auto* workspace = app.window->workspacePanel;
  auto* p = workspace->sdp;
  app.window->position(20, 20, 1200, 800);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  auto r = graph();
  p->present(r);
  workspace->showSdp(true);
  settleNative(app.app);
  nativeClick(workspace->tabs->getLast(), 15, 8);
  CHECK(p->trees.size() == 2 && !p->tree("files"));
  CHECK(p->selectedTab() == "sdl");
  auto* tree = p->tree("sdl");
  auto* source = tree->getFirstItem();
  tree->expandTree(source);
  auto* viewpoint = source->getFirst();
  CHECK(viewpoint->getText() == r.nodes.at("vp").label.c_str());
  CHECK(!viewpoint->getNext());
  CHECK(static_cast<SdpItem*>(viewpoint)->source == "model");
  tree->setCurrentItem(viewpoint);
  int activations = 0;
  p->activated = [&](const SdpTarget& target) {
    CHECK(target.uri == "VP02-focused");
    ++activations;
  };
  // Native expansion box must never dispatch the row's generation action.
  rowY(app, tree, viewpoint);
  tree->setPosition(0, tree->getYPosition());
  settleNative(app.app, 30);
  int boxX = -1, boxY = -1;
  for (int y = 0; y < tree->getViewportHeight() && boxX < 0; ++y)
    for (int x = 0; x < tree->getViewportWidth(); ++x)
      if (tree->hitItem(viewpoint, x, y) == 3) { boxX = x; boxY = y; break; }
  CHECK(boxX >= 0);
  nativeClick(tree, boxX, boxY);
  settleNative(app.app);
  CHECK(viewpoint->isExpanded() && activations == 0);
  auto* focused = viewpoint->getFirst();
  CHECK(focused && static_cast<SdpItem*>(focused)->id == "focused");
  tree->setCurrentItem(focused);
  tree->setFocus();
  nativeKey(tree, XK_Return);
  settleNative(app.app);
  CHECK(activations == 1);
  if (const auto* output = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (std::filesystem::path(output) / "sdl-clean-tree.png").c_str());
  tree->setCurrentItem(viewpoint);
  int chosen = 0;
  p->sourceRequested = [&](const SdpTarget& t) {
    CHECK(t.path == "/project/SDP/Entry.design");
    ++chosen;
  };
  p->activated = [](const SdpTarget&) {
    throw std::runtime_error("Right click activated normal action");
  };
  pathMenu(app, tree, 90, rowY(app, tree, viewpoint), "View source", "sdp-source-menu.png");
  CHECK(chosen == 1);
  r.roots = {"extension-tab", "kanban", "files", "sdl"};
  r.nodes["a0"].label = "A0 — Producer-defined overview";
  p->present(r);
  settleNative(app.app);
  CHECK(p->trees.size() == 3 && p->selectedTab() == "sdl" && p->tabs->getCurrent() == 2);
  tree = p->tree("sdl");
  source = tree->getFirstItem();
  CHECK(source->isExpanded());
  CHECK(source->getFirst()->getText() == r.nodes.at("vp").label.c_str());
  CHECK(static_cast<SdpItem*>(tree->getCurrentItem())->id == "vp");
  nativeClick(p->tabs->getFirst(), 15, 8);
  CHECK(p->selectedTab() == "extension-tab");
  if (const auto* output = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app,
                   (std::filesystem::path(output) / "sdp-dynamic-sessions-fixture.png").c_str());
  r.nodes["extension-tab"].label = "Session notes";
  p->present(r);
  settleNative(app.app);
  CHECK(static_cast<FXTabItem*>(p->tabs->getFirst())->getText() == "Session notes");
  r.roots = {"kanban", "sdl"};
  r.nodes["kanban"].state = "unavailable";
  r.nodes["kanban"].diagnostic = "Producer unavailable";
  p->present(r);
  settleNative(app.app);
  CHECK(p->selectedTab() == "sdl"); // Removed tab falls back to first usable root.
  // Changing topology inside FOX's modal menu must not destroy the dispatching tree.
  tree = p->tree("sdl");
  source = tree->getFirstItem();
  auto replacement = r;
  replacement.roots = {"extension-tab"};
  RefreshMenu driver(app, replacement);
  driver.action = "View source";
  nativeClick(tree, 90, rowY(app, tree, source), 0, Button3, false);
  app.app.addTimeout(&driver, 1, 100);
  settleNative(app.app);
  CHECK(driver.visited && chosen == 1 && p->selectedTab() == "extension-tab");
  r.roots.clear();
  p->present(r);
  settleNative(app.app);
  CHECK(p->trees.empty() && !p->tabs->shown() && p->selectedTab().empty());
  nativeClick(workspace->tabs->getFirst(), 15, 8);
  CHECK(workspace->tabs->getCurrent() == 0 && workspace->markdown->shown());
  p->present(graph());
  settleNative(app.app);
  CHECK(p->trees.size() == 2 && p->selectedTab() == "sdl");
  // An empty viewpoint from an older producer must not invoke its broad target.
  auto oldProducer = graph(); oldProducer.nodes["vp"].children.clear();
  p->present(oldProducer); nativeClick(workspace->tabs->getLast(),15,8);
  tree=p->tree("sdl"); tree->expandTree(tree->getFirstItem());
  viewpoint=tree->getFirstItem()->getFirst(); CHECK(viewpoint);
  p->activated=[](const SdpTarget&) { throw std::runtime_error("Unavailable overview dispatched bulk target"); };
  tree->setCurrentItem(viewpoint);tree->makeItemVisible(viewpoint);tree->setFocus();
  nativeKey(tree,XK_Return);settleNative(app.app);
  CHECK(p->status->getText().contains("Overview unavailable"));
}
TEST_MAIN(run)
