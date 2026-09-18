#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "contracts/diagram/DiagramModel.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <fstream>
#include <set>
using namespace xfmd;
int main(int argc, char** argv) {
  if (argc != 3 && argc != 4)
    return 2;
  try {
    SourceSnapshot source{{7, 3}, "# Mermaid export\n\n", {}, false};
    for (auto name : {"service-map",         "layer-delivery",     "traceability",
                      "apt-import",          "sequence-nested",    "measurement-state",
                      "state-regions",       "state-choice",       "sdl-class",
                      "apt-requirements",    "provenance-er",      "c4-context",
                      "c4-container",        "c4-component",       "architecture-resources",
                      "block-layers",        "packet-encoding",    "timeline-decisions",
                      "gantt-pilot",         "journey-review",     "pie-evidence",
                      "mindmap-review",      "gitgraph-proposal",  "sankey-provenance",
                      "quadrant-priorities", "zenuml-observation", "kanban-review",
                      "radar-quality",       "treemap-effort",     "xychart-evidence"}) {
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
    if (argc == 4) {
      std::ifstream gallery(argv[3]);
      CHECK(gallery.good());
      source.text = std::string((std::istreambuf_iterator<char>(gallery)), {});
    }
    unsigned expectedDiagrams = 0;
    for (std::size_t pos = 0; (pos = source.text.find("```mermaid", pos)) != std::string::npos;
         pos += 10)
      ++expectedDiagrams;
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
    CHECK(diagrams == expectedDiagrams);
    if (argc == 4) {
      std::set<unsigned> families;
      for (const auto& block : model->blocks)
        if (block.diagram)
          families.insert(block.diagram->semantic   ? unsigned(block.diagram->semantic->family)
                          : block.diagram->sequence ? 1
                                                    : 0);
      CHECK(families.size() == 23 && diagrams == 29);
    }
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
    // Check rendered Markdown runs: readingText intentionally omits decorative
    // link prefixes, which would make a whole-paragraph comparison inaccurate.
    for (const auto& run : frame->runs)
      if (!run.text.empty())
        expected << run.text << "\n";
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
    for (const auto& block : model->blocks)
      if (block.diagram && block.diagram->semantic) {
        for (const auto& record : block.diagram->semantic->records) {
          const auto& f = record.fields;
          auto emit = [&](unsigned i) {
            if (!f[i].empty())
              expected << f[i] << "\n";
          };
          switch (record.tag) {
          case SemanticTag::PieSlice:
            emit(0);
            break;
          case SemanticTag::TreeNode:
          case SemanticTag::ZenParticipant:
          case SemanticTag::KanbanColumn:
          case SemanticTag::KanbanCard:
          case SemanticTag::RadarAxis:
          case SemanticTag::RadarCurve:
            emit(1);
            break;
          case SemanticTag::GitBranch:
            emit(0);
            break;
          case SemanticTag::SankeyLink:
            emit(0);
            emit(1);
            break;
          case SemanticTag::QuadrantPoint:
            emit(0);
            break;
          case SemanticTag::QuadrantAxis:
            emit(1);
            emit(2);
            break;
          case SemanticTag::QuadrantLabel:
            emit(1);
            break;
          case SemanticTag::ZenMessage:
            emit(2);
            break;
          case SemanticTag::XyCategories:
            for (unsigned i = 0; i < f.size(); ++i)
              emit(i);
            break;
          case SemanticTag::XyAxis:
            emit(0);
            break;
          case SemanticTag::PacketField:
            emit(2);
            break;
          case SemanticTag::PlanningTitle:
            emit(0);
            break;
          case SemanticTag::PlanningSection:
            emit(1);
            break;
          case SemanticTag::TimelineEvent:
            emit(1);
            for (unsigned i = 3; i < f.size(); ++i)
              emit(i);
            break;
          case SemanticTag::GanttTask:
          case SemanticTag::JourneyTask:
            emit(1);
            break;
          case SemanticTag::C4Element:
            emit(1);
            emit(3);
            emit(4);
            break;
          case SemanticTag::C4Boundary:
          case SemanticTag::ArchitectureGroup:
          case SemanticTag::ArchitectureService:
          case SemanticTag::BlockCell:
            emit(1);
            break;
          case SemanticTag::C4Relation:
            emit(3);
            emit(4);
            break;
          case SemanticTag::State:
            if (f[3] == "normal" || f[3] == "composite")
              emit(1);
            break;
          case SemanticTag::StateTransition:
            emit(2);
            break;
          case SemanticTag::ClassType:
          case SemanticTag::ClassAttribute:
          case SemanticTag::ClassOperation:
          case SemanticTag::ClassAnnotation:
            emit(1);
            break;
          case SemanticTag::ClassRelation:
            emit(3);
            emit(4);
            emit(5);
            break;
          case SemanticTag::Requirement:
            emit(0);
            emit(1);
            break;
          case SemanticTag::RequirementAttribute:
            emit(2);
            break;
          case SemanticTag::RequirementRelation:
            emit(2);
            break;
          case SemanticTag::Entity:
            emit(0);
            break;
          case SemanticTag::EntityAttribute:
            emit(1);
            emit(2);
            emit(4);
            break;
          case SemanticTag::EntityRelation:
            emit(5);
            break;
          default:
            break;
          }
        }
      }
    expected << "Missing\nCurrent\nStale\nFunctionality\nImportAPT\nAPT-001\nSOURCE_"
                "ARTIFACT\nDATAGRAM_OCCURRENCE\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
