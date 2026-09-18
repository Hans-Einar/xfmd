#include "BoxUiPlacement.h"
#include "contracts/boxui/BoxUiFrame.h"
#include <algorithm>
namespace xfmd {
double BoxUiPlacement::append(const SemanticBlock& block, double x, double y, double width,
                              const LayoutRequest& request, ITextMetrics& metrics,
                              RenderFrame& frame) {
  const auto& box = *block.boxUiFrame;
  if (box.staticScene->fonts != metrics.fontSetId())
    throw Error(ErrorCode::Conflict, "BoxUI font set changed");
  double scale = std::min(1., width / box.width);
  if (request.profile.mode == LayoutMode::Paged)
    scale = std::min(scale, (request.profile.paper.height - 2 * request.profile.paper.margin) /
                                box.height);
  DrawRun run;
  run.bounds = {x, y, box.width * scale, box.height * scale};
  run.visual = block.boxUiFrame;
  run.source = block.source;
  run.source.quality = MappingQuality::Approximate;
  frame.runs.push_back(std::move(run));
  frame.maxRunHeight = std::max(frame.maxRunHeight, box.height * scale);
  frame.flow.lines.push_back({y, box.height * scale, 0});
  return box.height * scale;
}
} // namespace xfmd
