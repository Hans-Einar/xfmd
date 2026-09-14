#pragma once
#include "FontCatalog.h"
#include "application/preferences/ReadingColors.h"
#include "contracts/RenderFrame.h"
namespace xfmd {
class DisplayListPainter {
  FontCatalog& fonts;
  void text(cairo_t*, const RenderFrame&, const DrawRun&);

public:
  explicit DisplayListPainter(FontCatalog& value) : fonts(value) {}
  void paint(const RenderFrame&, cairo_t*, Rect clip, bool active = true,
             const ReadingPalette* palette = nullptr);
};
} // namespace xfmd
