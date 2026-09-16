#include "application/adapters/SharedTextMetrics.h"
#include "contracts/diagram/DiagramLimits.h"
#include "contracts/diagram/DiagramWire.h"
#include "interpreter/mermaid/MermaidInterpreter.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
#include "support/TestSupport.h"
#include <chrono>
#include <fstream>
#include <iostream>
using namespace xfmd;
void run() {
  std::cout << std::unitbuf;
  MermaidInterpreter parser;
  MermaidDiagramLayout layout;
  SharedTextMetrics metrics;
  for (auto name : {"traceability", "service-map", "layer-delivery"}) {
    std::ifstream file(std::string(XFMD_DIAGRAM_FIXTURES) + "/" + name + ".mmd");
    std::string text((std::istreambuf_iterator<char>(file)), {});
    auto parsed = parser.parse({text, {}});
    CHECK(parsed.model && parsed.error.empty());
    const unsigned nodes = std::string(name) == "traceability" ? 13 : 9;
    CHECK(parsed.model->nodes.size() == nodes);
    if (nodes == 13)
      CHECK(parsed.model->edges.size() == 17);
    const auto layoutStart = std::chrono::steady_clock::now();
    auto scene = layout.layout(*parsed.model, {}, metrics);
    const auto layoutMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::steady_clock::now() - layoutStart)
                              .count();
    CHECK(scene->nodes.size() == nodes && scene->width > 0 && scene->height > 0);
    CHECK(scene->edges.size() == parsed.model->edges.size());
    CHECK(scene->svg.rfind("<svg", 0) == 0);
    CHECK(scene->svg.find("edge-0") != std::string::npos);
    for (std::size_t i = 0; i < scene->nodes.size(); ++i) {
      auto label = metrics.measure(parsed.model->nodes[i].label, {});
      CHECK(scene->nodes[i].bounds.width >= label.width);
      CHECK(scene->nodes[i].bounds.height >= label.height);
    }
    std::cout << name << " " << scene->width << "x" << scene->height << " pt, " << layoutMs
              << " ms\n";
  }
  auto grouped = parser.parse({"flowchart LR\nsubgraph Outer [Group A]\nsubgraph Inner [Group "
                               "B]\nA[Ærlig]-->B\nend\nC[Rest]\nend\nB-->C",
                               {}});
  CHECK(grouped.model);
  auto groupedScene = layout.layout(*grouped.model, {}, metrics);
  CHECK(groupedScene->groups.size() == 2 && groupedScene->nodes.size() == 3);
  CHECK(groupedScene->labels[0].text == "Group A" && groupedScene->labels[1].text == "Group B");
  // Layout is independently replaceable: no parser or source needed here.
  DiagramModel model;
  model.nodes.push_back({"a", "Ærlig måling", DiagramShape::Rectangle});
  CHECK(layout.layout(model, {}, metrics)->nodes.size() == 1);
  auto failed = parser.parse({"flowchart LR\nA-->B\nclick A call()", {}});
  CHECK(!failed.model && !failed.error.empty());
  diagramWire::ResultOwner bad(xfmd_mermaid_parse_v1(99, nullptr, 0), xfmd_mermaid_parse_free_v1);
  CHECK(bad.result.status != 0);
  std::string stress = "flowchart TD\n";
  for (int i = 0; i < 127; ++i)
    stress += "N" + std::to_string(i) + "-->N" + std::to_string(i + 1) + "\n";
  auto graph = parser.parse({stress, {}});
  CHECK(graph.model);
  auto start = std::chrono::steady_clock::now();
  CHECK(layout.layout(*graph.model, {}, metrics)->nodes.size() == 128);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                                  start)
                .count();
  std::cout << "128-node chain: " << ms << " ms\n";
  CHECK(ms < diagramLayoutBudgetMilliseconds + 3000);
  model = {};
  for (unsigned i = 0; i < 128; ++i)
    model.nodes.push_back({"n" + std::to_string(i), "Node", DiagramShape::Rectangle});
  for (unsigned i = 0; i < 512; ++i)
    model.edges.push_back({i % 128, (i * 17 + i / 128 + 1) % 128, "", false, true, 0});
  start = std::chrono::steady_clock::now();
  bool budget = false;
  try {
    layout.layout(model, {}, metrics);
  } catch (const std::exception& e) {
    budget = std::string(e.what()) == "Diagram layout time budget exceeded";
  }
  CHECK(budget);
  ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                             start)
           .count();
  std::cout << "128-node / 512-edge cyclic graph: " << ms << " ms\n";
  CHECK(ms + 100 >= diagramLayoutBudgetMilliseconds);
  CHECK(ms < diagramLayoutBudgetMilliseconds + 3000);
  model = {};
  model.nodes.push_back({"after", "After cancellation", DiagramShape::Rectangle});
  CHECK(layout.layout(model, {}, metrics)->nodes.size() == 1);
}
TEST_MAIN(run)
