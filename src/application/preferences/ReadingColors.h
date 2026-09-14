#pragma once
#include "contracts/RenderFrame.h"
#include <cstdint>
namespace xfmd {
struct ReadingColors {
  int backgroundTone = -1, backgroundBrightness = 100;
  int textTone = -1, textBrightness = 15;
  static ReadingColors defaults(bool dark) {
    return dark ? ReadingColors{-1, 10, -1, 90} : ReadingColors{};
  }
  bool valid() const;
  bool operator==(const ReadingColors& b) const {
    return backgroundTone == b.backgroundTone && backgroundBrightness == b.backgroundBrightness &&
           textTone == b.textTone && textBrightness == b.textBrightness;
  }
  static std::uint32_t rgb(int tone, int brightness);
};
struct ReadingPalette {
  std::uint32_t background, text, surface, alternate, border, inactive, surround;
  static ReadingPalette from(const ReadingColors&);
  std::uint32_t decoration(DecorationRole) const;
  double contrast() const;
};
} // namespace xfmd
