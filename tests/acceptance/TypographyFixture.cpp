#include "application/adapters/DisplayListPainter.h"
#include "application/adapters/SharedTextMetrics.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cairo-pdf.h>
#include <filesystem>
int main(int argc, char** argv) {
  try {
    CHECK(argc == 2);
    std::filesystem::path directory(argv[1]);
    xfmd::SharedTextMetrics metrics;
    xfmd::CmarkInterpreter parser;
    xfmd::MarkdownRenderer renderer;
    auto model = parser.parse(
        {{1, 1}, "# XFMD\n\nUnicode æøå office é 日本語\n\n**Bold** and `code`.\n", {}, false});
    auto frame = renderer.layout(*model, {595.276, 1}, metrics);
    for (const auto& run : frame->runs) {
      CHECK(run.shaped);
      double advance = 0;
      auto parts = run.shapeParts;
      parts.insert(parts.begin(), run.shaped);
      for (const auto& part : parts)
        for (const auto& segment : part->segments)
          for (const auto& glyph : segment.glyphs) {
            CHECK(glyph.cluster < segment.text.size());
            advance += glyph.advance;
          }
      CHECK(std::abs(advance - run.bounds.width) < .01);
    }
    auto pdf = (directory / "typography.pdf").string();
    auto png = (directory / "typography.png").string();
    auto* surface = cairo_pdf_surface_create(pdf.c_str(), 595.276, 841.89);
    auto* image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 596, 842);
    for (auto* target : {surface, image}) {
      auto* cr = cairo_create(target);
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_paint(cr);
      xfmd::DisplayListPainter painter(metrics.catalog);
      painter.paint(*frame, cr, {0, 0, 595.276, 841.89});
      CHECK(cairo_status(cr) == CAIRO_STATUS_SUCCESS);
      cairo_destroy(cr);
    }
    CHECK(cairo_surface_write_to_png(image, png.c_str()) == CAIRO_STATUS_SUCCESS);
    cairo_surface_finish(surface);
    CHECK(cairo_surface_status(surface) == CAIRO_STATUS_SUCCESS);
    cairo_surface_destroy(image);
    cairo_surface_destroy(surface);
    // A fresh, thread-local catalog can replay the same immutable frame.
    xfmd::FontCatalog other;
    CHECK(other.id() == metrics.catalog.id());
    for (const auto& run : frame->runs)
      for (const auto& segment : run.shaped->segments)
        CHECK(other.font(segment.fontFace));
    std::cout << "PASS glyph widths, clusters and cross-catalog font identity\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
