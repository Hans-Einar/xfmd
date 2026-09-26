#include "PathContextMenu.h"
using namespace FX;
namespace xfmd {
namespace {
class MenuChoice : public FXObject {
public:
  PathMenuAction action = PathMenuAction::None;
  long handle(FXObject*, FXSelector selector, void*) override {
    if (FXSELTYPE(selector) != SEL_COMMAND)
      return 0;
    action = static_cast<PathMenuAction>(FXSELID(selector));
    return 1;
  }
};
} // namespace
PathMenuAction showPathContextMenu(FXWindow* owner, const FXEvent& event,
                                   PathMenuAction favoriteAction, bool offerWorkPath) {
  MenuChoice choice;
  // A popup belongs to the top-level shell so native keyboard focus reaches it.
  FXMenuPane menu(owner->getShell());
  if (offerWorkPath)
    new FXMenuCommand(&menu, "Set work path", nullptr, &choice,
                      static_cast<FXSelector>(PathMenuAction::SetWorkPath));
  const char* label = favoriteAction == PathMenuAction::RemoveFavorite ? "Remove from Favorites"
                                                                       : "Add to Favorites";
  new FXMenuCommand(&menu, label, nullptr, &choice, static_cast<FXSelector>(favoriteAction));
  menu.create();
  menu.popup(nullptr, event.root_x, event.root_y);
  // Context menus must receive Escape/arrow keys even while the document
  // retains X input focus. Release the grab before invoking any path action.
  menu.grabKeyboard();
  owner->getApp()->runModalWhileShown(&menu);
  if (menu.grabbedKeyboard())
    menu.ungrabKeyboard();
  return choice.action;
}
} // namespace xfmd
