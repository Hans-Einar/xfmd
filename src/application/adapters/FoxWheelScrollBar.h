#pragma once
#include "application/scroll/ScrollDynamics.h"
#include <functional>
#include <fx.h>
namespace xfmd {
class FoxWheelScrollBar : public FX::FXScrollBar {
  FXDECLARE(FoxWheelScrollBar)
  ScrollDynamics motion;
  ScrollProfile profile{1, false, .5, 3};
  int destination = 0, observed = 0, observedRange = 0, observedPage = 0;
  std::uint64_t finish = 0;
  double direction = 0;
  bool notifying = false;
  static FoxWheelScrollBar* activeBar;

protected:
  FoxWheelScrollBar() = default;

public:
  std::function<void(double)> zoomRequested;
  enum { ID_MOTION = FX::FXScrollBar::ID_LAST };
  FoxWheelScrollBar(FX::FXComposite*, FX::FXObject*, FX::FXSelector, FX::FXuint);
  ~FoxWheelScrollBar() override;
  static FX::FXScrollBar* replace(FX::FXScrollBar*);
  static void configureTree(FX::FXWindow*, const ScrollProfile&);
  static bool isWheelChange(FX::FXWindow* area) {
    return activeBar && activeBar->notifying && activeBar->getParent() == area;
  }
  static void cancelTree(FX::FXWindow*);
  void setProfile(const ScrollProfile& value) {
    cancelMotion();
    profile = value;
  }
  void cancelMotion();
  long onMouseWheel(FX::FXObject*, FX::FXSelector, void*);
  long onMotionTick(FX::FXObject*, FX::FXSelector, void*);
  long onPress(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
