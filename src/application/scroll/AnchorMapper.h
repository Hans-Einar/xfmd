#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
struct MappingResult {
  double y = 0;
  MappingQuality quality = MappingQuality::Unavailable;
  std::size_t pageIndex = 0;
};
class AnchorMapper {
public:
  static MappingResult map(SourceAnchor, const RenderFrame&);
  static SourceAnchor anchorAt(double y, const RenderFrame&);
};
} // namespace xfmd
