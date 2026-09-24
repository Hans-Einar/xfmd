#pragma once
#include "application/ui/style/ThemeProfiles.h"
namespace xfmd {
enum class ButtonRole { Toolbar, Normal, Primary, Pill };
struct ButtonVisualState {
  bool enabled, hovered, pressed, checked, focused, defaultButton;
};
class ButtonPainter {
public:
  virtual ~ButtonPainter() = default;
  virtual void paint(FX::FXDCWindow&, const UiPalette&, const UiMetrics&, int, int, ButtonRole,
                     const ButtonVisualState&) const = 0;
};
const ButtonPainter& flatButtonPainter();
const ButtonPainter& classicButtonPainter();
} // namespace xfmd
