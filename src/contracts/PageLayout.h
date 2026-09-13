#pragma once
#include "LayoutProfile.h"
#include <vector>
namespace xfmd {
struct FlowLine {
  LayoutUnit top = 0, height = 0;
  unsigned keepFollowing = 0;
};
struct FlowLayout {
  std::vector<FlowLine> lines;
};
struct PageSlice {
  LayoutUnit flowTop = 0, flowBottom = 0;
  std::size_t page = 0;
};
struct PageLayout {
  PaperSpec paper;
  std::vector<PageSlice> slices;
};
} // namespace xfmd
