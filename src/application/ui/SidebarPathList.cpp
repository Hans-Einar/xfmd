#include "SidebarPathList.h"
#include "application/adapters/FoxWheelScrollBar.h"
using namespace FX;
namespace xfmd {
SidebarPathList::SidebarPathList(FXComposite* parent, FXObject* target, FXSelector selector)
    : FXList(parent, target, selector, LIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
}
long SidebarPathList::handle(FXObject* sender, FXSelector selector, void* data) {
  // Do not enter FOX's right-button pan/grab or alter the selected/activated row.
  if (FXSELTYPE(selector) == SEL_RIGHTBUTTONPRESS)
    return 1;
  if (FXSELTYPE(selector) == SEL_RIGHTBUTTONRELEASE) {
    const auto event = *static_cast<FXEvent*>(data);
    const auto row = getItemAt(event.win_x, event.win_y);
    if (!event.moved && row >= 0 && contextRequested)
      contextRequested(row, event);
    return 1;
  }
  return FXList::handle(sender, selector, data);
}
} // namespace xfmd
