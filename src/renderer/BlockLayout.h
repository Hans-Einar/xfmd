#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class BlockLayout {
public:
  static void layout(const SemanticDocument&, const LayoutRequest&, ITextMetrics&, RenderFrame&);
};
} // namespace xfmd
