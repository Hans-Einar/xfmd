#include "application/adapters/DisplayListPainter.h"
#include "application/adapters/SharedTextMetrics.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
#include <set>
using namespace xfmd;
void run() {
  CHECK(ReadingColors::rgb(-1, 100) == 0xffffff);
  CHECK(ReadingColors::rgb(60, 100) == 0xffff00);
  CHECK(ReadingColors::rgb(120, 100) == 0x00ff00);
  CHECK(ReadingColors::rgb(240, 100) == 0x0000ff);
  CHECK(ReadingColors::rgb(100, 0) == 0);
  CHECK(!ReadingColors{-2, 10, -1, 90}.valid());
  CHECK(!ReadingColors{-1, 101, -1, 90}.valid());
  CHECK(ReadingPalette::from(ReadingColors::defaults(true)).contrast() > 7);
  CHECK(ReadingPalette::from(ReadingColors::defaults(false)).contrast() > 7);
  CHECK(std::abs(ReadingPalette::from({-1, 0, -1, 100}).contrast() - 21) < .001);
  CHECK(ReadingPalette::from({-1, 0, 60, 100}).contrast() < 21);
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  SharedTextMetrics metrics;
  auto model = parser.parse({{1, 1},
                             "# Heading\n\nText `inline` [link](a.md).\n\n```\ncode\n```\n\n> "
                             "Quote\n\n| A | B |\n|---|---|\n| c | d |\n| e | f |\n",
                             {},
                             false});
  auto continuous = renderer.layout(*model, {600, 1}, metrics);
  LayoutProfile profile;
  profile.mode = LayoutMode::Paged;
  auto paged = renderer.layout(*model, {600, 2, profile}, metrics);
  for (const auto& frame : {continuous, paged}) {
    std::set<DecorationRole> roles;
    for (auto& d : frame->decorations)
      roles.insert(d.role);
    CHECK(roles.size() == 4);
  }
  DisplayListPainter painter(metrics.catalog);
  auto palette = ReadingPalette::from({-1, 10, 60, 90});
  auto* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 650, 850);
  auto* cr = cairo_create(surface);
  auto pixel = [&](int x, int y) {
    cairo_surface_flush(surface);
    auto* row = reinterpret_cast<std::uint32_t*>(cairo_image_surface_get_data(surface) +
                                                 y * cairo_image_surface_get_stride(surface));
    return row[x] & 0xffffff;
  };
  RenderFrame rectangles;
  rectangles.decorations = {{{0, 0, 20, 20}, 0xf0f2f5, DecorationRole::Surface},
                            {{20, 0, 20, 20}, 0xffffff, DecorationRole::Background},
                            {{40, 0, 20, 20}, 0xb8bec7, DecorationRole::Border}};
  painter.paint(rectangles, cr, {0, 0, 100, 100}, true, &palette);
  CHECK(pixel(5, 5) == palette.surface && pixel(25, 5) == palette.background);
  CHECK(pixel(45, 5) == palette.border);
  // PDF's unchanged default call uses original colors, regardless of earlier screen paints.
  painter.paint(rectangles, cr, {0, 0, 100, 100});
  CHECK(pixel(5, 5) == 0xf0f2f5 && pixel(25, 5) == 0xffffff && pixel(45, 5) == 0xb8bec7);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_paint(cr);
  painter.paint(*continuous, cr, {0, 0, 650, 850}, true, &palette);
  int textPixels = 0;
  for (int y = 0; y < 850; ++y)
    for (int x = 0; x < 650; ++x)
      textPixels += pixel(x, y) == palette.text;
  CHECK(textPixels > 100);
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
TEST_MAIN(run)
