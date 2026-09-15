#include "MarkdownRenderer.h"
#include "BlockLayout.h"
#include "HitTester.h"
#include "PageComposer.h"
#include <algorithm>
namespace xfmd {
LayoutResult MarkdownRenderer::layout(const SemanticDocument& model, const LayoutRequest& request,
                                      ITextMetrics& metrics) {
  if (request.width <= 0)
    throw Error(ErrorCode::Layout, "Viewport width must be positive.");
  auto frame = std::make_shared<RenderFrame>();
  frame->token = model.token;
  frame->generation = request.generation;
  frame->key = {model.token, request.profile, metrics.fontSetId(), request.generation,
                request.width};
  frame->width = request.width;
  frame->contentWidth = request.width;
  frame->runs.reserve(
      std::min<std::size_t>(100000, std::max(model.blocks.size() * 12, model.sourceSize / 16)));
  frame->shapeParts.reserve(std::min<std::size_t>(1000000, model.sourceSize / 6));
  frame->flow.lines.reserve(model.sourceSize / 40);
  frame->anchors.reserve(
      std::min<std::size_t>(200000, std::max(model.blocks.size() * 16, model.sourceSize / 12)));
  frame->decorations.reserve(model.blocks.size());
  auto flowRequest = request;
  if (request.profile.mode == LayoutMode::Paged) {
    request.profile.paper.validate();
    flowRequest.width = request.profile.paper.width - 2 * request.profile.paper.margin + 48;
    frame->key.flowWidth = 0;
  }
  BlockLayout::layout(model, flowRequest, metrics, *frame);
  if (request.profile.mode == LayoutMode::Paged)
    PageComposer::compose(*frame, request.profile.paper, request.cancelled);
  std::stable_sort(frame->runs.begin(),frame->runs.end(),[](const DrawRun& a,const DrawRun& b){return a.bounds.y<b.bounds.y;});
  return frame;
}
HitResult MarkdownRenderer::hitTest(const RenderFrame& frame, Point point) const {
  return HitTester::hitTest(frame, point);
}
} // namespace xfmd
