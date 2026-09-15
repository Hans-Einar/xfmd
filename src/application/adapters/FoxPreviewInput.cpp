#include "FoxRenderHost.h"
#include "FoxWheelScrollBar.h"
#include <FX88591Codec.h>
#include <algorithm>
#include <cmath>
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
void FoxRenderHost::paintSelection(cairo_t* cr) {
  if (!active || !current || selection.empty())
    return;
  cairo_save(cr);
  cairo_set_source_rgba(cr, .2, .5, 1, .35);
  for (auto box : selection.rectangles(*current))
    cairo_rectangle(cr, box.x, box.y, box.width, box.height);
  cairo_fill(cr);
  cairo_restore(cr);
}
void FoxRenderHost::copySelection() {
  if (!active || !current || selection.empty())
    return;
  const FXDragType types[] = {getApp()->registerDragType("UTF8_STRING"), stringType};
  if (acquireClipboard(types, 2))
    clipboardText = selection.text(*current);
}
long FoxRenderHost::onClipboardRequest(FXObject*, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (event->target != stringType && event->target != getApp()->registerDragType("UTF8_STRING"))
    return 0;
  const bool primary = FXSELTYPE(sel) == SEL_SELECTION_REQUEST;
  const auto value =
      primary ? (active && current ? selection.text(*current) : std::string{}) : clipboardText;
  FXString encoded(value.data(), value.size());
  if (event->target == stringType)
    encoded = FX88591Codec().utf2mb(encoded);
  return setDNDData(primary ? FROM_SELECTION : FROM_CLIPBOARD, event->target, encoded);
}
long FoxRenderHost::onSelectionLost(FXObject*, FXSelector, void*) {
  selection.clear();
  update();
  return 1;
}
long FoxRenderHost::onButtonPress(FXObject*, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  unsigned bit = FXSELTYPE(sel) == SEL_LEFTBUTTONPRESS     ? 1u
                 : FXSELTYPE(sel) == SEL_MIDDLEBUTTONPRESS ? 2u
                                                           : 4u;
  handle(this, FXSEL(SEL_FOCUS_SELF, 0), data);
  if (!buttons) {
    pressedFrame.reset();
    pressedLink.clear();
    clickCancelled = bit != 1 || (event->state & (MIDDLEBUTTONMASK | RIGHTBUTTONMASK));
    pressPoint = {double(event->win_x), double(event->win_y)};
    if (!clickCancelled && active && current) {
      auto point =
          transform.toDocument({double(event->win_x - pos_x), double(event->win_y - pos_y)});
      if (event->state & SHIFTMASK)
        selection.extend(PreviewSelection::hit(*current, point));
      else
        selection.start(PreviewSelection::hit(*current, point));
      update();
      pressedFrame = current->key;
      pressedLink = renderer
                        ->hitTest(*current, transform.toDocument({double(event->win_x - pos_x),
                                                                  double(event->win_y - pos_y)}))
                        .link;
    }
    if (event->state & SHIFTMASK)
      clickCancelled = true;
    grab();
  } else {
    clickCancelled = true;
  }
  buttons |= bit;
  return 1;
}
long FoxRenderHost::onPointer(FXObject*, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  unsigned bit = FXSELTYPE(sel) == SEL_LEFTBUTTONRELEASE     ? 1u
                 : FXSELTYPE(sel) == SEL_MIDDLEBUTTONRELEASE ? 2u
                                                             : 4u;
  bool activate = (buttons == 1 && bit == 1 && !clickCancelled &&
                   !(event->state & (MIDDLEBUTTONMASK | RIGHTBUTTONMASK)));
  buttons &= ~bit;
  // Release before stale-frame checks or callbacks (which can open modal dialogs).
  if (!buttons && grabbed())
    ungrab();
  if (activate && active && current && pressedFrame && *pressedFrame == current->key &&
      std::abs(event->win_x - pressPoint.x) <= getApp()->getDragDelta() &&
      std::abs(event->win_y - pressPoint.y) <= getApp()->getDragDelta()) {
    auto point = transform.toDocument({double(event->win_x - pos_x), double(event->win_y - pos_y)});
    auto hit = renderer->hitTest(*current, point);
    if (!hit.link.empty() && hit.link == pressedLink && linkActivated) {
      pressedFrame.reset();
      pressedLink.clear();
      linkActivated(hit.link);
    }
  }
  if (!buttons) {
    if (bit == 1 && active && current && !selection.empty()) {
      const FXDragType types[] = {getApp()->registerDragType("UTF8_STRING"), stringType};
      if (!hasSelection())
        acquireSelection(types, 2);
    }
    pressedFrame.reset();
    pressedLink.clear();
  }
  return 1;
}
long FoxRenderHost::onUngrabbed(FXObject* sender, FXSelector sel, void* data) {
  buttons = 0;
  pressedFrame.reset();
  pressedLink.clear();
  clickCancelled = true;
  return FXScrollArea::onUngrabbed(sender, sel, data);
}
long FoxRenderHost::onLeave(FXObject* sender, FXSelector sel, void* data) {
  if (linkHovered)
    linkHovered("");
  setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
  return FXScrollArea::onLeave(sender, sel, data);
}
long FoxRenderHost::onMotion(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (buttons && (std::abs(event->win_x - pressPoint.x) > getApp()->getDragDelta() ||
                  std::abs(event->win_y - pressPoint.y) > getApp()->getDragDelta()))
    clickCancelled = true;
  auto point = transform.toDocument({double(event->win_x - pos_x), double(event->win_y - pos_y)});
  if (buttons == 1 && clickCancelled && active && current && pressedFrame &&
      *pressedFrame == current->key) {
    selection.extend(PreviewSelection::hit(*current, point));
    // Continue selecting beyond the visible edge while the pointer is moving.
    if (event->win_y < 0 || event->win_y >= viewport_h)
      setPosition(pos_x, pos_y + (event->win_y < 0 ? 24 : -24));
    update();
  }
  auto target = active && current ? renderer->hitTest(*current, point).link : std::string{};
  bool link = !target.empty();
  if (linkHovered)
    linkHovered(target);
  setDefaultCursor(getApp()->getDefaultCursor(link ? DEF_HAND_CURSOR : DEF_TEXT_CURSOR));
  return 1;
}
long FoxRenderHost::onKeyPress(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (active && current && (event->state & CONTROLMASK)) {
    if (event->code == KEY_a || event->code == KEY_A) {
      selection.all(*current);
      const FXDragType types[] = {getApp()->registerDragType("UTF8_STRING"), stringType};
      if (!hasSelection())
        acquireSelection(types, 2);
      update();
      return 1;
    }
    if (event->code == KEY_c || event->code == KEY_C) {
      copySelection();
      return 1;
    }
  }
  if (event->code == KEY_Escape) {
    selection.clear();
    update();
    return 1;
  }
  int y = pos_y;
  switch (event->code) {
  case KEY_Page_Down:
    y -= std::max(20, viewport_h - 40);
    break;
  case KEY_Page_Up:
    y += std::max(20, viewport_h - 40);
    break;
  case KEY_Down:
    y -= 30;
    break;
  case KEY_Up:
    y += 30;
    break;
  case KEY_Home:
    y = 0;
    break;
  case KEY_End:
    y = -getContentHeight();
    break;
  default:
    return 0;
  }
  FoxWheelScrollBar::cancelTree(this);
  keyboard = true;
  setPosition(pos_x, y);
  keyboard = false;
  return 1;
}
} // namespace xfmd
