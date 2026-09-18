#include "DiagramServices.h"
#include "application/boxui/BoxUiPreparation.h"
#include "application/diagrams/DiagramPreparation.h"
#include "application/media/EmbeddedVisuals.h"
#include "interpreter/CmarkInterpreter.h"
#include "interpreter/boxui/BoxUiInterpreter.h"
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
  auto diagrams = std::make_shared<MermaidInterpreter>();
  return std::make_unique<CmarkInterpreter>(diagrams, std::make_shared<BoxUiInterpreter>(diagrams));
}
PrepareDocument DiagramServices::preview(std::function<BoxUiState(DocumentToken)> state) {
  return [state = std::move(state), resources = std::shared_ptr<WorkerResources>{}](
             ParseResult model, const SourceSnapshot& source,
             const std::function<bool()>& cancelled) mutable {
    auto frozen = state ? state(source.token) : BoxUiState{};
    if (!resources)
      resources = std::make_shared<WorkerResources>();
    auto prepared = resources->diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source),
                                                resources->metrics, cancelled);
    return BoxUiPreparation::prepare(prepared, resources->metrics, resources->layout, frozen,
                                     cancelled, frozen.viewportWidth);
  };
}
ParseResult DiagramServices::prepare(ParseResult model, const SourceSnapshot& source,
                                     SharedTextMetrics& metrics,
                                     const std::function<bool()>& cancelled,
                                     const BoxUiState& state) {
  MermaidDiagramLayout layout;
  DiagramPreparation diagrams(layout);
  auto prepared =
      diagrams.prepare(EmbeddedVisuals::prepare(std::move(model), source), metrics, cancelled);
  return BoxUiPreparation::prepare(prepared, metrics, layout, state, cancelled,
                                   state.viewportWidth);
}
} // namespace xfmd
