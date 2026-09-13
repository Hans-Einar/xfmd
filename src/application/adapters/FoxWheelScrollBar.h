#pragma once
#include <cstdint>
#include <fx.h>
namespace xfmd {
// FOX 1.6 wheel animation with subpixel movement retained between wheel events.
class FoxWheelScrollBar : public FX::FXScrollBar {
  FXDECLARE(FoxWheelScrollBar)
  std::int64_t remainder = 0;

protected:
  FoxWheelScrollBar() = default;

public:
  FoxWheelScrollBar(FX::FXComposite*, FX::FXObject*, FX::FXSelector, FX::FXuint);
  // Used by scroll-area constructors before FOX resources are created.
  static FX::FXScrollBar* replace(FX::FXScrollBar*);
  long onMouseWheel(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
