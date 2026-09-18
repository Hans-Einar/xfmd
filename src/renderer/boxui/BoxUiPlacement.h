#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
struct BoxUiPlacement {
  static double append(const SemanticBlock&, double, double, double, const LayoutRequest&,
                       ITextMetrics&, RenderFrame&);
};
} // namespace xfmd
