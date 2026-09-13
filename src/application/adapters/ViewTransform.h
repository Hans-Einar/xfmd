#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
// Only the host knows pixels, zoom and page gaps. Document geometry stays in points.
class ViewTransform {
  bool paged = false;
  double pageHeight = 0, pageWidth = 0, offset = 0, padding = 0;
  std::size_t pages = 1;

public:
  double scale = 1;
  int contentWidth = 0, contentHeight = 0;
  void configure(const RenderFrame&, double viewportWidth, double dpiScale, double zoom, bool fit);
  Point toView(Point) const;
  Point toDocument(Point) const;
  Rect pageRect(std::size_t) const;
};
} // namespace xfmd
