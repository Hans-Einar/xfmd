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
renderMap[] = {FXMAPFUNC(SEL_MOUSEWHEEL, 0, FoxRenderHost::onMouseWheel),
               FXMAPFUNC(SEL_PAINT, 0, FoxRenderHost::onPaint),
               FXMAPFUNC(SEL_CLIPBOARD_REQUEST, 0, FoxRenderHost::onClipboardRequest),
               FXMAPFUNC(SEL_SELECTION_REQUEST, 0, FoxRenderHost::onClipboardRequest),
               FXMAPFUNC(SEL_SELECTION_LOST, 0, FoxRenderHost::onSelectionLost),
               FXMAPFUNC(SEL_KEYPRESS, 0, FoxRenderHost::onKeyPress),
               FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_MIDDLEBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_RIGHTBUTTONPRESS, 0, FoxRenderHost::onButtonPress),
               FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, FoxRenderHost::onPointer),
               FXMAPFUNC(SEL_UNGRABBED, 0, FoxRenderHost::onUngrabbed),
               FXMAPFUNC(SEL_LEAVE, 0, FoxRenderHost::onLeave),
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
  programmatic = true;
  FXScrollArea::layout();
  programmatic = false;
  if (current) {
    transform.configure(*current, viewport_w, dpiScale, zoom);
    programmatic = true;
    FXScrollArea::layout();
    programmatic = false;
  }
  if (observedWidth != viewport_w || observedHeight != viewport_h) {
    observedWidth = viewport_w;
    observedHeight = viewport_h;
    if (geometryChanged)
      geometryChanged();
  }
  const double flowWidth = this->flowWidth();
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
  if (linkHovered)
    linkHovered("");
  clickCancelled = true;
  if (token.document != expected.document)
    current.reset();
  if (token != expected)
    selection.clear();
  expected = token;
  requested.reset();
  active = false;
  update();
}
void FoxRenderHost::expectLayout(FrameKey key) {
  if (linkHovered)
    linkHovered("");
  clickCancelled = true;
  if (key.token != expected)
    selection.clear();
  expected = key.token;
  requested = std::move(key);
  active = false;
  update();
}
void FoxRenderHost::present(LayoutResult frame) {
  if (!frame || frame->token != expected || (requested && !(frame->key == *requested)))
    return;
  if (frame->key.profile.mode == LayoutMode::Continuous &&
      std::abs(frame->width - flowWidth()) > .01)
    return;
  if (!current || current->token != frame->token)
    selection.clear();
  current = std::move(frame);
  active = true;
  transform.configure(*current, viewport_w, dpiScale, zoom);
  programmatic = true;
  FXScrollArea::layout();
  programmatic = false;
  recalc();
  update();
}
void FoxRenderHost::moveContents(FXint x, FXint y) {
  if (linkHovered)
    linkHovered("");
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
  if (linkHovered)
    linkHovered("");
  FoxWheelScrollBar::cancelTree(this);
  programmatic = true;
  lastScrollOrigin = origin;
  setPosition(pos_x, -int(std::lround(transform.toView({0, std::max(0.0, y)}).y)));
  programmatic = false;
}
void FoxRenderHost::setViewScale(double factor) {
  if (linkHovered)
    linkHovered("");
  if (!std::isfinite(factor))
    return;
  factor = std::clamp(factor, .05, 8.0);
  if (std::abs(zoom - factor) < .000001)
    return;
  auto before = transform.toDocument({-double(pos_x), -double(pos_y)});
  zoom = factor;
  if (current) {
    transform.configure(*current, viewport_w, dpiScale, zoom);
    programmatic = true;
    FXScrollArea::layout();
    programmatic = false;
    setViewport(before.y);
    if (current->key.profile.mode == LayoutMode::Continuous) {
      active = false;
      lastWidth = flowWidth();
      if (resized)
        resized(lastWidth);
    }
  }
  recalc();
  update();
}
void FoxRenderHost::setReadingColors(const ReadingColors& colors) {
  if (!colors.valid())
    return;
  reading = colors;
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
  const auto palette = ReadingPalette::from(reading);
  auto setColor = [&](std::uint32_t rgb) {
    cairo_set_source_rgb(cr, ((rgb >> 16) & 255) / 255.0, ((rgb >> 8) & 255) / 255.0,
                         (rgb & 255) / 255.0);
  };
  bool paged = current && current->key.profile.mode == LayoutMode::Paged;
  setColor(paged ? palette.surround : palette.background);
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
          setColor(palette.background);
          cairo_rectangle(cr, box.x + pos_x, box.y + pos_y, box.width, box.height);
          cairo_fill(cr);
          cairo_save(cr);
          cairo_translate(cr, box.x + pos_x, box.y + pos_y - i * paper.height * transform.scale);
          cairo_scale(cr, transform.scale, transform.scale);
          cairo_rectangle(cr, 0, i * paper.height, paper.width, paper.height);
          cairo_clip(cr);
          painter.paint(*current, cr, {0, i * paper.height, paper.width, paper.height}, active,
                        &palette);
          paintSelection(cr);
          cairo_restore(cr);
        }
      } else {
        cairo_translate(cr, pos_x, pos_y);
        cairo_scale(cr, transform.scale, transform.scale);
        auto top = transform.toDocument({-double(pos_x), -double(pos_y)});
        painter.paint(*current, cr,
                      {top.x, top.y, viewport_w / transform.scale, viewport_h / transform.scale},
                      active, &palette);
        paintSelection(cr);
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
} // namespace xfmd
