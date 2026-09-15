#include "DisplayListPainter.h"
#include "DiagramPainter.h"
#include "application/media/CairoVisual.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
namespace {
struct SavedState {
  cairo_t* cr;
  explicit SavedState(cairo_t* value) : cr(value) { cairo_save(cr); }
  ~SavedState() { cairo_restore(cr); }
};
void color(cairo_t* cr, std::uint32_t rgb) {
  cairo_set_source_rgb(cr, ((rgb >> 16) & 255) / 255.0, ((rgb >> 8) & 255) / 255.0,
                       (rgb & 255) / 255.0);
}
void rectangle(cairo_t* cr, Rect r) {
  cairo_rectangle(cr, r.x, r.y, r.width, r.height);
  cairo_fill(cr);
}
bool visible(Rect r, Rect clip) {
  return r.y + r.height >= clip.y && r.y <= clip.y + clip.height && r.x + r.width >= clip.x &&
         r.x <= clip.x + clip.width;
}
} // namespace
void DisplayListPainter::text(cairo_t* cr, const RenderFrame& frame, const DrawRun& draw) {
  if (!draw.shaped)
    return;
  SavedState saved(cr);
  cairo_translate(cr, draw.bounds.x, draw.bounds.y);
  cairo_scale(cr, draw.textScale, draw.textScale);
  double x = 0;
  auto drawPart = [&](const ShapedText& part) {
    for (const auto& segment : part.segments) {
      PangoItem item{};
      item.length = int(segment.text.size());
      item.num_chars = int(g_utf8_strlen(segment.text.data(), item.length));
      item.analysis.font = fonts.font(segment.fontFace);
      item.analysis.level = segment.level;
      auto* glyphs = pango_glyph_string_new();
      pango_glyph_string_set_size(glyphs, int(segment.glyphs.size()));
      for (std::size_t i = 0; i < segment.glyphs.size(); ++i) {
        const auto& value = segment.glyphs[i];
        auto& g = glyphs->glyphs[i];
        g.glyph = value.index;
        g.geometry.width = std::lround(value.advance * PANGO_SCALE);
        g.geometry.x_offset = std::lround(value.xOffset * PANGO_SCALE);
        g.geometry.y_offset = std::lround(value.yOffset * PANGO_SCALE);
        g.attr.is_cluster_start = i == 0 || value.cluster != segment.glyphs[i - 1].cluster;
        glyphs->log_clusters[i] = value.cluster;
      }
      PangoGlyphItem run{};
      run.item = &item;
      run.glyphs = glyphs;
      cairo_move_to(cr, x, draw.ascent / draw.textScale);
      pango_cairo_show_glyph_item(cr, segment.text.c_str(), &run);
      for (const auto& glyph : segment.glyphs)
        x += glyph.advance;
      pango_glyph_string_free(glyphs);
    }
  };
  drawPart(*draw.shaped);
  for (std::size_t i = 0; i < draw.shapeCount; ++i)
    drawPart(*frame.shapeParts.at(draw.shapeBegin + i));
}
void DisplayListPainter::paint(const RenderFrame& frame, cairo_t* cr, Rect clip, bool active,
                               const ReadingPalette* palette) {
  SavedState saved(cr);
  cairo_rectangle(cr, clip.x, clip.y, clip.width, clip.height);
  cairo_clip(cr);
  for (const auto& decoration : frame.decorations)
    if (visible(decoration.bounds, clip)) {
      color(cr, palette ? palette->decoration(decoration.role) : decoration.color);
      rectangle(cr, decoration.bounds);
    }
  auto first = std::lower_bound(frame.runs.begin(), frame.runs.end(), clip.y - frame.maxRunHeight,
                                [](const DrawRun& run, double y) { return run.bounds.y < y; });
  for (; first != frame.runs.end() && first->bounds.y < clip.y + clip.height; ++first) {
    const auto& run = *first;
    if (!visible(run.bounds, clip))
      continue;
    if (run.codeBackground) {
      color(cr, palette ? palette->surface : 0xeff1f5);
      rectangle(cr, run.bounds);
    }
    color(cr, palette            ? (active ? palette->text : palette->inactive)
              : !active          ? 0x878787
              : run.link.empty() ? 0x1d2531
                                 : 0x1855a6);
    if (const auto* diagram = dynamic_cast<const DiagramScene*>(run.visual.get())) {
      DiagramPainter::paint(cr, *diagram, run.bounds, palette, active);
    } else if (const auto* visual = dynamic_cast<const CairoVisual*>(run.visual.get())) {
      SavedState resourceState(cr);
      cairo_translate(cr, run.bounds.x, run.bounds.y);
      const double w =
          visual->monochrome ? visual->width : cairo_image_surface_get_width(visual->surface);
      const double h =
          visual->monochrome ? visual->height : cairo_image_surface_get_height(visual->surface);
      cairo_scale(cr, run.bounds.width / w, run.bounds.height / h);
      if (visual->monochrome)
        cairo_mask_surface(cr, visual->surface, 0, 0);
      else {
        cairo_set_source_surface(cr, visual->surface, 0, 0);
        cairo_paint(cr);
      }
    } else
      text(cr, frame, run);
    if (!run.visual && !run.link.empty() && run.icon == InlineIcon::None) {
      cairo_set_line_width(cr, .6);
      cairo_move_to(cr, run.bounds.x, run.bounds.y + run.ascent + 2);
      cairo_line_to(cr, run.bounds.x + run.bounds.width, run.bounds.y + run.ascent + 2);
      cairo_stroke(cr);
    }
  }
}
} // namespace xfmd
