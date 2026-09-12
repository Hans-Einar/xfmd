#pragma once
#include "RenderFrame.h"
#include "SemanticDocument.h"
#include <memory>
namespace xfmd {
using LayoutResult = std::shared_ptr<const RenderFrame>;
class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual LayoutResult layout(const SemanticDocument&, const LayoutRequest&, ITextMetrics&) = 0;
  virtual HitResult hitTest(const RenderFrame&, Point) const = 0;
};
} // namespace xfmd
