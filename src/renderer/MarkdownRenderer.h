#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class MarkdownRenderer final : public IRenderer {
public:
  LayoutResult layout(const SemanticDocument&, const LayoutRequest&, ITextMetrics&) override;
  HitResult hitTest(const RenderFrame&, Point) const override;
};
} // namespace xfmd
