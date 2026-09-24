#include "ReadingColorPopup.h"
#include <algorithm>
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(ReadingColorPopup) popupMap[] = {FXMAPFUNC(SEL_KEYPRESS, 0, ReadingColorPopup::onKey)};
FXIMPLEMENT(ReadingColorPopup, FXPopup, popupMap, ARRAYNUMBER(popupMap))
long ReadingColorPopup::onKey(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (event->code == KEY_Escape) {
    popdown();
    if (origin)
      origin->setFocus();
    return 1;
  }
  return FXPopup::onKeyPress(sender, sel, data);
}
ReadingColorPopup::ReadingColorPopup(FXWindow* owner, UiContext& context)
    : FXPopup(owner, POPUP_VERTICAL | FRAME_RAISED | FRAME_THICK), ui(&context) {
  auto* body = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 8, 8, 8, 8);
  new FXLabel(body, "Reading colors — Escape to close", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
  controls = new PreviewColorControls(body, context);
}
ReadingColorPopup::~ReadingColorPopup() {
  if (shown())
    hide();
}
void ReadingColorPopup::hide() {
  ungrabKeyboard();
  FXPopup::hide();
}
void ReadingColorPopup::showAt(FXWindow* anchor) {
  if (shown()) {
    popdown();
    return;
  }
  create();
  ui->apply(this);
  origin = anchor;
  int x, y;
  anchor->translateCoordinatesTo(x, y, getRoot(), anchor->getWidth(), anchor->getHeight());
  int width = getDefaultWidth(), height = getDefaultHeight();
  x = std::max(0, std::min(x - width, getRoot()->getWidth() - width));
  y = std::max(0, std::min(y, getRoot()->getHeight() - height));
  popup(nullptr, x, y, width, height);
  grabKeyboard();
  setFocus();
  controls->backgroundTone->setFocus();
}
} // namespace xfmd
