#include "NavigationTree.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(NavigationTree)
navigationTreeMap[] = {
    FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, NavigationTree::onRelease),
    FXMAPFUNC(SEL_KEYPRESS, 0, NavigationTree::onKey),
    FXMAPFUNC(SEL_CLICKED, NavigationTree::ID_EVENT, NavigationTree::onClick),
    FXMAPFUNC(SEL_DOUBLECLICKED, NavigationTree::ID_EVENT, NavigationTree::onDouble),
    FXMAPFUNC(SEL_TIMEOUT, NavigationTree::ID_ACTIVATE, NavigationTree::onActivate)};
FXIMPLEMENT(NavigationTree, FXTreeList, navigationTreeMap, ARRAYNUMBER(navigationTreeMap))
NavigationTree::NavigationTree(FXComposite* parent)
    : FXTreeList(parent, this, ID_EVENT,
                 TREELIST_BROWSESELECT | TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES |
                     TREELIST_ROOT_BOXES | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
}
NavigationTree::~NavigationTree() { cancelActivation(); }
void NavigationTree::cancelActivation() {
  pending.reset();
  getApp()->removeTimeout(this, ID_ACTIVATE);
}
FXbool NavigationTree::expandTree(FXTreeItem* item, FXbool notify) {
  const bool wasExpanded = item && item->isExpanded();
  auto result = FXTreeList::expandTree(item, notify);
  if (item && !wasExpanded && expanded)
    expanded(static_cast<NavigationItem*>(item));
  return result;
}
long NavigationTree::onRelease(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  pointerClick = !event->moved && !(event->state & (CONTROLMASK | SHIFTMASK | ALTMASK)) &&
                 getItemAt(event->win_x, event->win_y) == getCurrentItem();
  auto result = FXTreeList::onLeftBtnRelease(sender, sel, data);
  pointerClick = false;
  return result;
}
long NavigationTree::onKey(FXObject* sender, FXSelector sel, void* data) {
  const auto* event = static_cast<FXEvent*>(data);
  if (event->code == KEY_Return || event->code == KEY_KP_Enter) {
    pointerClick = true;
    onClick(this, 0, getCurrentItem());
    pointerClick = false;
    return 1;
  }
  return FXTreeList::onKeyPress(sender, sel, data);
}
long NavigationTree::onClick(FXObject*, FXSelector, void* data) {
  auto* item = static_cast<NavigationItem*>(data);
  if (!pointerClick || !item || !item->isEnabled())
    return 1;
  if (item->action.kind == IndexActionKind::None) {
    if (item->isExpanded())
      collapseTree(item, true);
    else
      expandTree(item, true);
  } else {
    pending = item->action; // Never retain an item pointer across FOX dispatch.
    getApp()->addTimeout(this, ID_ACTIVATE, 0);
  }
  return 1;
}
long NavigationTree::onDouble(FXObject*, FXSelector, void*) { return 1; }
long NavigationTree::onActivate(FXObject*, FXSelector, void*) {
  auto action = std::move(pending);
  pending.reset();
  if (action && activated)
    activated(*action);
  return 1;
}
} // namespace xfmd
