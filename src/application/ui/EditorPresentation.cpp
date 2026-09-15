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
void EditorWidget::setViewProfile(const LayoutProfile& profile, bool fit) {
  if (viewProfile == profile && fitPage == fit)
    return;
  presentationDirty = true;
  viewProfile = profile;
  fitPage = fit;
  recalc();
}
void EditorWidget::applyViewProfile() {
  if (!baseFont)
    baseFont = getFont();
  FXFontDesc desc{};
  baseFont->getFontDesc(desc);
  const bool paged = viewProfile.mode == LayoutMode::Paged;
  const double paperWidth = viewProfile.paper.width * 4 / 3;
  const double scale =
      paged && fitPage ? std::clamp((getWidth() - 32.0) / paperWidth, .25, 8.0) : 1;
  const int margin = paged ? int(std::lround(viewProfile.paper.margin * 4 / 3 * scale)) + 16 : 12;
  const double textWidth = (viewProfile.paper.width - 2 * viewProfile.paper.margin) * 4 / 3;
  const int columns = std::max(1, int(textWidth / std::max(1, baseFont->getTextWidth("x", 1))));
  int size = std::max(30, int(std::lround(desc.size * scale)));
  if (paged && fitPage) {
    // FOX rounds monospace glyph advances to pixels. Fit complete columns plus
    // margins and the vertical scrollbar, rather than overflowing by a few pixels.
    const int cell =
        std::max(1, (getWidth() - verticalScrollBar()->getDefaultWidth() - 2 * margin) / columns);
    size = std::min(
        size, std::max(30, int(desc.size) * cell / std::max(1, baseFont->getTextWidth("x", 1))));
  }
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
  presentationWidth = getWidth();
  presentationDirty = false;
  layingOut = false;
}
} // namespace xfmd
