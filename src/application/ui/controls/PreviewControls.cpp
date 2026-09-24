#include "PreviewControls.h"
#include <cmath>
using namespace FX;
namespace xfmd {
PreviewControls::PreviewControls(FXComposite* p, UiContext& context, CommandRouter& commands)
    : UiRow(p, context), ui(context) {
  UiFactory factory(ui);
  wrap = factory.button(this, "Wrap\tWrap to window width", &commands, CommandRouter::WindowWrap);
  a4 = factory.button(this, "A4\tA4 page preview", &commands, CommandRouter::A4);
  menu = new FXMenuPane(this);
  addPresets(menu, commands);
  new FXMenuSeparator(menu);
  new FXMenuRadio(menu, "Fit page width", &commands, CommandRouter::FitWidth);
  new FXMenuRadio(menu, "Fit page height", &commands, CommandRouter::FitHeight);
  zoom = new FXMenuButton(this, "100%", nullptr, menu,
                          FRAME_RAISED | FRAME_THICK | MENUBUTTON_DOWN | ICON_AFTER_TEXT |
                              LAYOUT_CENTER_Y);
  zoom->setTipText("Document zoom for editor and preview");
  compact();
}
PreviewControls::~PreviewControls() { delete menu; }
void PreviewControls::addPresets(FXMenuPane* pane, CommandRouter& commands) {
  for (auto command : {CommandRouter::Zoom25, CommandRouter::Zoom50, CommandRouter::ActualSize,
                       CommandRouter::Zoom200, CommandRouter::Zoom300}) {
    auto label = std::to_string(int(CommandRouter::presetPercent(command))) + "%";
    new FXMenuRadio(pane, label.c_str(), &commands, command);
  }
}
void PreviewControls::sync(double percent, ZoomMode mode) {
  auto label = std::to_string(int(std::lround(percent))) + "%";
  zoom->setText(label.c_str());
  auto hint = mode == ZoomMode::FitWidth    ? "Fit page width"
              : mode == ZoomMode::FitHeight ? "Fit page height"
                                            : "Manual document zoom";
  zoom->setTipText((std::string(hint) + ": " + label).c_str());
  recalc();
}
void PreviewControls::compact() {
  setLayoutHints(LAYOUT_CENTER_Y);
  setPadLeft(0);
  setPadRight(0);
  setPadTop(0);
  setPadBottom(0);
  setHSpacing(1);
}
void PreviewControls::layout() {
  compact();
  UiRow::layout();
}
} // namespace xfmd
