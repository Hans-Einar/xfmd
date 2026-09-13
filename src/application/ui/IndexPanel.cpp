#include "IndexPanel.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
FXDEFMAP(IndexPanel)
indexPanelMap[] = {FXMAPFUNC(SEL_TIMEOUT, IndexPanel::ID_REQUEST, IndexPanel::onRequest)};
FXIMPLEMENT(IndexPanel, FXVerticalFrame, indexPanelMap, ARRAYNUMBER(indexPanelMap))
namespace {
NavigationItem* add(NavigationTree* tree, FXTreeItem* parent, const std::string& label,
                    IndexAction action = {}) {
  auto* item = new NavigationItem(label, std::move(action));
  tree->appendItem(parent, item);
  return item;
}
void clearChildren(NavigationTree* tree, FXTreeItem* parent) {
  while (parent->getFirst())
    tree->removeItem(parent->getFirst());
}
} // namespace
IndexPanel::IndexPanel(FXComposite* parent)
    : FXVerticalFrame(parent, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0) {
  auto* split =
      new FXSplitter(this, SPLITTER_VERTICAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* upper = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 300, 0, 0, 0, 0);
  new FXLabel(upper, "Document index", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
  outline = new NavigationTree(upper);
  auto* lower = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 280, 0, 0, 0, 0);
  references = new NavigationTree(lower);
  auto activate = [this](const IndexAction& action) {
    if (valid && activated)
      activated(action);
  };
  outline->activated = activate;
  references->activated = activate;
  references->expanded = [this](NavigationItem* item) { request(item); };
}
IndexPanel::~IndexPanel() { getApp()->removeTimeout(this, ID_REQUEST); }
void IndexPanel::invalidate(DocumentToken expected, const std::string& path) {
  if (token == expected && documentPath == path)
    return; // A layout/zoom change does not invalidate semantic source anchors.
  valid = false;
  getApp()->removeTimeout(this, ID_REQUEST);
  requests.clear();
  outline->cancelActivation();
  references->cancelActivation();
  outline->disable();
  references->disable();
}
void IndexPanel::present(const DocumentIndex& index, const std::string& path) {
  outline->cancelActivation();
  references->cancelActivation();
  getApp()->removeTimeout(this, ID_REQUEST);
  requests.clear();
  outline->clearItems();
  references->clearItems();
  files.clear();
  loading.clear();
  token = index.token;
  documentPath = path;
  valid = true;
  outline->enable();
  references->enable();
  std::vector<std::pair<int, NavigationItem*>> parents;
  for (const auto& heading : index.headings) {
    while (!parents.empty() && parents.back().first >= heading.level)
      parents.pop_back();
    IndexAction action;
    action.kind = IndexActionKind::Heading;
    action.path = path;
    action.anchor = heading.anchor;
    auto* item = add(outline, parents.empty() ? nullptr : parents.back().second, heading.title,
                     std::move(action));
    outline->expandTree(item);
    parents.push_back({heading.level, item});
  }
  if (index.headings.empty())
    add(outline, nullptr, "No headings");
  auto* root = add(references, nullptr, "References");
  auto* markdown = add(references, root, "Markdown");
  auto* hyperlinks = add(references, root, "Hyperlinks");
  for (const auto& link : index.markdown) {
    IndexAction action;
    action.kind = IndexActionKind::File;
    action.path = link.path;
    action.error = link.error;
    auto* item = add(references, markdown, link.target, std::move(action));
    add(references, item, link.error.empty() ? "Expand to load headings" : link.error);
    if (!link.path.empty())
      files[link.path] = item;
  }
  for (const auto& link : index.hyperlinks) {
    IndexAction action;
    action.kind = IndexActionKind::Hyperlink;
    action.target = link.target;
    action.anchor = link.anchor;
    add(references, hyperlinks,
        link.label == link.target ? link.target : link.label + " — " + link.target,
        std::move(action));
  }
  references->expandTree(root);
  references->expandTree(markdown);
  references->expandTree(hyperlinks);
}
void IndexPanel::request(NavigationItem* item) {
  if (!valid || item->action.kind != IndexActionKind::File || item->action.path.empty() ||
      !loading.insert(item->action.path).second)
    return;
  requests.insert(item->action.path);
  getApp()->addTimeout(this, ID_REQUEST, 0);
}
long IndexPanel::onRequest(FXObject*, FXSelector, void*) {
  auto paths = std::move(requests);
  requests.clear();
  for (const auto& path : paths) {
    auto found = files.find(path);
    if (!valid || found == files.end())
      continue;
    clearChildren(references, found->second);
    add(references, found->second, "Loading…");
    if (referenceRequested)
      referenceRequested(path);
  }
  return 1;
}
void IndexPanel::presentReference(const ReferenceResult& result) {
  auto found = files.find(result.path);
  if (!valid || found == files.end())
    return;
  loading.erase(result.path);
  auto* parent = found->second;
  clearChildren(references, parent);
  std::map<std::string, std::size_t> occurrences;
  for (const auto& heading : result.headings) {
    IndexAction action;
    action.kind = IndexActionKind::Heading;
    action.path = result.path;
    action.anchor = heading.anchor;
    action.heading = heading.title;
    action.occurrence = occurrences[heading.title]++;
    add(references, parent, heading.title, std::move(action));
  }
  if (!result.error.empty() || result.headings.empty())
    add(references, parent, result.error.empty() ? "No headings" : result.error);
}
} // namespace xfmd
