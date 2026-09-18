#pragma once
#include "contracts/boxui/IBoxUiLayout.h"
namespace xfmd {
class BoxUiLayout final : public IBoxUiLayout {
public:
  std::shared_ptr<const BoxUiFrame> prepare(const BoxUiModel&, const BoxUiLayoutRequest&,
                                            ITextMetrics&) override;
};
} // namespace xfmd
