#include "SdpTree.h"
using namespace FX;
namespace xfmd {
long SdpTree::handle(FXObject* sender, FXSelector selector, void* data) {
  if (FXSELTYPE(selector) == SEL_RIGHTBUTTONPRESS) {
    cancelActivation();
    return 1; // No FOX pan/grab and no normal row activation.
  }
  if (FXSELTYPE(selector) == SEL_RIGHTBUTTONRELEASE) {
    const auto event = *static_cast<FXEvent*>(data);
    auto* item = dynamic_cast<SdpItem*>(getItemAt(event.win_x, event.win_y));
    if (!event.moved && item && !item->source.empty() && contextRequested) {
      const auto source = item->source;
      contextRequested(source, event);
    }
    return 1;
  }
  if (FXSELTYPE(selector) == SEL_QUERY_TIP && sender) {
    FXint x = 0, y = 0;
    FXuint buttons = 0;
    getCursorPosition(x, y, buttons);
    if (auto* item = getItemAt(x, y)) {
      auto text = item->getText();
      sender->handle(this, FXSEL(SEL_COMMAND, ID_SETSTRINGVALUE), &text);
      return 1;
    }
  }
  return NavigationTree::handle(sender, selector, data);
}
} // namespace xfmd
