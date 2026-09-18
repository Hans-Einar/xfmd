#include "application/boxui/BoxUiPreparation.h"
#include "application/composition/DiagramServices.h"
#include "contracts/boxui/BoxUiAbi.h"
#include "renderer/MarkdownRenderer.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
#include "support/TestSupport.h"
#include <fstream>
using namespace xfmd;
void run() {
  std::ifstream f(XFMD_BOXUI_FIXTURE);
  std::string json((std::istreambuf_iterator<char>(f)), {});
  auto text = "Before\n\n```boxui\nboxui 0.1\n" + json + "\n```\n\nAfter\n";
  SourceSnapshot source{{77, 1}, text, {}, false};
  auto parser = DiagramServices::interpreter();
  SharedTextMetrics metrics;
  auto parsed = parser->parse(source);
  CHECK(parsed->blocks.size() == 3);
  CHECK(parsed->blocks[1].boxUi && parsed->blocks[1].boxUi->children.size() == 1);
  CHECK(parsed->blocks[1].boxUi->children[0].model);
  auto prepared = DiagramServices::prepare(parsed, source, metrics, [] { return false; });
  if (!prepared->blocks[1].boxUiFrame)
    throw std::runtime_error(prepared->blocks[1].runs.at(0).text);
  auto frame = prepared->blocks[1].boxUiFrame;
  CHECK(frame->controls.size() == 3 && frame->diagnostics.empty());
  for (auto& c : frame->controls)
    CHECK(!c.enabled);
  CHECK(frame->staticScene->svg.find("Running") != std::string::npos);
  CHECK(frame->staticScene->svg != frame->previewScene->svg);
  MarkdownRenderer renderer;
  for (double width : {320., 600., 1200.}) {
    auto laid = renderer.layout(*prepared, {width, 1}, metrics);
    unsigned boxes = 0;
    for (auto& r : laid->runs)
      if (dynamic_cast<const BoxUiFrame*>(r.visual.get())) {
        ++boxes;
        CHECK(r.bounds.width <= width);
      }
    CHECK(boxes == 1);
  }
  source.text = text + text;
  auto duplicate = parser->parse(source);
  for (auto& b : duplicate->blocks)
    CHECK(!b.boxUi);
  source.text = text;
  auto pos = source.text.find("```boxui");
  source.text.replace(pos, 8, "```mermaid");
  CHECK(parser->parse(source)->blocks[1].boxUi);
  source.text = text;
  pos = source.text.find("\"input\"");
  source.text.replace(pos, 7, "\"javascript\"");
  auto invalid = parser->parse(source);
  CHECK(!invalid->blocks[1].boxUi);
  CHECK(invalid->blocks[1].runs[0].text.find("BoxUI:") == 0);
  for (unsigned i = 0; i < 64; ++i) {
    auto result = xfmd_boxui_parse_v1(2, nullptr, 0);
    CHECK(result.status != 0 && result.owner);
    xfmd_boxui_result_free_v1(&result);
    CHECK(!result.owner);
    xfmd_boxui_result_free_v1(&result);
  }
  std::string fixture = XFMD_BOXUI_FIXTURE;
  std::ifstream childFile(fixture.substr(0, fixture.find_last_of('/')) + "/children.md");
  SourceSnapshot mixed{
      {99, 1}, std::string((std::istreambuf_iterator<char>(childFile)), {}), {}, false};
  auto nested =
      DiagramServices::prepare(parser->parse(mixed), mixed, metrics, [] { return false; });
  unsigned childCount = 0;
  for (auto& block : nested->blocks)
    if (block.boxUi) {
      CHECK(block.boxUiFrame && block.boxUiFrame->diagnostics.empty());
      CHECK(block.boxUi->children.size() == 1 && block.boxUi->children[0].model);
      ++childCount;
    }
  CHECK(childCount == 3);
  bool cancelled = false;
  try {
    MermaidDiagramLayout diagrams;
    BoxUiPreparation::prepare(parsed, metrics, diagrams, {}, [] { return true; });
  } catch (const std::exception&) {
    cancelled = true;
  }
  CHECK(cancelled);
}
TEST_MAIN(run)
