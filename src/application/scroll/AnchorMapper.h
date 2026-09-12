#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
struct MappingResult {
  int y = 0;
  MappingQuality quality = MappingQuality::Unavailable;
};
class AnchorMapper {
public:
  static MappingResult map(SourceAnchor, const RenderFrame&);
  static SourceAnchor anchorAt(int y, const RenderFrame&);
};
} // namespace xfmd
