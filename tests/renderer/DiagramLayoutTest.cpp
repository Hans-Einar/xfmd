#include "application/adapters/SharedTextMetrics.h"
#include "contracts/diagram/DiagramLimits.h"
#include "contracts/diagram/DiagramWire.h"
#include "interpreter/mermaid/MermaidInterpreter.h"
#include "renderer/diagram/DiagramTextLayout.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
#include "support/TestSupport.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace xfmd;
void run() {
  std::cout << std::unitbuf;
  MermaidInterpreter parser;
  MermaidDiagramLayout layout;
  SharedTextMetrics metrics;
  for (const auto* name : {"apt-import", "sequence-fragments", "sequence-nested"}) {
    std::ifstream file(std::string(XFMD_DIAGRAM_FIXTURES) + "/" + name + ".mmd");
    std::string text((std::istreambuf_iterator<char>(file)), {});
    auto parsed = parser.parse({text, {}});
    if (!parsed.model)
      throw std::runtime_error(parsed.error);
    CHECK(parsed.model->sequence && parsed.model->nodes.empty());
    auto scene = layout.layout(*parsed.model, {}, metrics);
    CHECK(
        scene->svg.find(std::string(name) != "sequence-fragments" ? "Operatør" : "Måletjeneste") !=
        std::string::npos);
    CHECK(scene->diagnostics.find("Sequence 2") != std::string::npos);
    CHECK(scene->nodes.empty() && scene->edges.empty());
    diagramWire::Writer wire;
    wire.model(*parsed.model);
    XfmdDiagramResult value{};
    value.abi_version = 1;
    value.struct_size = sizeof(value);
    value.data = wire.data.data();
    value.size = wire.data.size();
    diagramWire::Reader reader(value);
    auto decoded = reader.model();
    reader.finish();
    CHECK(decoded.sequence->events.size() == parsed.model->sequence->events.size());
    auto repeated=layout.layout(decoded, {}, metrics);
    CHECK(repeated->svg == scene->svg);
    if (const char* directory = std::getenv("XFMD_DIAGRAM_EVIDENCE"))
      std::ofstream(std::string(directory) + "/" + name + ".svg") << scene->svg;
  }
  for (const auto* name : {"measurement-state", "state-regions", "state-choice", "sdl-class",
                           "apt-requirements", "provenance-er", "c4-context", "c4-container", "c4-component", "architecture-resources", "block-layers", "packet-encoding", "timeline-decisions", "gantt-pilot", "journey-review"}) {
    std::ifstream file(std::string(XFMD_DIAGRAM_FIXTURES) + "/" + name + ".mmd");
    auto parsed = parser.parse({std::string((std::istreambuf_iterator<char>(file)), {}), {}});
    if (!parsed.model)
      throw std::runtime_error(parsed.error);
    CHECK(parsed.model->semantic && parsed.model->nodes.empty());
    auto scene = layout.layout(*parsed.model, {}, metrics);
    CHECK(scene->width > 0 && scene->height > 0 && scene->svg.find("<text") != std::string::npos);
    CHECK(scene->diagnostics.find("Semantic profile") != std::string::npos);
    diagramWire::Writer wire;
    wire.model(*parsed.model);
    XfmdDiagramResult value{
        1, sizeof(XfmdDiagramResult), 0, wire.data.data(), wire.data.size(), nullptr};
    diagramWire::Reader reader(value);
    auto decoded = reader.model();
    reader.finish();
    CHECK(decoded.semantic->records.size() == parsed.model->semantic->records.size());
    auto repeated=layout.layout(decoded, {}, metrics);
    // Native libavoid pin ties can change geometry; semantic wire records must not.
    CHECK(repeated->width > 0 && repeated->height > 0);
    CHECK(repeated->svg.find("<text") != std::string::npos);
    if (const char* directory = std::getenv("XFMD_DIAGRAM_EVIDENCE"))
      std::ofstream(std::string(directory) + "/" + name + ".svg") << scene->svg;
    std::cout << name << " " << scene->width << "x" << scene->height << " pt\n";
  }
  DiagramModel wrapping;
  wrapping.nodes.push_back({"a", "A", DiagramShape::Rectangle});
  wrapping.nodes.push_back({"b", "B", DiagramShape::Rectangle});
  const std::string caption = "provides or requires at its boundary";
  wrapping.edges.push_back({0, 1, caption, false, true, 0});
  auto measured = DiagramTextLayout::measure(wrapping, metrics).at(caption);
  CHECK(measured.lines.size() > 1 && measured.width <= 120.);
  std::string joined;
  for (const auto& line : measured.lines) {
    if (!joined.empty())
      joined += " ";
    joined += line.text;
  }
  CHECK(joined == caption);
  auto wrappedScene = layout.layout(wrapping, {}, metrics);
  CHECK(wrappedScene->svg.find("label-leader") != std::string::npos);
  CHECK(wrappedScene->svg.find("<circle") != std::string::npos);
  CHECK(wrappedScene->diagnostics.find("label leaders omitted: 0") != std::string::npos);
  for (const auto& line : measured.lines)
    CHECK(wrappedScene->svg.find(">" + line.text + "<") != std::string::npos);
  for (const auto& caption : {std::string("Ærlig måling av blåbær gir nyttig informasjon"),
                              std::string("First\n\nSecond"), std::string(50, 'W')}) {
    wrapping.edges[0].label = caption;
    auto text = DiagramTextLayout::measure(wrapping, metrics).at(caption);
    CHECK(!text.lines.empty());
    if (caption == "First\n\nSecond")
      CHECK(text.lines.size() == 3 && text.lines[1].text.empty());
    else if (caption == std::string(50, 'W'))
      CHECK(text.lines.size() == 1 && text.width > 120.);
    else
      CHECK(text.lines.size() > 1 && text.width <= 120.);
  }
  wrapping.nodes[0].label = caption;
  wrapping.edges[0].label = caption;
  CHECK(DiagramTextLayout::measure(wrapping, metrics).at(caption).lines.size() == 1);

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
    CHECK(!scene->diagnostics.empty());
    if (const char* directory = std::getenv("XFMD_DIAGRAM_EVIDENCE")) {
      std::ofstream output(std::string(directory) + "/" + name + ".svg");
      output << scene->svg;
      CHECK(output.good());
    }
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
  // Explicit selection and bad configuration must never silently switch engines.
  const char* oldEngine = std::getenv("XFMD_MERMAID_ROUTER");
  const std::string savedEngine = oldEngine ? oldEngine : "";
  const bool hadEngine = oldEngine != nullptr;
  setenv("XFMD_MERMAID_ROUTER", "legacy", 1);
  CHECK(layout.layout(model, {}, metrics)->diagnostics.find("Legacy") == 0);
  setenv("XFMD_MERMAID_ROUTER", "unknown", 1);
  bool invalidEngine = false;
  try {
    layout.layout(model, {}, metrics);
  } catch (const std::exception& e) {
    invalidEngine = std::string(e.what()).find("XFMD_MERMAID_ROUTER") != std::string::npos;
  }
  CHECK(invalidEngine);
  if (hadEngine)
    setenv("XFMD_MERMAID_ROUTER", savedEngine.c_str(), 1);
  else
    unsetenv("XFMD_MERMAID_ROUTER");
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
  bool budget = false, complexity = false;
  try {
    layout.layout(model, {}, metrics);
  } catch (const std::exception& e) {
    budget = std::string(e.what()) == "Diagram layout time budget exceeded" ||
             std::string(e.what()).find("BudgetExceeded") != std::string::npos;
    complexity = std::string(e.what()).find("complexity limit") != std::string::npos;
  }
  CHECK(budget || complexity);
  ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                             start)
           .count();
  std::cout << "128-node / 512-edge cyclic graph: " << ms << " ms\n";
  if (budget)
    CHECK(ms + 100 >= diagramLayoutBudgetMilliseconds);
  CHECK(ms < diagramLayoutBudgetMilliseconds + 3000);
  model = {};
  model.nodes.push_back({"after", "After cancellation", DiagramShape::Rectangle});
  CHECK(layout.layout(model, {}, metrics)->nodes.size() == 1);
}
TEST_MAIN(run)
