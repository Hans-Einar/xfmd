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
  if (!frame.readingText.empty() && frame.readingText.back() != '\n')
    frame.readingText += '\n';
  for (const auto& label : scene.labels) {
    DrawRun run;
    run.text = label.text;
    run.bounds = {x + label.origin.x * scale, y + label.origin.y * scale,
                  label.extent.width * scale, label.extent.height * scale};
    run.ascent = label.extent.ascent * scale;
    run.shaped = label.extent.shaped;
    run.textScale = scale;
    run.source = source;
    run.codeBackground = label.background;
    run.textBegin = frame.readingText.size();
    frame.readingText += label.text;
    run.textEnd = frame.readingText.size();
    frame.readingText += '\n';
    frame.runs.push_back(std::move(run));
  }
  frame.flow.lines.push_back({y, scene.height * scale, 0});
  return scene.height * scale;
}
} // namespace xfmd
