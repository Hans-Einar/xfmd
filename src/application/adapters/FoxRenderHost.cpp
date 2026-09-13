#include "FoxRenderHost.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include <algorithm>
#include "DisplayListPainter.h"
#include <cairo-xlib.h>
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxRenderHost)
renderMap[] = {FXMAPFUNC(SEL_PAINT, 0, FoxRenderHost::onPaint),
               FXMAPFUNC(SEL_KEYPRESS, 0, FoxRenderHost::onKeyPress),
               FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_MOTION, 0, FoxRenderHost::onMotion)};
FXIMPLEMENT(FoxRenderHost, FXScrollArea, renderMap, ARRAYNUMBER(renderMap))
FoxRenderHost::FoxRenderHost(FXComposite* parent, IRenderer& renderer, SharedTextMetrics& metrics)
    : FXScrollArea(parent, VSCROLLER_ALWAYS | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0),
      renderer(&renderer), metrics(&metrics) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
  enable(); // FXScrollArea does not enable native pointer/key dispatch by default.
  setBackColor(FXRGB(255, 255, 255));
}
FXint FoxRenderHost::getContentWidth() { return current ? current->contentWidth : 100; }
FXint FoxRenderHost::getContentHeight() { return current ? current->height : 60; }
void FoxRenderHost::layout() {
  FXScrollArea::layout();
  if (viewport_w > 0 && viewport_w != lastWidth) {
    lastWidth = viewport_w;
    active = false;
    if (resized)
      resized(viewport_w);
  }
}
void FoxRenderHost::present(LayoutResult frame) {
  if (!frame || frame->token != expected || frame->width != viewport_w)
    return;
  current = std::move(frame);
  active = true;
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
  if (!programmatic && active && viewportChanged)
    viewportChanged(-y);
}
void FoxRenderHost::setViewport(int y) {
  FoxWheelScrollBar::cancelTree(this);
  programmatic = true;
  setPosition(pos_x, -std::max(0, y));
  programmatic = false;
}
long FoxRenderHost::onPaint(FXObject*,FXSelector,void*) {
  auto* display=static_cast<Display*>(getApp()->getDisplay());
  auto* surface=cairo_xlib_surface_create(display,id(),static_cast<Visual*>(getVisual()->getVisual()),width,height);
  auto* cr=cairo_create(surface);cairo_set_source_rgb(cr,1,1,1);cairo_paint(cr);
  if(current) {
    cairo_translate(cr,pos_x,pos_y);
    DisplayListPainter painter(metrics->catalog);
    try {
      painter.paint(*current,cr,{-double(pos_x),-double(pos_y),double(viewport_w),double(viewport_h)},active);
    } catch(const std::exception& e) {
      active=false;
      cairo_identity_matrix(cr);cairo_set_source_rgb(cr,.65,.1,.1);cairo_move_to(cr,20,30);
      cairo_show_text(cr,e.what());
    }
  }
  cairo_destroy(cr);cairo_surface_destroy(surface);return 1;
}
long FoxRenderHost::onPointer(FXObject*, FXSelector, void* data) {
  if (!active || !current)
    return 1;
  auto* event = static_cast<FXEvent*>(data);
  auto hit = renderer->hitTest(*current, {event->win_x - pos_x, event->win_y - pos_y});
  if (!hit.link.empty() && linkActivated)
    linkActivated(hit.link);
  return 1;
}
long FoxRenderHost::onMotion(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  bool link =
      active && current &&
      !renderer->hitTest(*current, {event->win_x - pos_x, event->win_y - pos_y}).link.empty();
  setDefaultCursor(getApp()->getDefaultCursor(link ? DEF_HAND_CURSOR : DEF_ARROW_CURSOR));
  return 1;
}
long FoxRenderHost::onKeyPress(FXObject*, FXSelector, void* data) {
  FoxWheelScrollBar::cancelTree(this);
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
  setPosition(pos_x, y);
  return 1;
}
} // namespace xfmd
