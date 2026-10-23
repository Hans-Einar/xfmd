#include "DiagramPlacement.h"
#include "contracts/diagram/DiagramScene.h"
#include <algorithm>
namespace xfmd {
double DiagramPlacement::append(const SemanticBlock& block, double x, double y, double width,
                                const LayoutRequest& request, ITextMetrics& metrics,
                                RenderFrame& frame) {
  const auto& scene = *block.diagramScene;
  if (scene.fonts != metrics.fontSetId())
    throw Error(ErrorCode::Conflict, "Diagram font set changed; refresh preview");
  const bool fullSize = request.profile.mode == LayoutMode::Continuous &&
                        request.profile.fullSizeDiagrams;
  double scale = fullSize ? 1.0 : std::min(1.0, width / scene.width);
  double height = scene.height * scale;
  if (request.profile.mode == LayoutMode::Paged) {
    // Multiplying the limited scale back by scene.height can exceed the exact
    // page boundary by one ULP. Keep drawing and pagination on that boundary.
    const double available = request.profile.paper.height - 2 * request.profile.paper.margin;
    if (height > available) {
      height = available;
      scale = height / scene.height;
    }
  }
  auto source = block.source;
  source.quality = MappingQuality::Approximate;
  DrawRun visual;
  visual.bounds = {x, y, scene.width * scale, height};
  if (fullSize)
    frame.contentWidth = std::max(frame.contentWidth,
                                 x + visual.bounds.width + std::min(24.0, request.width * .15));
  visual.visual = block.diagramScene;
  visual.source = source;
  frame.runs.push_back(std::move(visual));
  frame.maxRunHeight = std::max(frame.maxRunHeight, height);
  frame.flow.lines.push_back({y, height, 0});
  return height;
}
} // namespace xfmd
