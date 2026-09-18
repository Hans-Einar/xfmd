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
  double scale = std::min(1.0, width / scene.width);
  if (request.profile.mode == LayoutMode::Paged)
    scale = std::min(scale, (request.profile.paper.height - 2 * request.profile.paper.margin) /
                                scene.height);
  auto source = block.source;
  source.quality = MappingQuality::Approximate;
  DrawRun visual;
  visual.bounds = {x, y, scene.width * scale, scene.height * scale};
  visual.visual = block.diagramScene;
  visual.source = source;
  frame.runs.push_back(std::move(visual));
  frame.maxRunHeight = std::max(frame.maxRunHeight, scene.height * scale);
  frame.flow.lines.push_back({y, scene.height * scale, 0});
  return scene.height * scale;
}
} // namespace xfmd
