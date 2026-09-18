#include "DiagramServices.h"
#include "interpreter/boxui/BoxUiInterpreter.h"
#include "application/boxui/BoxUiPreparation.h"
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
  auto diagrams=std::make_shared<MermaidInterpreter>();
  return std::make_unique<CmarkInterpreter>(diagrams, std::make_shared<BoxUiInterpreter>(diagrams));
}
PrepareDocument DiagramServices::preview() {
  return [resources =
              std::shared_ptr<WorkerResources>{}](ParseResult model, const SourceSnapshot& source,
                                                  const std::function<bool()>& cancelled) mutable {
    if (!resources)
      resources = std::make_shared<WorkerResources>();
    auto prepared = resources->diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source),
                                       resources->metrics, cancelled);
    return BoxUiPreparation::prepare(prepared,resources->metrics,resources->layout,{},cancelled);
  };
}
ParseResult DiagramServices::prepare(ParseResult model, const SourceSnapshot& source,
                                     SharedTextMetrics& metrics,
                                     const std::function<bool()>& cancelled) {
  MermaidDiagramLayout layout;
  DiagramPreparation diagrams(layout);
  auto prepared=diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source), metrics, cancelled);
  return BoxUiPreparation::prepare(prepared,metrics,layout,{},cancelled);
}
} // namespace xfmd
