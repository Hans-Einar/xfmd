#pragma once
#include "contracts/SemanticDocument.h"
#include <optional>
namespace xfmd {
class HeadingAnchor {
public:
  static std::optional<SourceAnchor> find(const SemanticDocument&, const std::string& fragment);
};
} // namespace xfmd
