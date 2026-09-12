#include "FoxRenderHost.h"
#include <algorithm>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxRenderHost) renderMap[] = {
  FXMAPFUNC(SEL_PAINT, 0, FoxRenderHost::onPaint),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
  FXMAPFUNC(SEL_MOTION, 0, FoxRenderHost::onMotion)};
FXIMPLEMENT(FoxRenderHost, FXScrollArea, renderMap, ARRAYNUMBER(renderMap))
FoxRenderHost::FoxRenderHost(FXComposite* parent, IRenderer& renderer, FoxTextMetrics& metrics)
    : FXScrollArea(parent, VSCROLLER_ALWAYS | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0), renderer(&renderer), metrics(&metrics) {
  setBackColor(FXRGB(255, 255, 255));
}
FXint FoxRenderHost::getContentWidth() { return current ? current->contentWidth : 100; }
FXint FoxRenderHost::getContentHeight() { return current ? current->height : 60; }
void FoxRenderHost::layout() {
  FXScrollArea::layout();
  if (viewport_w > 0 && viewport_w != lastWidth) {
    lastWidth = viewport_w; active = false;
    if (resized) resized(viewport_w);
  }
}
void FoxRenderHost::present(LayoutResult frame) {
  if (!frame || frame->token != expected || frame->width != viewport_w) return;
  current = std::move(frame); active = true;
  programmatic = true; FXScrollArea::layout(); programmatic = false;
  recalc(); update();
}
void FoxRenderHost::moveContents(FXint x, FXint y) {
  pos_x = x; pos_y = y; update();
  if (!programmatic && active && viewportChanged) viewportChanged(-y);
}
void FoxRenderHost::setViewport(int y) {
  programmatic = true;
  setPosition(pos_x, -std::max(0, y));
  programmatic = false;
}
long FoxRenderHost::onPaint(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  FXDCWindow dc(this, event);
  dc.setForeground(getBackColor()); dc.fillRectangle(0, 0, width, height);
  dc.setClipRectangle(0, 0, viewport_w, viewport_h);
  if (!current) {
    dc.setFont(metrics->font({})); dc.setForeground(FXRGB(90, 99, 112));
    dc.drawText(24, 40, "Open a Markdown or text file.", 29); return 1;
  }
  for (const auto& decoration : current->decorations) {
    auto r = decoration.bounds;
    if (r.y + r.height < -pos_y || r.y > -pos_y + viewport_h) continue;
    dc.setForeground(FXRGB((decoration.color >> 16) & 255, (decoration.color >> 8) & 255, decoration.color & 255));
    dc.fillRectangle(r.x + pos_x, r.y + pos_y, r.width, r.height);
  }
  auto first = std::lower_bound(current->runs.begin(), current->runs.end(), -pos_y - 150,
      [](const DrawRun& run, int y) { return run.bounds.y < y; });
  for (; first != current->runs.end() && first->bounds.y < -pos_y + viewport_h; ++first) {
    const auto& run = *first; auto r = run.bounds;
    if (run.codeBackground) { dc.setForeground(FXRGB(239, 241, 245)); dc.fillRectangle(r.x + pos_x, r.y + pos_y, r.width, r.height); }
    dc.setForeground(!active ? FXRGB(135, 135, 135) : run.link.empty() ? FXRGB(29, 37, 49) : FXRGB(24, 85, 166));
    int textX = r.x + pos_x;
    for (const auto& segment : metrics->segments(run.text, run.font)) {
      dc.setFont(segment.second);
      dc.drawText(textX, r.y + pos_y + run.ascent, segment.first.data(), int(segment.first.size()));
      textX += segment.second->getTextWidth(segment.first.data(), int(segment.first.size()));
    }
    if (!run.link.empty()) dc.drawLine(r.x + pos_x, r.y + pos_y + run.ascent + 2, r.x + r.width + pos_x, r.y + pos_y + run.ascent + 2);
  }
  return 1;
}
long FoxRenderHost::onPointer(FXObject*, FXSelector, void* data) {
  if (!active || !current) return 1;
  auto* event = static_cast<FXEvent*>(data);
  auto hit = renderer->hitTest(*current, {event->win_x - pos_x, event->win_y - pos_y});
  if (!hit.link.empty() && linkActivated) linkActivated(hit.link);
  return 1;
}
long FoxRenderHost::onMotion(FXObject*, FXSelector, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  bool link = active && current && !renderer->hitTest(*current, {event->win_x - pos_x, event->win_y - pos_y}).link.empty();
  setDefaultCursor(getApp()->getDefaultCursor(link ? DEF_HAND_CURSOR : DEF_ARROW_CURSOR));
  return 1;
}
}
