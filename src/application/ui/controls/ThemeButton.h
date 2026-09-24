#pragma once
#include "ReadingColorPopup.h"
#include "UiButton.h"
namespace xfmd {
class ThemeButton : public UiButton {
  FXDECLARE(ThemeButton)
  ReadingColorPopup* colors = nullptr;

protected:
  ThemeButton() = default;

public:
  ThemeButton(FX::FXComposite*, UiContext&, FX::FXObject*, FX::FXSelector, ReadingColorPopup*);
  long onRight(FX::FXObject*, FX::FXSelector, void*);
  long onKey(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
