#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class InlineLayout {
public:
  static int layout(const SemanticBlock&, int x, int y, int width, FontSpec, ITextMetrics&,
                    RenderFrame&);
};
} // namespace xfmd
