#include "PreviewControls.h"
using namespace FX;
namespace xfmd {
PreviewControls::PreviewControls(FXComposite* p, UiContext& context, CommandRouter& commands)
    : UiRow(p, context), ui(context) {
  UiFactory factory(ui);
  title = new FXLabel(this, "PREVIEW", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  wrap = factory.button(this, "Wrap\tWrap to window width", &commands, CommandRouter::WindowWrap);
  a4 = factory.button(this, "A4\tA4 page preview", &commands, CommandRouter::A4);
  menu = new FXMenuPane(this);
  new FXMenuRadio(menu, "Fit page width", &commands, CommandRouter::FitWidth);
  new FXMenuRadio(menu, "Actual size (100%)", &commands, CommandRouter::ActualSize);
  zoom = new FXMenuButton(this, "Fit width", nullptr, menu,
                          FRAME_RAISED | FRAME_THICK | MENUBUTTON_DOWN | LAYOUT_CENTER_Y);
  zoom->setTipText("A4 preview zoom");
}
PreviewControls::~PreviewControls() { delete menu; }
void PreviewControls::sync(bool value, bool fit) {
  paged = value;
  zoom->setText(fit ? "Fit width" : "100%");
  recalc();
}
void PreviewControls::layout() {
  const int space = ui.metrics().inset * 2 + ui.metrics().gap * 3;
  bool formats = getWidth() >= wrap->getDefaultWidth() + a4->getDefaultWidth() + space;
  if (formats) {
    wrap->show();
    a4->show();
  } else {
    wrap->hide();
    a4->hide();
  }
  bool showZoom = formats && paged &&
                  getWidth() >= wrap->getDefaultWidth() + a4->getDefaultWidth() +
                                    zoom->getDefaultWidth() + space;
  if (showZoom)
    zoom->show();
  else
    zoom->hide();
  UiRow::layout();
}
} // namespace xfmd
