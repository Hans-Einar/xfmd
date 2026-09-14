#pragma once
#include "application/preferences/Appearance.h"
#include <fx.h>
#include <string>
namespace xfmd {
struct UiPalette {
  FX::FXColor surface, panel, field, text, muted, border, accent, selected, hover, danger;
};
struct UiMetrics {
  int gap, inset, height, radius, iconSize;
};
class ThemeProfiles {
  UiPalette light, dark;
  UiMetrics comfortable, compact;

public:
  ThemeProfiles();
  bool load(const std::string& path, std::string& error);
  const UiPalette& palette(const Appearance& a) const { return a.theme == "dark" ? dark : light; }
  UiMetrics metrics(const Appearance&) const;
};
} // namespace xfmd
