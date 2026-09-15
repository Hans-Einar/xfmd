#include "PreviewControls.h"
using namespace FX;
namespace xfmd {
PreviewControls::PreviewControls(FXComposite* p, UiContext& context, CommandRouter& commands)
    : UiRow(p, context), ui(context) {
  UiFactory factory(ui);
  wrap = factory.button(this, "Wrap\tWrap to window width", &commands, CommandRouter::WindowWrap);
  a4 = factory.button(this, "A4\tA4 page preview", &commands, CommandRouter::A4);
  menu = new FXMenuPane(this);
  new FXMenuRadio(menu, "Fit page width", &commands, CommandRouter::FitWidth);
  new FXMenuRadio(menu, "Actual size (100%)", &commands, CommandRouter::ActualSize);
  zoom = new FXMenuButton(this, "Fit width", nullptr, menu,
                          FRAME_RAISED | FRAME_THICK | MENUBUTTON_DOWN | LAYOUT_CENTER_Y);
  zoom->setTipText("A4 scale for editor and preview");
  compact();
}
PreviewControls::~PreviewControls() { delete menu; }
void PreviewControls::sync(bool value, bool fit) {
  paged = value;
  if (paged)
    zoom->enable();
  else
    zoom->disable();
  zoom->setText(fit ? "Fit width" : "100%");
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
