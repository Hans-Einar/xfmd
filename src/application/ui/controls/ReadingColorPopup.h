#pragma once
#include "PreviewColorControls.h"
namespace xfmd {
class ReadingColorPopup : public FX::FXPopup {
  FXDECLARE(ReadingColorPopup)
  UiContext* ui = nullptr;
  FX::FXWindow* origin = nullptr;

protected:
  ReadingColorPopup() = default;

public:
  PreviewColorControls* controls = nullptr;
  ReadingColorPopup(FX::FXWindow*, UiContext&);
  long onKey(FX::FXObject*, FX::FXSelector, void*);
  ~ReadingColorPopup() override;
  void hide() override;
  void showAt(FX::FXWindow* anchor);
};
} // namespace xfmd
