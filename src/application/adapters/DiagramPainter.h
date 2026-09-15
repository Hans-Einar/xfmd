#pragma once
#include "application/preferences/ReadingColors.h"
#include "contracts/diagram/DiagramScene.h"
#include <cairo.h>
namespace xfmd {
class DiagramPainter {
public:
  static void paint(cairo_t*, const DiagramScene&, Rect, const ReadingPalette*, bool);
};
} // namespace xfmd
