#pragma once
#include "UiButton.h"
#include <functional>
namespace xfmd {
class UiRow : public FX::FXHorizontalFrame {
  UiContext& ui;

public:
  UiRow(FX::FXComposite*, UiContext&, FX::FXuint flags = 0);
  void restyle();
};
class PanelHeader : public FX::FXLabel {
  UiContext& ui;

public:
  PanelHeader(FX::FXComposite*, UiContext&, const char*);
  void restyle();
};
class UiFactory {
  UiContext& ui;

public:
  using Builder =
      std::function<UiButton*(FX::FXComposite*, UiContext&, const FX::FXString&, FX::FXObject*,
                              FX::FXSelector, UiIcon, ButtonRole, FX::FXuint)>;
  Builder buttonClass;
  explicit UiFactory(UiContext& context) : ui(context) {}
  UiButton* button(FX::FXComposite*, const FX::FXString&, FX::FXObject*, FX::FXSelector,
                   UiIcon = UiIcon::NoIcon, ButtonRole = ButtonRole::Toolbar, FX::FXuint flags = 0);
  UiRow* row(FX::FXComposite* parent, FX::FXuint flags = 0) { return new UiRow(parent, ui, flags); }
  PanelHeader* header(FX::FXComposite* p, const char* s) { return new PanelHeader(p, ui, s); }
};
} // namespace xfmd
