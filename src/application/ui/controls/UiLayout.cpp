#include "UiLayout.h"
using namespace FX;
namespace xfmd {
UiRow::UiRow(FXComposite* p, UiContext& context, FXuint flags)
    : FXHorizontalFrame(p, LAYOUT_FILL_X | PACK_UNIFORM_HEIGHT | flags), ui(context) {
  restyle();
}
void UiRow::restyle() {
  auto m = ui.metrics();
  setPadLeft(m.inset);
  setPadRight(m.inset);
  setPadTop(m.gap / 2);
  setPadBottom(m.gap / 2);
  setHSpacing(m.gap);
}
UiForm::UiForm(FXComposite* p, UiContext& context)
    : FXMatrix(p, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X), ui(context) {
  restyle();
}
void UiForm::restyle() {
  auto m = ui.metrics();
  setHSpacing(m.inset);
  setVSpacing(m.gap);
  setPadLeft(m.inset);
  setPadRight(m.inset);
  setPadTop(m.gap);
  setPadBottom(m.gap);
}
DialogActions::DialogActions(FXComposite* p, UiContext& context, FXObject* target,
                             FXSelector accept, FXSelector cancel)
    : UiRow(p, context) {
  new FXFrame(this, LAYOUT_FILL_X);
  UiFactory factory(context);
  factory.button(this, "&Cancel", target, cancel, UiIcon::NoIcon, ButtonRole::Normal);
  factory.button(this, "&OK", target, accept, UiIcon::NoIcon, ButtonRole::Primary,
                 BUTTON_INITIAL | BUTTON_DEFAULT);
}
PanelHeader::PanelHeader(FXComposite* p, UiContext& context, const char* text)
    : FXLabel(p, text, nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X), ui(context) {
  restyle();
}
void PanelHeader::restyle() {
  auto m = ui.metrics();
  setPadLeft(m.inset);
  setPadRight(m.inset);
  setPadTop(m.gap);
  setPadBottom(m.gap);
}
UiButton* UiFactory::button(FXComposite* p, const FXString& text, FXObject* target, FXSelector id,
                            UiIcon icon, ButtonRole role, FXuint flags) {
  if (buttonClass)
    return buttonClass(p, ui, text, target, id, icon, role, flags);
  return new UiButton(p, ui, text, target, id, icon, role, flags);
}
} // namespace xfmd
