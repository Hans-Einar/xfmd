#pragma once
#include "application/scroll/ScrollDynamics.h"
#include <fx.h>
namespace xfmd {
// FOX 1.6 wheel animation with subpixel movement retained between wheel events.
class FoxWheelScrollBar : public FX::FXScrollBar {
  FXDECLARE(FoxWheelScrollBar)
  ScrollDynamics motion;
  ScrollProfile profile{1,false,.5,3};

protected:
  FoxWheelScrollBar() = default;

public:
  FoxWheelScrollBar(FX::FXComposite*, FX::FXObject*, FX::FXSelector, FX::FXuint);
  // Used by scroll-area constructors before FOX resources are created.
  static FX::FXScrollBar* replace(FX::FXScrollBar*);
  static void configureTree(FX::FXWindow*, const ScrollProfile&);
  void setProfile(const ScrollProfile& value) { profile=value;motion.reset(); }
  long onMouseWheel(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
