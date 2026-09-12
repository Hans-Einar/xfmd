#pragma once
#include "contracts/DocumentTypes.h"
#include <vector>
namespace xfmd {
class TextProjection {
  std::vector<std::size_t> offsets;
  std::string newline = "\n";

public:
  std::string text;
  explicit TextProjection(const std::string& raw);
  std::size_t sourceOffset(std::size_t projected) const;
  std::size_t displayOffset(std::size_t source) const;
  Edit difference(const std::string& edited) const;
};
} // namespace xfmd
