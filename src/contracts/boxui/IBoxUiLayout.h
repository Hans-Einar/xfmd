#pragma once
#include "BoxUiFrame.h"
namespace xfmd {
class IBoxUiLayout {
public:
  virtual ~IBoxUiLayout() = default;
  virtual std::shared_ptr<const BoxUiFrame> prepare(const BoxUiModel&, const BoxUiLayoutRequest&,
                                                    ITextMetrics&) = 0;
};
} // namespace xfmd
