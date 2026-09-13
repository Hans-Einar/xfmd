#pragma once
#include "contracts/PageLayout.h"
#include <functional>
namespace xfmd {
class PageBreaker {
public:
  static std::vector<PageSlice> breakLines(const FlowLayout&, const PaperSpec&,
                                           const std::function<bool()>& cancelled = {});
};
} // namespace xfmd
