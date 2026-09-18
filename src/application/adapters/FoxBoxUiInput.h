#pragma once
#include <cstdint>
#include <functional>
#include <fx.h>
namespace xfmd {
class FoxBoxUiField : public FX::FXTextField {
  FXDECLARE(FoxBoxUiField)
  bool enterHeld = false;

protected:
  FoxBoxUiField() = default;

public:
  std::function<void()> commit, restore;
  FoxBoxUiField(FX::FXComposite*, FX::FXObject*, FX::FXSelector);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onKeyRelease(FX::FXObject*, FX::FXSelector, void*);
};
class FoxBoxUiButton : public FX::FXButton {
  FXDECLARE(FoxBoxUiButton)
  std::uint64_t armed = 0;
  bool held = false;

protected:
  FoxBoxUiButton() = default;

public:
  std::uint64_t publication = 0;
  FoxBoxUiButton(FX::FXComposite*, const char*, FX::FXObject*, FX::FXSelector);
  long onPress(FX::FXObject*, FX::FXSelector, void*);
  long onRelease(FX::FXObject*, FX::FXSelector, void*);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onKeyRelease(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
