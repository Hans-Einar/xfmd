#include "application/diagrams/DiagramPreparation.h"
#include "application/adapters/SharedTextMetrics.h"
#include "application/composition/DiagramServices.h"
#include "interpreter/CmarkInterpreter.h"
#include "interpreter/mermaid/MermaidInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
#include "support/TestSupport.h"
using namespace xfmd;
struct CountingLayout : IDiagramLayout {
  unsigned calls = 0;
  bool fail = false, invalidSvg = false;
  MermaidDiagramLayout real;
  std::shared_ptr<const DiagramScene> layout(const DiagramModel& model,
                                             const DiagramLayoutRequest& request,
                                             ITextMetrics& metrics) override {
    ++calls;
    if (fail)
      throw std::runtime_error("Injected failure");
    auto value = real.layout(model, request, metrics);
    if (invalidSvg) {
      auto broken = std::make_shared<DiagramScene>(*value);
      broken->svg = "<svg malformed";
      return broken;
    }
    return value;
  }
};
struct FontAlias : ITextMetrics {
  ITextMetrics& base;
  FontSetId id;
  explicit FontAlias(ITextMetrics& value) : base(value), id(value.fontSetId() + 1) {}
  FontSetId fontSetId() const override { return id; }
  TextExtent measure(std::string_view text, FontSpec font) override {
    return base.measure(text, font);
  }
};
void run() {
  auto parser = DiagramServices::interpreter();
  SharedTextMetrics metrics;
  CountingLayout layout;
  DiagramPreparation prepare(layout);
  SourceSnapshot source{
      {3, 1},
      "Before\n\n~~~mermaid extra\nflowchart LR\nA[Blåbær] -->|Target| B{Ready}\n~~~\n\nAfter",
      {},
      false};
  auto parsed = parser->parse(source);
  CHECK(parsed->blocks.size() == 3 && parsed->blocks[1].diagram);
  CHECK(parsed->blocks[1].source.begin == 8);
  auto model = prepare.prepare(parsed, metrics);
  CHECK(model->blocks[1].diagramScene && layout.calls == 1);
  source.text = "Offset\n\n" + source.text;
  source.token.revision++;
  auto second = prepare.prepare(parser->parse(source), metrics);
  CHECK(second->blocks[2].diagramScene == model->blocks[1].diagramScene && layout.calls == 1);
  CHECK(second->blocks[2].source.begin != model->blocks[1].source.begin);
  MarkdownRenderer renderer;
  auto wide = renderer.layout(*model, {600, 1}, metrics);
  auto narrow = renderer.layout(*model, {140, 2}, metrics);
  CHECK(narrow->readingText.find("Blåbær") == std::string::npos);
  CHECK(model->blocks[1].diagramScene->svg.find("Blåbær") != std::string::npos);
  CHECK(layout.calls == 1);
  bool scaled = false;
  for (const auto& run : narrow->runs) {
    CHECK(run.bounds.x + run.bounds.width <= 141);
    scaled |= run.visual && run.bounds.width < run.visual->width;
  }
  CHECK(scaled);
  bool cancelled = false;
  try {
    prepare.prepare(parsed, metrics, [] { return true; });
  } catch (const std::exception&) {
    cancelled = true;
  }
  CHECK(cancelled);
  FontAlias alternate(metrics);
  auto changed = prepare.prepare(parsed, alternate);
  CHECK(changed->blocks[1].diagramScene != model->blocks[1].diagramScene);
  CHECK(changed->blocks[1].diagramScene->fonts == alternate.fontSetId());
  bool conflict = false;
  try {
    renderer.layout(*model, {600, 3}, alternate);
  } catch (const Error&) {
    conflict = true;
  }
  CHECK(conflict);
  DiagramCache cache;
  auto large = std::make_shared<DiagramScene>();
  large->bytes = 20 * 1024 * 1024;
  cache.insert("first", large);
  cache.insert("second", large);
  CHECK(!cache.find("first") && cache.find("second"));
  CHECK(cache.bytes() <= 32 * 1024 * 1024);
  layout.fail = true;
  source.text = "```mermaid\nflowchart TD\nX-->Y\n```";
  auto failed = prepare.prepare(parser->parse(source), metrics);
  CHECK(failed->blocks[0].kind == BlockKind::Code && !failed->blocks[0].diagramScene);
  CHECK(failed->blocks[0].runs[0].text.find("Injected failure") != std::string::npos);
  layout.fail = false;
  layout.invalidSvg = true;
  auto invalidSvg = prepare.prepare(parser->parse(source), metrics);
  CHECK(invalidSvg->blocks[0].kind == BlockKind::Code && !invalidSvg->blocks[0].diagramScene);
  CHECK(invalidSvg->blocks[0].runs[0].text.find("Mermaid:") != std::string::npos);
  source.text = "```mermaid\nsequenceDiagram\nA->>B: message\n```";
  auto unsupported = parser->parse(source);
  CHECK(unsupported->blocks[0].kind == BlockKind::Code && !unsupported->blocks[0].diagram);
  CHECK(unsupported->blocks[0].runs.back().text.find("sequenceDiagram") != std::string::npos);
  source.text = "flowchart LR\nA-->B";
  CHECK(!parser->parse(source)->blocks[0].diagram);
  source.text.clear();
  for (int i = 0; i < 17; ++i)
    source.text += "```mermaid\nflowchart LR\nA-->B\n```\n\n";
  auto limit = parser->parse(source);
  CHECK(limit->blocks[15].diagram && !limit->blocks[16].diagram);
}
TEST_MAIN(run)
