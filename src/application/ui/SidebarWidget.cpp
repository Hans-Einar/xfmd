#include "SidebarWidget.h"
#include "TreeActivation.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include "application/workspace/WorkPathHistory.h"
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
namespace {
struct PathItem : FXTreeItem {
  std::filesystem::path path;
  bool directory;
  FXint getHeight(const FXTreeList* list) const override {
    return FXTreeItem::getHeight(list) + list->getFont()->getFontHeight() / 3;
  }
  PathItem(const TreeEntry& entry, const std::string& label)
      : FXTreeItem(label.c_str()), path(entry.path), directory(entry.directory) {
    setHasItems(directory);
    setDraggable(false);
  }
};
FXint order(const FXTreeItem* left, const FXTreeItem* right) {
  const auto* a = static_cast<const PathItem*>(left);
  const auto* b = static_cast<const PathItem*>(right);
  if (a->directory != b->directory)
    return a->directory ? -1 : 1;
  return comparecase(a->getText(), b->getText());
}
} // namespace
FXDEFMAP(SidebarWidget)
sidebarMap[] = {
    FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, SidebarWidget::onRelease),
    FXMAPFUNC(SEL_KEYPRESS, 0, SidebarWidget::onKey),
    FXMAPFUNC(SEL_CLICKED, SidebarWidget::ID_TREE_EVENT, SidebarWidget::onOpen),
    FXMAPFUNC(SEL_TIMEOUT, SidebarWidget::ID_ACTIVATE, SidebarWidget::onActivate),
    FXMAPFUNC(SEL_DOUBLECLICKED, SidebarWidget::ID_TREE_EVENT, SidebarWidget::onOpen),
    FXMAPFUNC(SEL_TIMEOUT, SidebarWidget::ID_POLL, SidebarWidget::onPoll),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, SidebarWidget::onContext),
    FXMAPFUNC(SEL_COMMAND, SidebarWidget::ID_SET_WORK_PATH, SidebarWidget::onSetWorkPath)};
FXIMPLEMENT(SidebarWidget, FXTreeList, sidebarMap, ARRAYNUMBER(sidebarMap))
SidebarWidget::SidebarWidget(FXComposite* parent)
    : FXTreeList(parent, this, ID_TREE_EVENT,
                 TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
  setSortFunc(order);
}
SidebarWidget::~SidebarWidget() {
  getApp()->removeTimeout(this, ID_POLL);
  getApp()->removeTimeout(this, ID_ACTIVATE);
  pendingOpen.clear();
  scanner.stop();
}
void SidebarWidget::create() {
  FXTreeList::create();
  getApp()->addTimeout(this, ID_POLL, 30);
}
void SidebarWidget::setRoot(const std::filesystem::path& path, const std::string& label) {
  getApp()->removeTimeout(this, ID_ACTIVATE);
  pendingOpen.clear();
  scanner.stop();
  restoring = false;
  restoreExpanded.clear();
  restoreSelected.clear();
  root = path;
  rootLabel = label;
  clearItems();
  items.clear();
  requested.clear();
  files = 0;
  auto* item = appendItem(nullptr, new PathItem({root, true}, rootLabel));
  items[root.string()] = item;
  requested.insert(root.string());
  FXTreeList::expandTree(item);
  setCurrentItem(item);
  setPosition(0, 0);
  scanning = true;
  scanner.start(root, filter);
}
void SidebarWidget::refresh() {
  std::set<std::string> expanded;
  for (const auto& entry : items)
    if (entry.second->isExpanded())
      expanded.insert(entry.first);
  const std::string selected = getItemPathname(getCurrentItem()).text();
  const int x = getXPosition(), y = getYPosition();
  setRoot(root, rootLabel);
  restoreExpanded = std::move(expanded);
  restoreSelected = selected;
  restoreX = x;
  restoreY = y;
  restoring = true;
}
void SidebarWidget::setFilter(FileNameFilter value) {
  filter = std::move(value);
  if (!root.empty())
    setRoot(root, rootLabel);
}
FXTreeItem* SidebarWidget::add(const TreeEntry& entry) {
  auto found = items.find(entry.path.string());
  if (found != items.end())
    return found->second;
  if (!WorkPathHistory::contains(root, entry.path))
    return nullptr;
  auto* parent = add({entry.path.parent_path(), true});
  if (!parent)
    return nullptr;
  auto* item = appendItem(parent, new PathItem(entry, entry.path.filename().string()));
  items[entry.path.string()] = item;
  if (!entry.directory)
    ++files;
  return item;
}
FXbool SidebarWidget::expandTree(FXTreeItem* item, FXbool notify) {
  if (item && isItemDirectory(item) && !filter.active()) {
    const auto path = getItemPathname(item);
    if (requested.insert(path.text()).second) {
      scanner.request(path.text());
      scanning = true;
    }
  }
  return FXTreeList::expandTree(item, notify);
}
FXTreeItem* SidebarWidget::getPathnameItem(const FXString& path) const {
  auto found = items.find(path.text());
  return found == items.end() ? nullptr : found->second;
}
FXString SidebarWidget::getItemPathname(const FXTreeItem* item) const {
  return item ? static_cast<const PathItem*>(item)->path.c_str() : "";
}
bool SidebarWidget::isItemDirectory(const FXTreeItem* item) const {
  return item && static_cast<const PathItem*>(item)->directory;
}
long SidebarWidget::onOpen(FXObject*, FXSelector selector, void* data) {
  auto* item = static_cast<FXTreeItem*>(data);
  if (!item)
    item = getCurrentItem();
  if (!item)
    return 1;
  const bool twice = FXSELTYPE(selector) == SEL_DOUBLECLICKED;
  if (!pointerClick)
    return 1;
  if (item == getFirstItem() && twice) {
    if (broadenRoot)
      broadenRoot();
  } else if (isItemDirectory(item) && twice) {
    if (item->isExpanded())
      collapseTree(item, true);
    else
      expandTree(item, true);
  } else if (isItemFile(item) && open && !twice) {
    std::error_code ec;
    auto target = std::filesystem::canonical(getItemPathname(item).text(), ec);
    if (!ec && WorkPathHistory::contains(root, target)) {
      pendingOpen = target.string();
      pendingSystemDefault = pointerSystemDefault;
      getApp()->addTimeout(this, ID_ACTIVATE, 0);
    } else if (status)
      status("File unavailable or outside work path.");
  }
  return 1;
}
long SidebarWidget::onRelease(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  pointerSystemDefault = bool(event->state & CONTROLMASK);
  pointerClick = !event->moved && !(event->state & (SHIFTMASK | ALTMASK)) &&
                 getItemAt(event->win_x, event->win_y) == getCurrentItem();
  auto result = FXTreeList::onLeftBtnRelease(sender, sel, data);
  pointerClick = false;
  pointerSystemDefault = false;
  return result;
}
long SidebarWidget::onKey(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (activatesTreeItem(*event, isItemFile(getCurrentItem()))) {
    pointerSystemDefault = false;
    pointerClick = true;
    auto* item = getCurrentItem();
    if (item && isItemDirectory(item)) {
      if (item->isExpanded())
        collapseTree(item, true);
      else
        expandTree(item, true);
    } else
      onOpen(this, FXSEL(SEL_CLICKED, ID_TREE_EVENT), item);
    pointerClick = false;
    return 1;
  }
  return FXTreeList::onKeyPress(sender, sel, data);
}
long SidebarWidget::onActivate(FXObject*, FXSelector, void*) {
  auto path = std::move(pendingOpen);
  pendingOpen.clear();
  if (!path.empty() && open)
    open(path, pendingSystemDefault);
  return 1;
}
long SidebarWidget::onPoll(FXObject*, FXSelector, void*) {
  auto batch = scanner.take();
  for (const auto& entry : batch.entries)
    add(entry);
  if (!batch.entries.empty())
    sortItems();
  const auto requestsBeforeRestore = requested.size();
  if (restoring) {
    for (auto it = restoreExpanded.begin(); it != restoreExpanded.end();) {
      auto found = items.find(*it);
      if (found != items.end()) {
        expandTree(found->second);
        it = restoreExpanded.erase(it);
      } else
        ++it;
    }
    if (auto* item = getPathnameItem(restoreSelected.c_str()))
      setCurrentItem(item);
    if (!batch.busy && requestsBeforeRestore == requested.size()) {
      // Recompute scroll ranges after asynchronous inserts/expansion before
      // restoring offsets; the previous empty tree would clamp them to zero.
      layout();
      setPosition(restoreX, restoreY);
      restoring = false;
      restoreExpanded.clear();
    }
  }
  scanning = batch.busy || requestsBeforeRestore != requested.size();
  if (status) {
    std::string text = scanning ? "Searching… " : (files ? "" : "No matching files. ");
    text += std::to_string(files) + " files";
    if (batch.errors)
      text += "; unreadable: " + std::to_string(batch.errors);
    status(text);
  }
  getApp()->addTimeout(this, ID_POLL, 30);
  return 1;
}
long SidebarWidget::onContext(FXObject*, FXSelector, void* data) {
  // Let FOX release its pointer grab before entering the popup's modal loop.
  FXTreeList::onRightBtnRelease(this, 0, data);
  auto* event = static_cast<FXEvent*>(data);
  auto* item = getItemAt(event->win_x, event->win_y);
  if (!isItemDirectory(item))
    return 1;
  contextPath = getItemPathname(item).text();
  FXMenuPane menu(this);
  new FXMenuCommand(&menu, "Set work path", nullptr, this, ID_SET_WORK_PATH);
  menu.create();
  menu.popup(nullptr, event->root_x, event->root_y);
  getApp()->runModalWhileShown(&menu);
  return 1;
}
long SidebarWidget::onSetWorkPath(FXObject*, FXSelector, void*) {
  if (workPathRequested && !contextPath.empty())
    workPathRequested(contextPath);
  return 1;
}
} // namespace xfmd
