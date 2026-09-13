#include "UiButton.h"

void drawGlyph(FXDCWindow& dc, Glyph glyph, int x, int y) {
  dc.setLineWidth(2);
  switch (glyph) {
  case Glyph::Open:
    dc.drawLine(x + 1, y + 5, x + 7, y + 5);
    dc.drawLine(x + 7, y + 5, x + 9, y + 8);
    dc.drawLine(x + 9, y + 8, x + 17, y + 8);
    dc.drawLine(x + 1, y + 5, x + 1, y + 17);
    dc.drawLine(x + 1, y + 17, x + 15, y + 17);
    dc.drawLine(x + 15, y + 17, x + 18, y + 10);
    dc.drawLine(x + 18, y + 10, x + 5, y + 10);
    dc.drawLine(x + 5, y + 10, x + 1, y + 17);
    break;
  case Glyph::Save:
    dc.drawRectangle(x + 2, y + 2, 14, 16);
    dc.drawRectangle(x + 5, y + 2, 8, 6);
    dc.drawRectangle(x + 5, y + 12, 8, 6);
    break;
  case Glyph::Back:
  case Glyph::Forward: {
    int a = glyph == Glyph::Back ? 3 : 16, b = glyph == Glyph::Back ? 9 : 10;
    dc.drawLine(x + a, y + 10, x + b, y + 4);
    dc.drawLine(x + a, y + 10, x + b, y + 16);
    dc.drawLine(x + 3, y + 10, x + 16, y + 10);
    break;
  }
  case Glyph::Sidebar:
    dc.drawRectangle(x + 1, y + 3, 17, 14);
    dc.drawLine(x + 7, y + 3, x + 7, y + 17);
    break;
  case Glyph::Refresh:
    dc.drawArc(x + 3, y + 3, 13, 13, 45 * 64, 290 * 64);
    dc.drawLine(x + 15, y + 2, x + 15, y + 7);
    dc.drawLine(x + 15, y + 7, x + 10, y + 7);
    break;
  case Glyph::Search:
    dc.drawArc(x + 2, y + 2, 11, 11, 0, 360 * 64);
    dc.drawLine(x + 12, y + 12, x + 18, y + 18);
    break;
  default:
    break;
  }
  dc.setLineWidth(1);
}

FXDEFMAP(UiButton) buttonMap[] = {FXMAPFUNC(SEL_PAINT, 0, UiButton::onPaint)};
FXIMPLEMENT(UiButton, FXButton, buttonMap, ARRAYNUMBER(buttonMap))
UiButton::UiButton(FXComposite* parent, const Style& s, const char* text, Glyph g, bool checked,
                   int example)
    : FXButton(parent, text, nullptr, nullptr, 0, BUTTON_TOOLBAR | LAYOUT_CENTER_Y), style(&s),
      glyph(g), selected(checked), specimen(example) {
  if (s.classic) {
    setButtonStyle(0);
    setFrameStyle(FRAME_RAISED | FRAME_THICK);
    if (selected)
      setState(STATE_ENGAGED);
    setBackColor(selected ? s.selected : s.surface);
  }
}
FXint UiButton::getDefaultWidth() {
  return std::max(style->height, getFont()->getTextWidth(getText()) +
                                     (glyph == Glyph::NoIcon ? 0 : 24) + style->inset * 2);
}
FXint UiButton::getDefaultHeight() { return style->height; }
long UiButton::onPaint(FXObject* sender, FXSelector sel, void* ptr) {
  const Style& s = *style;
  if (s.classic) {
    FXButton::onPaint(sender, sel, ptr);
    if (glyph != Glyph::NoIcon) {
      FXDCWindow dc(this, static_cast<FXEvent*>(ptr));
      dc.setForeground(isEnabled() ? s.text : s.muted);
      drawGlyph(dc, glyph, (getWidth() - 20) / 2, (getHeight() - 20) / 2);
    }
    return 1;
  }
  FXDCWindow dc(this, static_cast<FXEvent*>(ptr));
  const int w = getWidth(), h = getHeight();
  bool hot = underCursor() || specimen == 1;
  bool down = getState() == STATE_DOWN || specimen == 2;
  bool checked = selected || getState() == STATE_ENGAGED;
  dc.setForeground(s.surface);
  dc.fillRectangle(0, 0, w, h);
  dc.setForeground(checked || down ? s.selected : hot ? s.hover : s.surface);
  dc.fillRoundRectangle(1, 1, w - 3, h - 3, s.radius * 2, s.radius * 2);
  if (checked || hot || down) {
    dc.setForeground(checked ? s.accent : s.border);
    dc.drawRoundRectangle(1, 1, w - 3, h - 3, s.radius * 2, s.radius * 2);
  }
  dc.setForeground(isEnabled() ? s.text : s.muted);
  dc.setFont(getFont());
  int textWidth = getFont()->getTextWidth(getText());
  int total = textWidth + (glyph == Glyph::NoIcon ? 0 : 24), left = (w - total) / 2;
  if (glyph != Glyph::NoIcon) {
    drawGlyph(dc, glyph, left, (h - 20) / 2);
    left += 24;
  }
  dc.drawText(left, (h - getFont()->getFontHeight()) / 2 + getFont()->getFontAscent(), getText());
  if (hasFocus() || specimen == 3) {
    dc.setForeground(s.accent);
    dc.drawFocusRectangle(4, 4, w - 8, h - 8);
  }
  return 1;
}
