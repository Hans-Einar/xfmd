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
