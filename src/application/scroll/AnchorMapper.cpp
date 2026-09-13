#include "AnchorMapper.h"
#include <algorithm>
#include <cmath>
#include <limits>
namespace xfmd {
MappingResult AnchorMapper::map(SourceAnchor anchor, const RenderFrame& frame) {
  const AnchorRegion* best = nullptr;
  std::size_t distance = std::numeric_limits<std::size_t>::max(), span = distance;
  for (const auto& region : frame.anchors) {
    if (region.source.quality == MappingQuality::Unavailable)
      continue;
    auto start = region.source.begin, end = region.source.end;
    auto delta = anchor.byte < start  ? start - anchor.byte
                 : anchor.byte >= end ? anchor.byte - end + 1
                                      : 0;
    auto length = end - start;
    if (delta < distance || (delta == distance && length < span)) {
      best = &region;
      distance = delta;
      span = length;
    }
  }
  if (!best)
    return {};
  auto quality = distance == 0 ? best->source.quality : MappingQuality::Approximate;
  return {int(std::max(0.0, best->bounds.y +
                          int(std::clamp(anchor.fraction, 0.0, 0.99) * best->bounds.height))),
          quality};
}
SourceAnchor AnchorMapper::anchorAt(int y, const RenderFrame& frame) {
  const AnchorRegion* best = nullptr;
  double distance = std::numeric_limits<double>::max();
  double height = distance;
  for (const auto& region : frame.anchors) {
    if (region.source.quality == MappingQuality::Unavailable)
      continue;
    double start = region.bounds.y, end = start + region.bounds.height;
    double delta = y < start ? start - y : y >= end ? y - end + 1 : 0;
    if (delta < distance || (delta == distance && region.bounds.height < height) ||
        (delta == distance && region.bounds.height == height && best &&
         region.source.begin < best->source.begin)) {
      best = &region;
      distance = delta;
      height = region.bounds.height;
    }
  }
  if (!best)
    return {0, 0, MappingQuality::Unavailable};
  double fraction =
      std::clamp(double(y - best->bounds.y) / std::max(1.0, best->bounds.height), 0.0, 0.99);
  return {best->source.begin, fraction,
          distance == 0 ? best->source.quality : MappingQuality::Approximate};
}
} // namespace xfmd
