#pragma once
#include <string>
namespace xfmd {
struct Appearance {
  std::string theme = "light";
  bool compact = false;
  std::string buttons = "flat";
  int fontSize = 10;
  bool operator==(const Appearance& b) const {
    return theme == b.theme && compact == b.compact && buttons == b.buttons &&
           fontSize == b.fontSize;
  }
  bool operator!=(const Appearance& b) const { return !(*this == b); }
};
} // namespace xfmd
