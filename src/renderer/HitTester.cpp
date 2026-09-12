#include "HitTester.h"
#include <algorithm>
namespace xfmd {
HitResult HitTester::hitTest(const RenderFrame& frame, Point point) {
  auto first = std::lower_bound(frame.runs.begin(), frame.runs.end(), point.y - 100,
      [](const DrawRun& run, int y) { return run.bounds.y < y; });
  for (; first != frame.runs.end() && first->bounds.y <= point.y; ++first)
    if (!first->link.empty() && first->bounds.contains(point)) return {first->link, first->source};
  return {};
}
}
