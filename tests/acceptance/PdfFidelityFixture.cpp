#include "application/adapters/DisplayListPainter.h"
#include "application/export/ExportPipeline.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
#include <filesystem>
#include <fstream>
using namespace xfmd;
int main(int argc, char** argv) {
  try {
    CHECK(argc == 2);
    std::filesystem::path dir(argv[1]);
    SharedTextMetrics metrics;
    CmarkInterpreter parser;
    MarkdownRenderer renderer;
    std::string text = "# PDF Fidelity æøå office é 日本語\n\n";
    for (int i = 0; i < 45; ++i)
      text += "## Marker" + std::to_string(i) +
              "\n\nA paragraph with **bold**, *italic* and `code` with a "
              "[link](local.md).\n\n```cpp\nint value = 42; // a code line\n```\n\n";
    text += "| TableHeader | Center | Right |\n|:---|:---:|---:|\n";
    for (int i = 0; i < 80; ++i)
      text += "| TableRow" + std::to_string(i) +
              " | æøå **bold** [web](https://example.org) | 1234 |\n";
    text += "\nENDOFFROZENBUFFER\n";
    ExportRequest request{
        {{7, 11}, text, {}, false}, {}, metrics.fontSetId(), (dir / "export.pdf").string(), {}};
    request.frame = renderer.layout(*parser.parse(request.source),
                                    {600, 1, {LayoutMode::Paged, request.paper}}, metrics);
    ExportControl control;
    auto count = ExportPipeline::run(request, PdfTarget::inspect(request.target), control, parser,
                                     renderer, metrics);
    std::ofstream(dir / "count") << count;
    for (unsigned page = 0; page < count; ++page) {
      int width = std::ceil(request.paper.width * 96 / 72),
          height = std::ceil(request.paper.height * 96 / 72);
      auto* image = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
      auto* cr = cairo_create(image);
      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_paint(cr);
      cairo_scale(cr, 96.0 / 72, 96.0 / 72);
      cairo_translate(cr, 0, -double(page) * request.paper.height);
      DisplayListPainter painter(metrics.catalog);
      painter.paint(*request.frame, cr,
                    {0, page * request.paper.height, request.paper.width, request.paper.height});
      cairo_surface_flush(image);
      std::ofstream ppm(dir / ("screen-" + std::to_string(page + 1) + ".ppm"), std::ios::binary);
      ppm << "P6\n" << width << " " << height << "\n255\n";
      auto* bytes = cairo_image_surface_get_data(image);
      int stride = cairo_image_surface_get_stride(image);
      for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
          auto pixel = reinterpret_cast<std::uint32_t*>(bytes + y * stride)[x];
          char rgb[] = {char(pixel >> 16), char(pixel >> 8), char(pixel)};
          ppm.write(rgb, 3);
        }
      cairo_destroy(cr);
      cairo_surface_destroy(image);
    }
    std::cout << count << " pages from shared immutable frame\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
