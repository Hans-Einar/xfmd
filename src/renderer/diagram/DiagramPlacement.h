#pragma once
#include "contracts/RenderFrame.h"
#include "contracts/SemanticDocument.h"
namespace xfmd {
class DiagramPlacement {
public:
  static double append(const SemanticBlock&, double, double, double, const LayoutRequest&,
                       ITextMetrics&, RenderFrame&);
};
} // namespace xfmd
