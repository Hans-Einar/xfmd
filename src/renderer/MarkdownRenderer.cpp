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
  frame->runs.reserve(model.blocks.size() * 8);
  frame->anchors.reserve(model.blocks.size() * 10);
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
  return frame;
}
HitResult MarkdownRenderer::hitTest(const RenderFrame& frame, Point point) const {
  return HitTester::hitTest(frame, point);
}
} // namespace xfmd
