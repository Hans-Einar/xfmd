#pragma once
#include <functional>
#include <fx.h>
namespace xfmd {
// Common lower-sidebar list: wheel behavior and non-activating context gestures.
class SidebarPathList : public FX::FXList {
public:
  std::function<void(int, const FX::FXEvent&)> contextRequested;
  SidebarPathList(FX::FXComposite*, FX::FXObject*, FX::FXSelector);
  long handle(FX::FXObject*, FX::FXSelector, void*) override;
};
} // namespace xfmd
