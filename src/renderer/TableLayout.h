#pragma once
#include "contracts/IRenderer.h"
namespace xfmd {
class TableLayout {
public:
  static double layout(const SemanticTable&, double left, double top, double width, ITextMetrics&,
                       RenderFrame&, const LayoutRequest&);
};
} // namespace xfmd
