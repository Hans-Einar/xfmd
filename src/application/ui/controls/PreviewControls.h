#pragma once
#include "UiLayout.h"
#include "application/commands/CommandRouter.h"
#include "application/zoom/ZoomMode.h"
namespace xfmd {
class PreviewControls : public UiRow {
  UiContext& ui;
  UiButton *wrap, *a4;
  FX::FXMenuButton* zoom;
  FX::FXMenuPane* menu;

public:
  PreviewControls(FX::FXComposite*, UiContext&, CommandRouter&);
  ~PreviewControls() override;
  static void addPresets(FX::FXMenuPane*, CommandRouter&);
  void sync(double percent, ZoomMode);
  void layout() override;
  void compact();
};
} // namespace xfmd
