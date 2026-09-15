#pragma once
#include "UiLayout.h"
#include "application/commands/CommandRouter.h"
namespace xfmd {
class PreviewControls : public UiRow {
  UiContext& ui;
  UiButton *wrap, *a4;
  FX::FXMenuButton* zoom;
  FX::FXMenuPane* menu;
  bool paged = false;

public:
  PreviewControls(FX::FXComposite*, UiContext&, CommandRouter&);
  ~PreviewControls() override;
  void sync(bool paged, bool fitWidth);
  void layout() override;
  void compact();
};
} // namespace xfmd
