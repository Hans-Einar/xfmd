#include "DiagramServices.h"
#include "application/diagrams/DiagramPreparation.h"
#include "application/media/EmbeddedVisuals.h"
#include "interpreter/CmarkInterpreter.h"
#include "interpreter/mermaid/MermaidInterpreter.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
namespace xfmd {
namespace {
struct WorkerResources {
  SharedTextMetrics metrics;
  MermaidDiagramLayout layout;
  DiagramPreparation diagrams{layout};
};
} // namespace
std::unique_ptr<IInterpreter> DiagramServices::interpreter() {
  return std::make_unique<CmarkInterpreter>(std::make_shared<MermaidInterpreter>());
}
PrepareDocument DiagramServices::preview() {
  return [resources =
              std::shared_ptr<WorkerResources>{}](ParseResult model, const SourceSnapshot& source,
                                                  const std::function<bool()>& cancelled) mutable {
    if (!resources)
      resources = std::make_shared<WorkerResources>();
    return resources->diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source),
                                       resources->metrics, cancelled);
  };
}
ParseResult DiagramServices::prepare(ParseResult model, const SourceSnapshot& source,
                                     SharedTextMetrics& metrics,
                                     const std::function<bool()>& cancelled) {
  MermaidDiagramLayout layout;
  DiagramPreparation diagrams(layout);
  return diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source), metrics, cancelled);
}
} // namespace xfmd
