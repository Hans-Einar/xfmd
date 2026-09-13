#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class InlineLayout {
public:
  static double layout(const SemanticBlock&, double x, double y, double width, FontSpec,
                       ITextMetrics&, RenderFrame&, bool wrapCode = false,
                       const std::function<bool()>& cancelled = {});
};
} // namespace xfmd
