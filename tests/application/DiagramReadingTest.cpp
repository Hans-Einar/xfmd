#include "application/adapters/DisplayListPainter.h"
#include "application/composition/DiagramServices.h"
#include "application/preview/PreviewSelection.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
using namespace xfmd;
void run() {
  auto parser = DiagramServices::interpreter();
  SharedTextMetrics metrics;
  MarkdownRenderer renderer;
  SourceSnapshot source{
      {4, 1}, "```mermaid\nflowchart LR\nA[Blåbær] -->|Ready| B{Ærlig}\n```", {}, false};
  auto model =
      DiagramServices::prepare(parser->parse(source), source, metrics, [] { return false; });
  CHECK(model->blocks[0].diagramScene);
  auto frame = renderer.layout(*model, {180, 1}, metrics);
  const DrawRun* label = nullptr;
  const DrawRun* picture = nullptr;
  for (const auto& run : frame->runs) {
    if (run.text == "Blåbær")
      label = &run;
    if (run.visual)
      picture = &run;
  }
  CHECK(label && picture && label->textScale < 1);
  PreviewSelection selection;
  selection.start(
      PreviewSelection::hit(*frame, {label->bounds.x, label->bounds.y + label->bounds.height / 2}));
  selection.extend(PreviewSelection::hit(
      *frame, {label->bounds.x + label->bounds.width, label->bounds.y + label->bounds.height / 2}));
  CHECK(selection.text(*frame) == "Blåbær");
  auto rectangles = selection.rectangles(*frame);
  double width = 0;
  for (auto box : rectangles)
    width += box.width;
  CHECK(std::abs(width - label->bounds.width) < .01);
  CHECK(label->source.quality == MappingQuality::Approximate && label->source.begin == 0);
  auto* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 180, 300);
  auto* cr = cairo_create(surface);
  DisplayListPainter painter(metrics.catalog);
  const auto& scene = *model->blocks[0].diagramScene;
  double scale = picture->bounds.width / scene.width;
  int x = int(picture->bounds.x + (scene.nodes[0].bounds.x + 5) * scale);
  int y = int(picture->bounds.y + (scene.nodes[0].bounds.y + 5) * scale);
  std::uint32_t previous = 0;
  for (auto colors : {ReadingColors::defaults(false), ReadingColors::defaults(true),
                      ReadingColors{-1, 12, 60, 85}}) {
    auto palette = ReadingPalette::from(colors);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);
    painter.paint(*frame, cr, {0, 0, 180, 300}, true, &palette);
    cairo_surface_flush(surface);
    auto* row = reinterpret_cast<const std::uint32_t*>(cairo_image_surface_get_data(surface) +
                                                       y * cairo_image_surface_get_stride(surface));
    CHECK((row[x] & 0xffffff) == palette.surface);
    CHECK((row[x] & 0xffffff) != previous);
    previous = row[x] & 0xffffff;
    CHECK(selection.text(*frame) == "Blåbær");
  }
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  auto paged = renderer.layout(*model, {595, 2, {LayoutMode::Paged, {}}}, metrics);
  CHECK(paged->pages.slices.size() == 1);
  CHECK(paged->readingText == frame->readingText);
}
TEST_MAIN(run)
