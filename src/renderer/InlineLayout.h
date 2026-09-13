#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class InlineLayout {
public:
  static double layout(const SemanticBlock&, double x, double y, double width, FontSpec, ITextMetrics&,
                    RenderFrame&);
};
} // namespace xfmd
