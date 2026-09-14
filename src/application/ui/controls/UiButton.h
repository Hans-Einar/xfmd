#pragma once
#include "application/ui/style/UiContext.h"
namespace xfmd {
class UiButton : public FX::FXButton {
  FXDECLARE(UiButton)
  UiContext* ui = nullptr;
  UiIcon glyph = UiIcon::NoIcon;
  bool checked = false;
  ButtonRole role = ButtonRole::Normal;

protected:
  UiButton() = default;

public:
  UiButton(FX::FXComposite*, UiContext&, const FX::FXString&, FX::FXObject*, FX::FXSelector,
           UiIcon = UiIcon::NoIcon, ButtonRole = ButtonRole::Normal, FX::FXuint flags = 0);
  bool isChecked() const { return checked; }
  long onChecked(FX::FXObject*, FX::FXSelector, void*);
  void create() override;
  FX::FXint getDefaultWidth() override;
  FX::FXint getDefaultHeight() override;
  long onPaint(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
