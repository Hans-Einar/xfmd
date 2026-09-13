#include "FoxRenderHost.h"
#include "DisplayListPainter.h"
#include "FoxWheelScrollBar.h"
#include <algorithm>

#include <cmath>
#include <cstring>
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxRenderHost)
renderMap[] = {FXMAPFUNC(SEL_PAINT, 0, FoxRenderHost::onPaint),
               FXMAPFUNC(SEL_KEYPRESS, 0, FoxRenderHost::onKeyPress),
               FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_MIDDLEBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_RIGHTBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_UNGRABBED, 0, FoxRenderHost::onUngrabbed),
               FXMAPFUNC(SEL_MOTION, 0, FoxRenderHost::onMotion)};
FXIMPLEMENT(FoxRenderHost, FXScrollArea, renderMap, ARRAYNUMBER(renderMap))
FoxRenderHost::FoxRenderHost(FXComposite* parent, IRenderer& r, SharedTextMetrics& m)
    : FXScrollArea(parent, VSCROLLER_ALWAYS | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0),
      renderer(&r), metrics(&m) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
  enable();
  setBackColor(FXRGB(255, 255, 255));
  dpiScale =
      std::clamp(getApp()->reg().readRealEntry("SETTINGS", "screenres", 96.0) / 72.0, .5, 4.0);
}
FXint FoxRenderHost::getContentWidth() { return current ? transform.contentWidth : 100; }
FXint FoxRenderHost::getContentHeight() { return current ? transform.contentHeight : 60; }
void FoxRenderHost::layout() {
  const auto before = transform.toDocument({-double(pos_x), -double(pos_y)});
  FXScrollArea::layout();
  if (current) {
    transform.configure(*current, viewport_w, dpiScale, zoom, fit);
    programmatic = true;
    FXScrollArea::layout();
    programmatic = false;
  }
  const double flowWidth = viewport_w / dpiScale;
  if (viewport_w > 0 && flowWidth != lastWidth) {
    lastWidth = flowWidth;
    if (current && current->key.profile.mode == LayoutMode::Paged)
      setViewport(before.y);
    else
      active = false;
    if (resized)
      resized(flowWidth);
  }
}
void FoxRenderHost::expect(DocumentToken token) {
  clickCancelled = true;
  if (token.document != expected.document)
    current.reset();
  expected = token;
  requested.reset();
  active = false;
  update();
}
void FoxRenderHost::expectLayout(FrameKey key) {
  clickCancelled = true;
  expected = key.token;
  requested = std::move(key);
  active = false;
  update();
}
void FoxRenderHost::present(LayoutResult frame) {
  if (!frame || frame->token != expected || (requested && !(frame->key == *requested)))
    return;
  if (frame->key.profile.mode == LayoutMode::Continuous &&
      std::abs(frame->width - viewport_w / dpiScale) > .01)
    return;
  current = std::move(frame);
  active = true;
  transform.configure(*current, viewport_w, dpiScale, zoom, fit);
  programmatic = true;
  FXScrollArea::layout();
  programmatic = false;
  recalc();
  update();
}
void FoxRenderHost::moveContents(FXint x, FXint y) {
  pos_x = x;
  pos_y = y;
  update();
  if (!programmatic && active && viewportChanged) {
    lastScrollOrigin = FoxWheelScrollBar::isWheelChange(this) ? ScrollOrigin::UserWheel
                       : keyboard                             ? ScrollOrigin::Keyboard
                                                              : ScrollOrigin::UserDrag;
    viewportChanged(transform.toDocument({-double(x), -double(y)}).y);
  }
}
void FoxRenderHost::setViewport(double y, ScrollOrigin origin) {
  FoxWheelScrollBar::cancelTree(this);
  programmatic = true;
  lastScrollOrigin = origin;
  setPosition(pos_x, -int(std::lround(transform.toView({0, std::max(0.0, y)}).y)));
  programmatic = false;
}
void FoxRenderHost::setViewScale(bool fitWidth, double factor) {
  if (!std::isfinite(factor))
    return;
  auto before = transform.toDocument({-double(pos_x), -double(pos_y)});
  fit = fitWidth;
  zoom = std::clamp(factor, .25, 4.0);
  if (current) {
    transform.configure(*current, viewport_w, dpiScale, zoom, fit);
    programmatic = true;
    FXScrollArea::layout();
    programmatic = false;
    setViewport(before.y);
  }
  recalc();
  update();
}
long FoxRenderHost::onPaint(FXObject*, FXSelector, void*) {
  cairo_surface_t* surface = nullptr;
  try {
    surface = canvas.begin(*getApp(), viewport_w, viewport_h);
  } catch (const std::exception& e) {
    active = false;
    FXDCWindow dc(this);
    dc.setForeground(FXRGB(200, 20, 20));
    dc.drawText(20, 30, e.what(), std::strlen(e.what()));
    return 1;
  }
  auto* cr = cairo_create(surface);
  bool paged = current && current->key.profile.mode == LayoutMode::Paged;
  cairo_set_source_rgb(cr, paged ? .18 : 1, paged ? .20 : 1, paged ? .23 : 1);
  cairo_paint(cr);
  cairo_rectangle(cr, 0, 0, viewport_w, viewport_h);
  cairo_clip(cr);
  try {
    if (current) {
      DisplayListPainter painter(metrics->catalog);
      if (paged) {
        const auto& paper = current->pages.paper;
        for (std::size_t i = 0; i < current->pages.slices.size(); ++i) {
          const auto box = transform.pageRect(i);
          if (box.y + pos_y > viewport_h || box.y + box.height + pos_y < 0)
            continue;
          cairo_set_source_rgb(cr, 1, 1, 1);
          cairo_rectangle(cr, box.x + pos_x, box.y + pos_y, box.width, box.height);
          cairo_fill(cr);
          cairo_save(cr);
          cairo_translate(cr, box.x + pos_x, box.y + pos_y - i * paper.height * transform.scale);
          cairo_scale(cr, transform.scale, transform.scale);
          painter.paint(*current, cr, {0, i * paper.height, paper.width, paper.height}, active);
          cairo_restore(cr);
        }
      } else {
        cairo_translate(cr, pos_x, pos_y);
        cairo_scale(cr, transform.scale, transform.scale);
        auto top = transform.toDocument({-double(pos_x), -double(pos_y)});
        painter.paint(*current, cr,
                      {top.x, top.y, viewport_w / transform.scale, viewport_h / transform.scale},
                      active);
      }
    }
  } catch (const std::exception& e) {
    active = false;
    cairo_identity_matrix(cr);
    cairo_set_source_rgb(cr, .8, .1, .1);
    cairo_move_to(cr, 20, 30);
    cairo_show_text(cr, e.what());
  }
  cairo_destroy(cr);
  canvas.present(*this);
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
      pressedFrame = current->key;
      pressedLink = renderer
                        ->hitTest(*current, transform.toDocument({double(event->win_x - pos_x),
                                                                  double(event->win_y - pos_y)}))
                        .link;
    }
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
long FoxRenderHost::onMotion(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (buttons && (std::abs(event->win_x - pressPoint.x) > getApp()->getDragDelta() ||
                  std::abs(event->win_y - pressPoint.y) > getApp()->getDragDelta()))
    clickCancelled = true;
  auto point = transform.toDocument({double(event->win_x - pos_x), double(event->win_y - pos_y)});
  bool link = active && current && !renderer->hitTest(*current, point).link.empty();
  setDefaultCursor(getApp()->getDefaultCursor(link ? DEF_HAND_CURSOR : DEF_ARROW_CURSOR));
  return 1;
}
long FoxRenderHost::onKeyPress(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
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
