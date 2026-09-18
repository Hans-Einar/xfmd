#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "contracts/diagram/DiagramModel.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <fstream>
using namespace xfmd;
int main(int argc, char** argv) {
  if (argc != 3)
    return 2;
  try {
    SourceSnapshot source{{7, 3}, "# Mermaid export\n\n", {}, false};
    for (auto name : {"service-map", "layer-delivery", "traceability", "apt-import"}) {
      std::ifstream file(std::string(XFMD_DIAGRAM_FIXTURES) + "/" + name + ".mmd");
      source.text +=
          "```mermaid\n" + std::string((std::istreambuf_iterator<char>(file)), {}) + "\n```\n\n";
    }
    source.text += "```mermaid\nflowchart TD\nA[Blåbær] --> B{Ærlig måling}\n```\n";
    for (int i = 0; i < 50; ++i)
      source.text += "\n\nParagraph " + std::to_string(i) + ".";
    source.text += "\n\n```mermaid\nflowchart TD\n";
    for (int i = 0; i < 11; ++i)
      source.text += "N" + std::to_string(i) + "[Stage " + std::to_string(i) + "] --> N" +
                     std::to_string(i + 1) + "[Stage " + std::to_string(i + 1) + "]\n";
    source.text += "```\n\nAfter the tall diagram.\n";
    auto parser = DiagramServices::interpreter();
    SharedTextMetrics metrics;
    MarkdownRenderer renderer;
    auto target = PdfTarget::inspect(argv[1]);
    ExportControl control;
    ExportRequest request{source, {}, metrics.fontSetId(), target.path, {}};
    auto pages = ExportPipeline::run(request, target, control, *parser, renderer, metrics);
    CHECK(pages >= 3);
    auto model =
        DiagramServices::prepare(parser->parse(source), source, metrics, [] { return false; });
    unsigned diagrams = 0;
    for (const auto& b : model->blocks)
      diagrams += bool(b.diagramScene);
    CHECK(diagrams == 6);
    auto frame = renderer.layout(*model, {595, 1, {LayoutMode::Paged, {}}}, metrics);
    for (const auto& run : frame->runs) {
      auto page = std::size_t(run.bounds.y / frame->pages.paper.height);
      CHECK(run.bounds.y + run.bounds.height <= page * frame->pages.paper.height +
                                                    frame->pages.paper.height -
                                                    frame->pages.paper.margin + .1);
    }
    for (const auto& visual : frame->runs)
      if (visual.visual) {
        for (const auto& label : frame->runs)
          if (label.textBegin != std::string::npos && label.source.begin == visual.source.begin)
            CHECK(std::size_t(label.bounds.y / frame->pages.paper.height) ==
                  std::size_t(visual.bounds.y / frame->pages.paper.height));
      }
    std::ofstream expected(argv[2]);
    expected << frame->readingText << "\n";
    // SVG text must still survive vector PDF export although GUI label
    // selection is deliberately deferred.
    for (const auto& b : model->blocks)
      if (b.diagramScene)
        for (const auto& label : b.diagramScene->labels)
          expected << label.text << "\n";
    for (const auto& block : model->blocks)
      if (block.diagram && block.diagram->sequence) {
        for (const auto& participant : block.diagram->sequence->participants)
          expected << participant.label << "\n";
        for (const auto& event : block.diagram->sequence->events)
          if (!event.text.empty())
            expected << event.text << "\n";
      }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
