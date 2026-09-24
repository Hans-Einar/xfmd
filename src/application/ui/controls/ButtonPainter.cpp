#include "ButtonPainter.h"
using namespace FX;
namespace xfmd {
namespace {
class Flat : public ButtonPainter {
public:
  void paint(FXDCWindow& dc, const UiPalette& p, const UiMetrics& m, int w, int h, ButtonRole role,
             const ButtonVisualState& s) const override {
    const bool active = s.checked || s.pressed || role == ButtonRole::Primary;
    const int diameter = role == ButtonRole::Pill ? h - 3 : m.radius * 2;
    dc.setForeground(active ? p.selected : s.hovered && s.enabled ? p.hover : p.surface);
    dc.fillRoundRectangle(1, 1, w - 3, h - 3, diameter, diameter);
    if (active || s.hovered || role != ButtonRole::Toolbar || s.defaultButton) {
      dc.setForeground(active || s.defaultButton ? p.accent : p.border);
      dc.drawRoundRectangle(1, 1, w - 3, h - 3, diameter, diameter);
    }
    if (s.checked) {
      dc.setForeground(p.accent);
      dc.fillRectangle(w / 2 - 4, h - 4, 8, 2);
    }
  }
};
class Classic : public ButtonPainter {
public:
  void paint(FXDCWindow& dc, const UiPalette& p, const UiMetrics& m, int w, int h, ButtonRole role,
             const ButtonVisualState& s) const override {
    if (role == ButtonRole::Pill) {
      flatButtonPainter().paint(dc, p, m, w, h, role, s);
      return;
    }
    bool down = s.pressed || s.checked;
    dc.setForeground(down || role == ButtonRole::Primary ? p.selected : p.surface);
    dc.fillRectangle(1, 1, w - 2, h - 2);
    dc.setForeground(down ? p.border : p.panel);
    dc.drawLine(0, 0, w - 1, 0);
    dc.drawLine(0, 0, 0, h - 1);
    dc.setForeground(down ? p.panel : p.border);
    dc.drawLine(0, h - 1, w - 1, h - 1);
    dc.drawLine(w - 1, 0, w - 1, h - 1);
    if (s.checked || s.defaultButton) {
      dc.setForeground(p.accent);
      dc.drawRectangle(2, 2, w - 5, h - 5);
    }
  }
};
} // namespace
const ButtonPainter& flatButtonPainter() {
  static Flat value;
  return value;
}
const ButtonPainter& classicButtonPainter() {
  static Classic value;
  return value;
}
} // namespace xfmd
