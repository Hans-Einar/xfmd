#include "ReadingColors.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
namespace {
std::uint32_t mix(std::uint32_t a, std::uint32_t b, double amount) {
  std::uint32_t result = 0;
  for (int shift : {0, 8, 16})
    result |= std::uint32_t(
                  std::lround(((a >> shift) & 255) * (1 - amount) + ((b >> shift) & 255) * amount))
              << shift;
  return result;
}
double luminance(std::uint32_t rgb) {
  auto linear = [](unsigned channel) {
    double c = channel / 255.0;
    return c <= .04045 ? c / 12.92 : std::pow((c + .055) / 1.055, 2.4);
  };
  return .2126 * linear((rgb >> 16) & 255) + .7152 * linear((rgb >> 8) & 255) +
         .0722 * linear(rgb & 255);
}
} // namespace
bool ReadingColors::valid() const {
  return backgroundTone >= -1 && backgroundTone <= 359 && textTone >= -1 && textTone <= 359 &&
         backgroundBrightness >= 0 && backgroundBrightness <= 100 && textBrightness >= 0 &&
         textBrightness <= 100;
}
std::uint32_t ReadingColors::rgb(int tone, int brightness) {
  double value = std::clamp(brightness, 0, 100) / 100.0;
  double r = value, g = value, b = value;
  if (tone >= 0) {
    double hue = std::clamp(tone, 0, 359) / 60.0;
    double x = value * (1 - std::abs(std::fmod(hue, 2) - 1));
    switch (int(hue)) {
    case 0:
      r = value;
      g = x;
      b = 0;
      break;
    case 1:
      r = x;
      g = value;
      b = 0;
      break;
    case 2:
      r = 0;
      g = value;
      b = x;
      break;
    case 3:
      r = 0;
      g = x;
      b = value;
      break;
    case 4:
      r = x;
      g = 0;
      b = value;
      break;
    default:
      r = value;
      g = 0;
      b = x;
      break;
    }
  }
  return (std::uint32_t(std::lround(r * 255)) << 16) | (std::uint32_t(std::lround(g * 255)) << 8) |
         std::uint32_t(std::lround(b * 255));
}
ReadingPalette ReadingPalette::from(const ReadingColors& c) {
  auto bg = ReadingColors::rgb(c.backgroundTone, c.backgroundBrightness);
  auto fg = ReadingColors::rgb(c.textTone, c.textBrightness);
  return {bg,
          fg,
          mix(bg, fg, .08),
          mix(bg, fg, .035),
          mix(bg, fg, .35),
          mix(bg, fg, .6),
          mix(bg, fg, .12)};
}
std::uint32_t ReadingPalette::decoration(DecorationRole role) const {
  switch (role) {
  case DecorationRole::Surface:
    return surface;
  case DecorationRole::Alternate:
    return alternate;
  case DecorationRole::Background:
    return background;
  default:
    return border;
  }
}
double ReadingPalette::contrast() const {
  double a = luminance(background), b = luminance(text);
  return (std::max(a, b) + .05) / (std::min(a, b) + .05);
}
} // namespace xfmd
