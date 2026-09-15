#include "PreviewColorControls.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(PreviewColorControls)
colorMap[] = {
    FXMAPFUNC(SEL_CHANGED, PreviewColorControls::ID_COLOR, PreviewColorControls::onColor),
    FXMAPFUNC(SEL_COMMAND, PreviewColorControls::ID_COLOR, PreviewColorControls::onColor)};
FXIMPLEMENT(PreviewColorControls, FXVerticalFrame, colorMap, ARRAYNUMBER(colorMap))
PreviewColorControls::PreviewColorControls(FXComposite* parent, UiContext&)
    : FXVerticalFrame(parent, LAYOUT_FIX_WIDTH, 0, 0, 360, 0, 0, 0, 0, 0, 0, 0) {
  auto* row =
      new FXHorizontalFrame(this, LAYOUT_FILL_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0);
  auto column = [&](const char* label, FXSlider*& hue, FXSlider*& light) {
    auto* box = new FXHorizontalFrame(row, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0);
    new FXLabel(box, label, nullptr, LAYOUT_CENTER_Y, 0, 0, 0, 0, 1, 1, 0, 0);
    auto* sliders = new FXVerticalFrame(box, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    hue = new FXSlider(sliders, this, ID_COLOR,
                       SLIDER_HORIZONTAL | LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT, 0, 0, 0, 16);
    hue->setRange(-1, 359);
    light = new FXSlider(sliders, this, ID_COLOR,
                         SLIDER_HORIZONTAL | LAYOUT_FILL_X | LAYOUT_FIX_HEIGHT, 0, 0, 0, 16);
    light->setRange(0, 100);
  };
  column("BG", backgroundTone, backgroundBrightness);
  column("Text", textTone, textBrightness);
  sync(ReadingColors{}, false);
}
ReadingColors PreviewColorControls::values() const {
  return {backgroundTone->getValue(), backgroundBrightness->getValue(), textTone->getValue(),
          textBrightness->getValue()};
}
void PreviewColorControls::labels(const ReadingColors& c) {
  auto tone = [](int value) {
    return value < 0 ? std::string("neutral") : std::to_string(value) + "°";
  };
  backgroundTone->setTipText(
      ("Background hue: " + tone(c.backgroundTone) + "; left end is neutral").c_str());
  textTone->setTipText(("Text hue: " + tone(c.textTone) + "; left end is neutral").c_str());
  backgroundBrightness->setTipText(
      ("Background brightness: " + std::to_string(c.backgroundBrightness) + "%").c_str());
  textBrightness->setTipText(
      ("Text brightness: " + std::to_string(c.textBrightness) + "%").c_str());
}
void PreviewColorControls::sync(const ReadingColors& c, bool) {
  backgroundTone->setValue(c.backgroundTone);
  backgroundBrightness->setValue(c.backgroundBrightness);
  textTone->setValue(c.textTone);
  textBrightness->setValue(c.textBrightness);
  labels(c);
}
long PreviewColorControls::onColor(FXObject*, FXSelector sel, void*) {
  const auto colors = values();
  labels(colors);
  if (changed)
    changed(colors, FXSELTYPE(sel) == SEL_COMMAND);
  return 1;
}
} // namespace xfmd
