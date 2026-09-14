#include "PreviewColorControls.h"
#include <cstdio>
using namespace FX;
namespace xfmd {
FXDEFMAP(PreviewColorControls)
colorMap[] = {
    FXMAPFUNC(SEL_CHANGED, PreviewColorControls::ID_COLOR, PreviewColorControls::onColor),
    FXMAPFUNC(SEL_COMMAND, PreviewColorControls::ID_COLOR, PreviewColorControls::onColor),
    FXMAPFUNC(SEL_COMMAND, PreviewColorControls::ID_RESET, PreviewColorControls::onReset)};
FXIMPLEMENT(PreviewColorControls, FXVerticalFrame, colorMap, ARRAYNUMBER(colorMap))
PreviewColorControls::PreviewColorControls(FXComposite* parent, UiContext& ui)
    : FXVerticalFrame(parent, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {
  auto* row = new UiRow(this, ui, PACK_UNIFORM_WIDTH);
  auto column = [&](FXLabel*& hueLabel, FXSlider*& hue, FXLabel*& lightLabel, FXSlider*& light) {
    auto* box = new FXVerticalFrame(row, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    hueLabel = new FXLabel(box, "", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
    hue = new FXSlider(box, this, ID_COLOR, SLIDER_HORIZONTAL | LAYOUT_FILL_X);
    hue->setRange(-1, 359);
    hue->setTipText("Left end: neutral gray. Otherwise hue 0–359°, fixed saturation.");
    lightLabel = new FXLabel(box, "", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
    light = new FXSlider(box, this, ID_COLOR, SLIDER_HORIZONTAL | LAYOUT_FILL_X);
    light->setRange(0, 100);
    light->setTipText("RGB brightness 0–100%. Does not change monitor brightness.");
  };
  column(backgroundLabel, backgroundTone, backgroundLightLabel, backgroundBrightness);
  column(textLabel, textTone, textLightLabel, textBrightness);
  auto* footer = new UiRow(this, ui);
  contrast = new FXLabel(footer, "", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);

  UiFactory(ui).button(footer, "Reset colors", this, ID_RESET, UiIcon::NoIcon, ButtonRole::Normal);
  sync(ReadingColors{}, false);
}
ReadingColors PreviewColorControls::values() const {
  return {backgroundTone->getValue(), backgroundBrightness->getValue(), textTone->getValue(),
          textBrightness->getValue()};
}
void PreviewColorControls::labels(const ReadingColors& c) {
  contrast->setTipText("Body text/background sRGB contrast. WCAG: 4.5:1 minimum; 7:1 enhanced.");
  auto tone = [](int value) {
    return value < 0 ? std::string("neutral") : std::to_string(value) + "°";
  };
  backgroundLabel->setText(("Background hue: " + tone(c.backgroundTone)).c_str());
  textLabel->setText(("Text hue: " + tone(c.textTone)).c_str());
  backgroundLightLabel->setText(
      ("Brightness: " + std::to_string(c.backgroundBrightness) + "%").c_str());
  textLightLabel->setText(("Brightness: " + std::to_string(c.textBrightness) + "%").c_str());
  char message[100];
  double ratio = ReadingPalette::from(c).contrast();
  std::snprintf(message, sizeof(message), "%s · Contrast %.1f:1%s", dark ? "Dark" : "Light", ratio,
                ratio < 4.5 ? " (low)" : "");
  contrast->setText(message);
}
void PreviewColorControls::sync(const ReadingColors& c, bool darkTheme) {
  dark = darkTheme;
  backgroundTone->setValue(c.backgroundTone);
  backgroundBrightness->setValue(c.backgroundBrightness);
  textTone->setValue(c.textTone);
  textBrightness->setValue(c.textBrightness);
  labels(c);
}
void PreviewColorControls::showError(const std::string& error) {
  contrast->setText("Colors not saved (see tooltip)");
  contrast->setTipText(error.c_str());
}
long PreviewColorControls::onColor(FXObject*, FXSelector sel, void*) {
  const auto colors = values();
  labels(colors);
  if (changed)
    changed(colors, FXSELTYPE(sel) == SEL_COMMAND);
  return 1;
}
long PreviewColorControls::onReset(FXObject*, FXSelector, void*) {
  auto colors = ReadingColors::defaults(dark);
  sync(colors, dark);
  if (changed)
    changed(colors, true);
  return 1;
}
} // namespace xfmd
