#include "EditorWidget.h"
#include <algorithm>
#include <cmath>
using namespace FX;
namespace xfmd {
void EditorWidget::setReadingColors(const ReadingColors& colors) {
  if (!colors.valid())
    return;
  auto p = ReadingPalette::from(colors);
  auto native = [](std::uint32_t c) { return FXRGB((c >> 16) & 255, (c >> 8) & 255, c & 255); };
  setBackColor(native(p.background));
  setTextColor(native(p.text));
  setCursorColor(native(p.text));
  setActiveBackColor(native(p.background));
  update();
}
void EditorWidget::setViewProfile(const LayoutProfile& profile, double factor) {
  if (!std::isfinite(factor) || factor <= 0)
    return;
  if (viewProfile == profile && viewScale == factor)
    return;
  presentationDirty = true;
  viewProfile = profile;
  viewScale = factor;
  recalc();
}
void EditorWidget::applyViewProfile() {
  if (!baseFont)
    baseFont = getFont();
  FXFontDesc desc{};
  baseFont->getFontDesc(desc);
  const bool paged = viewProfile.mode == LayoutMode::Paged;
  const double dpi =
      std::clamp(getApp()->reg().readRealEntry("SETTINGS", "screenres", 96.0) / 72.0, .5, 4.0);
  const double scale = viewScale;
  const int margin = paged ? int(std::lround(viewProfile.paper.margin * dpi * scale)) + 16 : 12;
  const double textWidth = (viewProfile.paper.width - 2 * viewProfile.paper.margin) * dpi;
  const int columns = std::max(1, int(textWidth / std::max(1, baseFont->getTextWidth("x", 1))));
  int size = std::max(10, int(std::lround(desc.size * scale)));
  if (getFont()->getSize() != FXuint(size)) {
    desc.size = size;
    auto font = std::make_unique<FXFont>(getApp(), desc);
    font->create();
    setFont(font.get());
    scaledFont = std::move(font);
  }
  auto style = getTextStyle() | TEXT_WORDWRAP;
  if (paged) {
    style |= TEXT_FIXEDWRAP;
    setWrapColumns(columns);
  } else
    style &= ~TEXT_FIXEDWRAP;
  setTextStyle(style);
  setMarginLeft(margin);
  setMarginRight(margin);
}
void EditorWidget::layout() {
  if (layingOut)
    return;
  layingOut = true;
  const bool changed = presentationDirty ||
                       (viewProfile.mode == LayoutMode::Paged && presentationWidth != getWidth());
  if (changed) {
    bool old = scrolling;
    scrolling = true;
    auto anchor = sourceAnchor();
    applyViewProfile();
    FXText::layout();
    setSourceAnchor({anchor});
    scrolling = old;
  } else
    FXText::layout();
  const bool geometry = observedViewportWidth != viewport_w || observedViewportHeight != viewport_h;
  observedViewportWidth = viewport_w;
  observedViewportHeight = viewport_h;
  presentationHeight = getHeight();
  presentationWidth = getWidth();
  presentationDirty = false;
  layingOut = false;
  if (geometry && geometryChanged)
    geometryChanged();
}
} // namespace xfmd
