#include "UiButton.h"
#include <algorithm>
using namespace FX;
namespace xfmd {
FXDEFMAP(UiButton)
buttonMap[] = {FXMAPFUNC(SEL_PAINT, 0, UiButton::onPaint),
               FXMAPFUNC(SEL_COMMAND, FXWindow::ID_CHECK, UiButton::onChecked),
               FXMAPFUNC(SEL_COMMAND, FXWindow::ID_UNCHECK, UiButton::onChecked)};
FXIMPLEMENT(UiButton, FXButton, buttonMap, ARRAYNUMBER(buttonMap))
UiButton::UiButton(FXComposite* parent, UiContext& context, const FXString& text, FXObject* target,
                   FXSelector selector, UiIcon icon, ButtonRole r, FXuint flags)
    : FXButton(parent, text, context.icons.get(icon), target, selector,
               BUTTON_TOOLBAR | LAYOUT_CENTER_Y | flags),
      ui(&context), glyph(icon), role(r) {}
long UiButton::onChecked(FXObject*, FXSelector selector, void*) {
  bool value = FXSELID(selector) == FXWindow::ID_CHECK;
  if (value != checked) {
    checked = value;
    update();
  }
  return 1;
}
void UiButton::create() {
  FXButton::create();
  if (auto* disabled = ui->icons.get(glyph, false))
    disabled->create();
}
FXint UiButton::getDefaultWidth() {
  const auto m = ui->metrics();
  const int width = getFont()->getTextWidth(getText());
  return std::max(getDefaultHeight(),
                  width + (glyph == UiIcon::NoIcon ? 0 : m.iconSize + (width ? m.gap : 0)) +
                      m.inset * 2);
}
FXint UiButton::getDefaultHeight() {
  auto m = ui->metrics();
  return std::max({m.height, getFont()->getFontHeight() + 10, m.iconSize + 8});
}
long UiButton::onPaint(FXObject*, FXSelector, void* ptr) {
  FXDCWindow dc(this, static_cast<FXEvent*>(ptr));
  const auto& p = ui->palette();
  auto m = ui->metrics();
  const int w = getWidth(), h = getHeight();
  dc.setForeground(p.surface);
  dc.fillRectangle(0, 0, w, h);
  ButtonVisualState state{bool(isEnabled()), underCursor(), getState() == STATE_DOWN,
                          checked,           hasFocus(),    isDefault()};
  ui->painter().paint(dc, p, m, w, h, role, state);
  dc.setForeground(isEnabled() ? p.text : p.muted);
  dc.setFont(getFont());
  int textWidth = getFont()->getTextWidth(getText());
  auto* image = ui->icons.get(glyph, isEnabled());
  int total = textWidth + (image ? image->getWidth() + (textWidth ? m.gap : 0) : 0),
      x = (w - total) / 2;
  if (image) {
    dc.drawIcon(image, x, (h - image->getHeight()) / 2);
    x += image->getWidth() + (textWidth ? m.gap : 0);
  }
  int baseline = (h - getFont()->getFontHeight()) / 2 + getFont()->getFontAscent();
  dc.drawText(x, baseline, getText());
  if (hotkey && hotoff < getText().length()) {
    int start = getFont()->getTextWidth(getText().text(), hotoff);
    int length = getFont()->getTextWidth(getText().text() + hotoff, 1);
    dc.drawLine(x + start, baseline + 1, x + start + length, baseline + 1);
  }
  if (state.focused) {
    dc.setForeground(p.accent);
    dc.drawFocusRectangle(4, 4, w - 8, h - 8);
  }
  return 1;
}
} // namespace xfmd
