#pragma once
#include "contracts/RenderFrame.h"
#include "contracts/SemanticDocument.h"
#include <optional>
namespace xfmd {
class LinkMarker {
public:
  static std::optional<DrawRun> make(const InlineRun&, FontSpec, ITextMetrics&);
};
} // namespace xfmd
