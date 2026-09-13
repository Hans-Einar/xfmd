#include "PageComposer.h"
#include "PageAnchorIndex.h"
#include "PageBreaker.h"
#include <algorithm>
namespace xfmd {
void PageComposer::compose(RenderFrame& frame, const PaperSpec& paper,
                           const std::function<bool()>& cancelled) {
  frame.pages.paper = paper;
  frame.pages.slices = PageBreaker::breakLines(frame.flow, paper, cancelled);
  for (auto& run : frame.runs) {
    auto page = std::upper_bound(frame.pages.slices.begin(), frame.pages.slices.end(), run.bounds.y,
                                 [](double y, const PageSlice& p) { return y < p.flowTop; });
    if (page != frame.pages.slices.begin())
      --page;
    run.bounds.x += paper.margin - 24;
    run.bounds.y += page->page * paper.height + paper.margin - page->flowTop;
    if (run.bounds.x + run.bounds.width > paper.width - paper.margin + 0.01)
      throw Error(ErrorCode::Layout, "Text exceeds the printable page width.");
  }
  std::vector<Decoration> decorations;
  for (const auto& item : frame.decorations) {
    auto first =
        std::lower_bound(frame.pages.slices.begin(), frame.pages.slices.end(), item.bounds.y,
                         [](const PageSlice& p, double y) { return p.flowBottom <= y; });
    for (; first != frame.pages.slices.end() && first->flowTop < item.bounds.y + item.bounds.height;
         ++first) {
      const double top = std::max(item.bounds.y, first->flowTop),
                   bottom = std::min(item.bounds.y + item.bounds.height, first->flowBottom);
      Rect r{item.bounds.x + paper.margin - 24,
             first->page * paper.height + paper.margin + top - first->flowTop, item.bounds.width,
             bottom - top};
      r.width = std::min(r.width, paper.width - paper.margin - r.x);
      if (r.height > 0 && r.width > 0)
        decorations.push_back({r, item.color});
    }
  }
  frame.decorations = std::move(decorations);
  frame.anchors = PageAnchorIndex::build(frame.anchors, frame.pages);
  frame.width = frame.contentWidth = paper.width;
  frame.height = paper.height * frame.pages.slices.size();
}
} // namespace xfmd
