#include "ViewTransform.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
void ViewTransform::configure(const RenderFrame& frame, double viewportWidth, double dpiScale,
                              double zoom, bool fit) {
  paged = frame.key.profile.mode == LayoutMode::Paged;
  pageWidth = frame.key.profile.paper.width;
  pageHeight = frame.key.profile.paper.height;
  pages = std::max<std::size_t>(1, frame.pages.slices.size());
  padding = paged ? 16 : 0;
  scale = paged && fit ? std::max(.05, (viewportWidth - 2 * padding) / pageWidth) : dpiScale * zoom;
  if (!paged)
    scale = dpiScale;
  scale = std::clamp(scale, .05, 8.0);
  offset = paged ? std::max(padding, (viewportWidth - pageWidth * scale) / 2) : 0;
  contentWidth =
      paged && fit ? int(viewportWidth) : int(std::ceil(frame.contentWidth * scale + 2 * padding));
  contentHeight =
      int(std::ceil(frame.height * scale + 2 * padding + (paged ? (pages - 1) * 20 : 0)));
}
Point ViewTransform::toView(Point p) const {
  auto page =
      paged ? std::min(pages - 1, std::size_t(std::max(0.0, std::floor(p.y / pageHeight)))) : 0;
  return {offset + p.x * scale, padding + p.y * scale + page * 20};
}
Point ViewTransform::toDocument(Point p) const {
  if (!paged)
    return {p.x / scale, p.y / scale};
  const double pitch = pageHeight * scale + 20, local = std::max(0.0, p.y - padding);
  auto page = std::min(pages - 1, std::size_t(local / pitch));
  double within = local - page * pitch;
  if (within >= pageHeight * scale + 10 && page + 1 < pages) {
    ++page;
    within = 0;
  }
  return {(p.x - offset) / scale,
          page * pageHeight + std::clamp(within / scale, 0.0, pageHeight - .00001)};
}
Rect ViewTransform::pageRect(std::size_t page) const {
  return {offset, padding + page * (pageHeight * scale + 20), pageWidth * scale,
          pageHeight * scale};
}
} // namespace xfmd
