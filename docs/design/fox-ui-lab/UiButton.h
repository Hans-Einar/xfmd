#pragma once
#include "Style.h"
#include <string>

enum class Glyph { NoIcon, Open, Save, Back, Forward, Sidebar, Refresh, Search };
void drawGlyph(FXDCWindow&, Glyph, int, int);

class UiButton : public FXButton {
  FXDECLARE(UiButton)
  const Style* style = nullptr;
  Glyph glyph = Glyph::NoIcon;
  bool selected = false;
  int specimen = 0;

protected:
  UiButton() = default;

public:
  UiButton(FXComposite*, const Style&, const char*, Glyph = Glyph::NoIcon, bool selected = false,
           int specimen = 0);
  FXint getDefaultWidth() override;
  FXint getDefaultHeight() override;
  long onPaint(FXObject*, FXSelector, void*);
};
