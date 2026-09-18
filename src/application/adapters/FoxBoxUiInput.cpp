#include "FoxBoxUiInput.h"
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxBoxUiField)
fieldMap[] = {FXMAPFUNC(SEL_KEYPRESS, 0, FoxBoxUiField::onKeyPress),
              FXMAPFUNC(SEL_KEYRELEASE, 0, FoxBoxUiField::onKeyRelease)};
FXIMPLEMENT(FoxBoxUiField, FXTextField, fieldMap, ARRAYNUMBER(fieldMap))
FoxBoxUiField::FoxBoxUiField(FXComposite* p, FXObject* target, FXSelector id)
    : FXTextField(p, 20, target, id, TEXTFIELD_ENTER_ONLY | FRAME_SUNKEN | FRAME_THICK) {}
long FoxBoxUiField::onKeyPress(FXObject* s, FXSelector sel, void* data) {
  auto* e = static_cast<FXEvent*>(data);
  if (e->code == KEY_Return || e->code == KEY_KP_Enter) {
    enterHeld = true;
    return 1;
  }
  if (e->code == KEY_Escape) {
    enterHeld = false;
    if (restore)
      restore();
    return 1;
  }
  return FXTextField::onKeyPress(s, sel, data);
}
long FoxBoxUiField::onKeyRelease(FXObject* s, FXSelector sel, void* data) {
  auto* e = static_cast<FXEvent*>(data);
  if (e->code == KEY_Return || e->code == KEY_KP_Enter) {
    bool fire = enterHeld;
    enterHeld = false;
    if (fire && isEnabled() && commit)
      commit();
    return 1;
  }
  return FXTextField::onKeyRelease(s, sel, data);
}
FXDEFMAP(FoxBoxUiButton)
buttonMap[] = {FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, FoxBoxUiButton::onPress),
               FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FoxBoxUiButton::onRelease),
               FXMAPFUNC(SEL_KEYPRESS, 0, FoxBoxUiButton::onKeyPress),
               FXMAPFUNC(SEL_KEYRELEASE, 0, FoxBoxUiButton::onKeyRelease)};
FXIMPLEMENT(FoxBoxUiButton, FXButton, buttonMap, ARRAYNUMBER(buttonMap))
FoxBoxUiButton::FoxBoxUiButton(FXComposite* p, const char* label, FXObject* target, FXSelector id)
    : FXButton(p, label, nullptr, target, id, BUTTON_NORMAL) {}
long FoxBoxUiButton::onPress(FXObject* s, FXSelector sel, void* data) {
  armed = publication;
  held = true;
  return FXButton::onLeftBtnPress(s, sel, data);
}
long FoxBoxUiButton::onRelease(FXObject* s, FXSelector sel, void* data) {
  bool valid = held && armed == publication;
  held = false;
  auto* saved = getTarget();
  if (!valid)
    setTarget(nullptr);
  auto result = FXButton::onLeftBtnRelease(s, sel, data);
  setTarget(saved);
  return result;
}
long FoxBoxUiButton::onKeyPress(FXObject* s, FXSelector sel, void* data) {
  auto* e = static_cast<FXEvent*>(data);
  if (e->code == KEY_space || e->code == KEY_Return || e->code == KEY_KP_Enter) {
    if (!held) {
      armed = publication;
      held = true;
    }
    return 1;
  }
  return FXButton::onKeyPress(s, sel, data);
}
long FoxBoxUiButton::onKeyRelease(FXObject* s, FXSelector sel, void* data) {
  auto* e = static_cast<FXEvent*>(data);
  if (e->code == KEY_space || e->code == KEY_Return || e->code == KEY_KP_Enter) {
    bool fire = held && armed == publication && isEnabled();
    held = false;
    if (fire && getTarget())
      getTarget()->handle(this, FXSEL(SEL_COMMAND, getSelector()), nullptr);
    return 1;
  }
  return FXButton::onKeyRelease(s, sel, data);
}
} // namespace xfmd
