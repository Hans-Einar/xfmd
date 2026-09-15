#pragma once
#include "UiLayout.h"
#include "application/preferences/ReadingColors.h"
namespace xfmd {
class PreviewColorControls : public FX::FXVerticalFrame {
  FXDECLARE(PreviewColorControls)
  void labels(const ReadingColors&);

protected:
  PreviewColorControls() = default;

public:
  enum { ID_COLOR = FX::FXVerticalFrame::ID_LAST, ID_LAST };
  FX::FXSlider *backgroundTone = nullptr, *backgroundBrightness = nullptr;
  FX::FXSlider *textTone = nullptr, *textBrightness = nullptr;
  std::function<void(const ReadingColors&, bool)> changed;
  PreviewColorControls(FX::FXComposite*, UiContext&);
  ReadingColors values() const;
  void sync(const ReadingColors&, bool darkTheme);
  long onColor(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
