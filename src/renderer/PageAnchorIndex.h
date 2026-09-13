#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
class PageAnchorIndex {
public:
  static std::vector<AnchorRegion> build(const std::vector<AnchorRegion>&,const PageLayout&);
};
} // namespace xfmd
