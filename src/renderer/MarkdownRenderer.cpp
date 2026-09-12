#include "MarkdownRenderer.h"
#include "BlockLayout.h"
#include "HitTester.h"
#include <algorithm>
namespace xfmd {
LayoutResult MarkdownRenderer::layout(const SemanticDocument& model, const LayoutRequest& request, ITextMetrics& metrics) {
  if (request.width <= 0) throw Error(ErrorCode::Layout, "Viewport width must be positive.");
  auto frame = std::make_shared<RenderFrame>();
  frame->token = model.token; frame->generation = request.generation;
  frame->width = request.width; frame->contentWidth = request.width;
  BlockLayout::layout(model, request, metrics, *frame);
  return frame;
}
HitResult MarkdownRenderer::hitTest(const RenderFrame& frame, Point point) const { return HitTester::hitTest(frame, point); }
}
