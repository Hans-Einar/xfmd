#include "SdpPanel.h"
#include "SdpTree.h"
#include "application/sdp/SdpSources.h"
#include "application/sdp/SdpDisplay.h"
#include <algorithm>
#include <set>
using namespace FX;
namespace xfmd {
namespace {
class SdpStatus : public FXLabel {
public:
  explicit SdpStatus(FXComposite* parent)
      : FXLabel(parent, "", nullptr, LAYOUT_FILL_X | JUSTIFY_LEFT) {}
  FXint getDefaultWidth() override { return 10; }
};
} // namespace
SdpPanel::SdpPanel(FXComposite* parent)
    : FXVerticalFrame(parent, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0) {
  status = new SdpStatus(this);
  tabs = new FXTabBook(this, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  tabs->hide();
}
std::string SdpPanel::selectedTab() const {
  const auto index = tabs->getCurrent();
  return index >= 0 && std::size_t(index) < tabIds.size() ? tabIds[index] : "";
}
NavigationTree* SdpPanel::tree(const std::string& id) const {
  auto found = std::find(tabIds.begin(), tabIds.end(), id);
  return found == tabIds.end() ? nullptr : trees[found - tabIds.begin()];
}
void SdpPanel::buildTabs(const std::vector<std::string>& ids) {
  if (tabIds != ids) {
    for (auto* tree : trees)
      tree->cancelActivation();
    while (auto* child = tabs->getFirst())
      delete child;
    trees.clear();
    tabIds = ids;
    for (const auto& id : ids) {
      new FXTabItem(tabs, snapshot.nodes.at(id).label.c_str());
      auto* page = new FXVerticalFrame(tabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
      auto* tree = new SdpTree(page);
      trees.push_back(tree);
      tree->expanded = [this, tree](NavigationItem* item) { expand(tree, item); };
      tree->activated = [this, tree](const IndexAction& action) {
        auto found = snapshot.nodes.find(action.target);
        if (found == snapshot.nodes.end())
          return;
        const auto row = sdpDisplayRow(snapshot, action.target, {});
        if (!row)
          return;
        const auto& node = snapshot.nodes.at(row->node);
        if (node.kind == "viewpoint" && !node.children.empty()) {
          if (auto* item = dynamic_cast<SdpItem*>(tree->getCurrentItem());
              item && item->id == action.target)
            tree->expandTree(item);
          message("Overview unavailable from this producer — choose a detail below " + node.label);
          return;
        }
        if (activated)
          activated(row->target);
      };
      tree->contextRequested = [this](const auto& source, const auto& event) {
        context(source, event);
      };
    }
    if (tabs->id())
      tabs->create();
  }
  auto* child = tabs->getFirst();
  for (const auto& id : ids) {
    auto* item = static_cast<FXTabItem*>(child);
    const auto& node = snapshot.nodes.at(id);
    item->setText(node.label.c_str());
    item->setTipText(
        (node.label + " — " + node.state + (node.diagnostic.empty() ? "" : ": " + node.diagnostic))
            .c_str());
    child = child->getNext()->getNext();
  }
  if (ids.empty())
    tabs->hide();
  else
    tabs->show();
  tabs->recalc();
  recalc();
}
void SdpPanel::message(const std::string& text) {
  status->setText(text.substr(0, 100).c_str());
  status->setTipText(text.c_str());
}
void SdpPanel::clear() {
  ++generation;
  if (inContext) {
    deferredClear = true;
    deferred.reset();
    return;
  }
  buildTabs({});
  snapshot = {};
  rows = 0;
  truncated = false;
}
void SdpPanel::present(SdpReply value) {
  ++generation;
  if (inContext) {
    deferred = std::move(value);
    return;
  }
  const auto selectedRoot = selectedTab();
  std::map<std::string, std::set<std::string>> expanded;
  std::map<std::string, std::string> selected, selectedId;
  for (std::size_t i = 0; i < trees.size(); ++i) {
    const auto& id = tabIds[i];
    if (auto* item = dynamic_cast<SdpItem*>(trees[i]->getCurrentItem())) {
      selected[id] = item->key;
      selectedId[id] = item->id;
    }
    std::function<void(FXTreeItem*)> remember = [&](FXTreeItem* first) {
      for (auto* row = first; row; row = row->getNext()) {
        if (auto* item = dynamic_cast<SdpItem*>(row); item && item->isExpanded())
          expanded[id].insert(item->key);
        remember(row->getFirst());
      }
    };
    remember(trees[i]->getFirstItem());
    trees[i]->cancelActivation();
    trees[i]->clearItems();
  }
  rows = 0;
  truncated = false;
  snapshot = std::move(value);
  std::vector<std::string> roots;
  for (const auto& id : snapshot.roots)
    if (snapshot.nodes.at(id).kind == "tab")
      roots.push_back(id);
  buildTabs(roots);
  for (std::size_t i = 0; i < trees.size(); ++i) {
    const auto& root = snapshot.nodes.at(roots[i]);
    for (const auto& child : root.children) {
      if (rows >= 4096) {
        trees[i]->appendItem(nullptr,
                             new NavigationItem("Display limit — narrow the source model"));
        break;
      }
      add(trees[i], nullptr, child, {roots[i]});
    }
    if (!root.diagnostic.empty())
      trees[i]->appendItem(nullptr, new NavigationItem(root.diagnostic));
    if (root.children.empty())
      trees[i]->appendItem(nullptr, new NavigationItem("No entries — " + root.state));
    // Find one short route to the previous selection, including moved cards.
    std::map<std::string, std::string> parent;
    std::vector<std::string> queue{roots[i]};
    parent.emplace(roots[i], "");
    for (std::size_t q = 0; q < queue.size() && !parent.count(selectedId[roots[i]]); ++q) {
      const auto& node = snapshot.nodes.at(queue[q]);
      auto children = sdpDisplayChildren(snapshot, node);
      if (!node.reference.empty())
        children.push_back(node.reference);
      for (const auto& child : children)
        if (parent.emplace(child, node.id).second)
          queue.push_back(child);
    }
    if (parent.count(selectedId[roots[i]]))
      for (auto id = parent.at(selectedId[roots[i]]); !id.empty(); id = parent.at(id))
        expanded[roots[i]].insert(id);
    FXTreeItem* exact = nullptr;
    FXTreeItem* fallback = nullptr;
    std::function<void(FXTreeItem*)> restore = [&](FXTreeItem* first) {
      for (auto* row = first; row; row = row->getNext()) {
        if (auto* item = dynamic_cast<SdpItem*>(row)) {
          if (expanded[roots[i]].count(item->key) || expanded[roots[i]].count(item->id))
            trees[i]->expandTree(item);
          if (item->key == selected[roots[i]])
            exact = item;
          if (!fallback && item->id == selectedId[roots[i]])
            fallback = item;
        }
        restore(row->getFirst());
      }
    };
    restore(trees[i]->getFirstItem());
    if (auto* chosen = exact ? exact : fallback) {
      trees[i]->setCurrentItem(chosen);
      trees[i]->selectItem(chosen);
      trees[i]->makeItemVisible(chosen);
    }
  }
  auto chosen = std::find(roots.begin(), roots.end(), selectedRoot);
  if (chosen == roots.end()) {
    chosen = std::find_if(roots.begin(), roots.end(), [this](const auto& id) {
      const auto& state = snapshot.nodes.at(id).state;
      return state == "available" || state == "validated";
    });
  }
  if (!roots.empty())
    tabs->setCurrent(chosen == roots.end() ? 0 : int(chosen - roots.begin()));
  message(snapshot.project + (roots.empty() ? " — No SDP navigation tabs returned"
                                            : " — Navigation updated; views are snapshots"));
  if (truncated)
    message("Navigation partially expanded — depth limit; select a detail or source");
}
void SdpPanel::add(NavigationTree* tree, NavigationItem* parent, const std::string& id,
                   std::vector<std::string> ancestors, const std::string& inherited) {
  if (rows >= 4096)
    return;
  auto row = sdpDisplayRow(snapshot, id, ancestors, inherited);
  if (!row) {
    // A pure cycle has no lost content. A depth cutoff can hide real descendants.
    if (ancestors.size() >= snapshot.depthLimit) {
      truncated = true;
      message("Navigation partially expanded — depth limit; select a detail or source");
    }
    return;
  }
  if (row->truncated) {
    truncated = true;
    message("Navigation partially expanded — depth limit; select a detail or source");
  }
  ++rows;
  const auto& node = snapshot.nodes.at(row->node);
  IndexAction action;
  if (sdpDisplayAction(row->target)) {
    action.kind = IndexActionKind::Command;
    action.target = row->id;
  }
  auto* item = new SdpItem(row->label, row->id, row->source, std::move(row->ancestors),
                           std::move(action));
  item->node = row->node;
  item->diagnostic = row->diagnostic;
  const auto* displayParent = dynamic_cast<SdpItem*>(parent);
  item->key = (displayParent ? displayParent->key : std::string{}) +
              std::to_string(row->id.size()) + ":" + row->id;
  tree->appendItem(parent, item);
  item->populated = row->terminal;
  if (row->terminal && !row->diagnostic.empty())
    item->setText((row->label + " — " + row->diagnostic).c_str());
  item->setHasItems(!row->terminal && (!node.children.empty() || !row->diagnostic.empty()));
}

void SdpPanel::expand(NavigationTree* tree, NavigationItem* value) {
  auto* item = dynamic_cast<SdpItem*>(value);
  if (!item || item->populated)
    return;
  item->populated = true;
  const auto& node = snapshot.nodes.at(item->node);
  if (!item->diagnostic.empty())
    tree->appendItem(item, new NavigationItem(item->diagnostic));
  for (const auto& child : sdpDisplayChildren(snapshot, node)) {
    if (rows >= 4096) {
      tree->appendItem(item, new NavigationItem("Display limit — Refresh to reset"));
      break;
    }
    add(tree, item, child, item->ancestors, item->source);
  }
}
} // namespace xfmd
