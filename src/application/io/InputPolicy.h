#pragma once
#include "contracts/DocumentTypes.h"
#include <string_view>
namespace xfmd {
class InputPolicy {
public:
  static bool supportedPath(const std::string&);
  static bool plainText(const std::string&);
  static void validate(std::string_view text, const std::string& path = {});
};
} // namespace xfmd
