#include "PageAnchorIndex.h"
#include <algorithm>
namespace xfmd {
std::vector<AnchorRegion> PageAnchorIndex::build(const std::vector<AnchorRegion>& input,
                                                 const PageLayout& pages) {
  std::vector<AnchorRegion> result;
  result.reserve(input.size());
  for (const auto& anchor : input) {
    auto first = std::lower_bound(pages.slices.begin(), pages.slices.end(), anchor.bounds.y,
                                  [](const PageSlice& p, double y) { return p.flowBottom <= y; });
    for (; first != pages.slices.end() && first->flowTop < anchor.bounds.y + anchor.bounds.height;
         ++first) {
      const double top = std::max(anchor.bounds.y, first->flowTop);
      const double bottom = std::min(anchor.bounds.y + anchor.bounds.height, first->flowBottom);
      if (bottom > top)
        result.push_back(
            {anchor.source,
             {anchor.bounds.x + pages.paper.margin - 24,
              first->page * pages.paper.height + pages.paper.margin + top - first->flowTop,
              anchor.bounds.width, bottom - top},
             first->page});
    }
  }
  return result;
}
} // namespace xfmd
