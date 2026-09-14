#pragma once
#include "UiLayout.h"
#include "application/preferences/ReadingColors.h"
namespace xfmd {
class PreviewColorControls : public FX::FXVerticalFrame {
  FXDECLARE(PreviewColorControls)
  FX::FXLabel *backgroundLabel = nullptr, *textLabel = nullptr;
  FX::FXLabel *backgroundLightLabel = nullptr, *textLightLabel = nullptr, *contrast = nullptr;
  bool dark = false;
  void labels(const ReadingColors&);

protected:
  PreviewColorControls() = default;

public:
  enum { ID_COLOR = FX::FXVerticalFrame::ID_LAST, ID_RESET, ID_LAST };
  FX::FXSlider *backgroundTone = nullptr, *backgroundBrightness = nullptr;
  FX::FXSlider *textTone = nullptr, *textBrightness = nullptr;
  std::function<void(const ReadingColors&, bool)> changed;
  PreviewColorControls(FX::FXComposite*, UiContext&);
  ReadingColors values() const;
  void sync(const ReadingColors&, bool darkTheme);
  void showError(const std::string&);
  long onColor(FX::FXObject*, FX::FXSelector, void*);
  long onReset(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
