#include "ThemeButton.h"
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(ThemeButton)
themeMap[] = {FXMAPFUNC(SEL_RIGHTBUTTONPRESS, 0, ThemeButton::onRight),
              FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, ThemeButton::onRight),
              FXMAPFUNC(SEL_KEYPRESS, 0, ThemeButton::onKey)};
FXIMPLEMENT(ThemeButton, UiButton, themeMap, ARRAYNUMBER(themeMap))
ThemeButton::ThemeButton(FXComposite* parent, UiContext& ui, FXObject* target, FXSelector selector,
                         ReadingColorPopup* popup)
    : UiButton(parent, ui, "\tToggle Light / Dark; right-click or Shift+F10 for colors", target,
               selector, UiIcon::Theme, ButtonRole::Toolbar, LAYOUT_RIGHT),
      colors(popup) {}
long ThemeButton::onRight(FXObject*, FXSelector sel, void*) {
  if (FXSELTYPE(sel) == SEL_RIGHTBUTTONRELEASE)
    colors->showAt(this);
  return 1;
}
long ThemeButton::onKey(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (event->code == KEY_Menu || (event->code == KEY_F10 && (event->state & SHIFTMASK))) {
    colors->showAt(this);
    return 1;
  }
  return UiButton::onKeyPress(sender, sel, data);
}
} // namespace xfmd
