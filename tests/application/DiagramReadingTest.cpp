#include "application/adapters/DiagramPainter.h"
#include "application/adapters/DisplayListPainter.h"
#include "application/composition/DiagramServices.h"
#include "application/preview/PreviewSelection.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
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
  const DrawRun* picture = nullptr;
  for (const auto& run : frame->runs) {
    CHECK(run.text != "Blåbær" && run.text != "Ready");
    if (run.visual)
      picture = &run;
  }
  CHECK(picture && picture->source.quality == MappingQuality::Approximate);
  CHECK(frame->readingText.find("Blåbær") == std::string::npos);
  CHECK(model->blocks[0].diagramScene->svg.find("Blåbær") != std::string::npos);
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
  }
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  auto paged = renderer.layout(*model, {595, 2, {LayoutMode::Paged, {}}}, metrics);
  CHECK(paged->pages.slices.size() == 1);
  CHECK(paged->readingText == frame->readingText);
  for (const auto* name :
       {"apt-import", "sequence-fragments", "sequence-nested", "measurement-state", "state-regions",
        "state-choice", "sdl-class", "apt-requirements", "provenance-er", "c4-context", "c4-container", "c4-component", "architecture-resources", "block-layers"}) {
    std::ifstream file(std::string(XFMD_SEQUENCE_FIXTURES) + "/" + name + ".mmd");
    SourceSnapshot sequence{{9, 1},
                            "Before\n\n```mermaid\n" +
                                std::string((std::istreambuf_iterator<char>(file)), {}) +
                                "\n```\n\nAfter",
                            {},
                            false};
    auto prepared =
        DiagramServices::prepare(parser->parse(sequence), sequence, metrics, [] { return false; });
    std::shared_ptr<const DiagramScene> scene;
    for (const auto& block : prepared->blocks)
      if (block.diagramScene)
        scene = block.diagramScene;
    CHECK(scene && !scene->diagnostics.empty());
    for (const auto width : {320., 900.}) {
      auto view = renderer.layout(*prepared, {width, 1}, metrics);
      CHECK(view->readingText.find("Before") != std::string::npos);
      for (const auto& run : view->runs)
        if (run.visual) {
          CHECK(run.visual.get() == scene.get());
          CHECK(std::abs(run.bounds.width / run.bounds.height - scene->width / scene->height) <
                .001);
        }
    }
    unsigned long long previousHash = 0;
    for (bool dark : {false, true}) {
      const int width = 1000, height = std::ceil(width * scene->height / scene->width);
      auto* image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
      auto* canvas = cairo_create(image);
      auto palette = ReadingPalette::from(ReadingColors::defaults(dark));
      DiagramPainter::paint(canvas, *scene, {0, 0, double(width), double(height)}, &palette, true);
      CHECK(cairo_status(canvas) == CAIRO_STATUS_SUCCESS);
      cairo_surface_flush(image);
      unsigned long long hash = 1469598103934665603ULL;
      const auto* bytes = cairo_image_surface_get_data(image);
      for (int k = 0; k < height * cairo_image_surface_get_stride(image); ++k)
        hash = (hash ^ bytes[k]) * 1099511628211ULL;
      CHECK(hash != previousHash);
      previousHash = hash;
      if (const auto* directory = std::getenv("XFMD_DIAGRAM_EVIDENCE")) {
        auto output = std::string(directory) + "/" + name + (dark ? "-dark.png" : "-light.png");
        CHECK(cairo_surface_write_to_png(image, output.c_str()) == CAIRO_STATUS_SUCCESS);
      }
      cairo_destroy(canvas);
      cairo_surface_destroy(image);
    }
  }
  SourceSnapshot unsupported{{9, 2},
                             "Before\n\n```mermaid\nsequenceDiagram\nparticipant A\nparticipant "
                             "B\nA-xB: lost\n```\n\nAfter",
                             {},
                             false};
  auto fallback = DiagramServices::prepare(parser->parse(unsupported), unsupported, metrics,
                                           [] { return false; });
  auto fallbackFrame = renderer.layout(*fallback, {600, 1}, metrics);
  CHECK(fallbackFrame->readingText.find("A-xB: lost") != std::string::npos);
  CHECK(fallbackFrame->readingText.find("Mermaid:") != std::string::npos);
  CHECK(fallbackFrame->readingText.find("After") != std::string::npos);
}
TEST_MAIN(run)
