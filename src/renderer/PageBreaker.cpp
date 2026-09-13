#include "PageBreaker.h"
#include <algorithm>
namespace xfmd {
std::vector<PageSlice> PageBreaker::breakLines(const FlowLayout& flow, const PaperSpec& paper,
                                               const std::function<bool()>& cancelled) {
  paper.validate();
  std::vector<PageSlice> pages;
  const double available = paper.height - 2 * paper.margin;
  for (std::size_t i = 0; i < flow.lines.size(); ++i) {
    if (cancelled && cancelled())
      throw Error(ErrorCode::Layout, "Layout cancelled.");
    const auto& line = flow.lines[i];
    if (!std::isfinite(line.top) || !std::isfinite(line.height) || line.height <= 0 ||
        line.height > available)
      throw Error(ErrorCode::Layout, "A document line is taller than the printable page.");
    auto last = std::min(flow.lines.size() - 1, i + line.keepFollowing);
    const double groupBottom = flow.lines[last].top + flow.lines[last].height;
    bool needsPage = pages.empty() || line.top + line.height - pages.back().flowTop > available;
    if (!needsPage && line.keepFollowing && groupBottom - line.top <= available &&
        groupBottom - pages.back().flowTop > available)
      needsPage = true;
    if (needsPage) {
      if (pages.size() >= 2000)
        throw Error(ErrorCode::TooLarge, "Document exceeds the 2000-page limit.");
      pages.push_back({line.top, line.top + line.height, pages.size()});
    } else
      pages.back().flowBottom = line.top + line.height;
  }
  if (pages.empty())
    pages.push_back({0, 0, 0});
  return pages;
}
} // namespace xfmd
