#include "ThemeProfiles.h"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <stdexcept>
using namespace FX;
namespace xfmd {
ThemeProfiles::ThemeProfiles()
    : light{FXRGB(237, 241, 245), FXRGB(247, 249, 251), FXRGB(255, 255, 255), FXRGB(39, 52, 70),
            FXRGB(100, 114, 135), FXRGB(202, 211, 222), FXRGB(36, 92, 190),   FXRGB(220, 232, 250),
            FXRGB(224, 230, 239), FXRGB(160, 35, 40)},
      dark{FXRGB(37, 44, 53),    FXRGB(32, 38, 46),   FXRGB(25, 31, 39),    FXRGB(229, 234, 242),
           FXRGB(164, 175, 191), FXRGB(71, 83, 97),   FXRGB(139, 184, 255), FXRGB(54, 75, 105),
           FXRGB(54, 64, 78),    FXRGB(255, 145, 145)},
      comfortable{8, 12, 34, 5, 18}, compact{4, 8, 28, 3, 16} {}
UiMetrics ThemeProfiles::metrics(const Appearance& a) const {
  auto m = a.compact ? compact : comfortable;
  const double factor = a.fontSize / 10.0;
  for (auto* value : {&m.gap, &m.inset, &m.height, &m.radius, &m.iconSize})
    *value = std::max(1, int(std::lround(*value * factor)));
  return m;
}
bool ThemeProfiles::load(const std::string& path, std::string& error) {
  try {
    ThemeProfiles next;
    if (!std::filesystem::exists(path)) {
      *this = next;
      error.clear();
      return true;
    }
    if (std::filesystem::file_size(path) > 65536)
      throw std::runtime_error("Appearance file is larger than 64 KiB.");
    FXSettings settings;
    if (!settings.parseFile(path.c_str(), true))
      throw std::runtime_error("Cannot read appearance file.");
    auto color = [&](const char* section, const char* key, FXColor& result) {
      const char* raw = settings.readStringEntry(section, key, nullptr);
      if (!raw)
        return;
      std::string text(raw);
      if (text.size() != 7 || text[0] != '#' ||
          text.find_first_not_of("0123456789abcdefABCDEF", 1) != std::string::npos)
        throw std::runtime_error(std::string(section) + "." + key + " must be #RRGGBB.");
      auto value = std::stoul(text.substr(1), nullptr, 16);
      result = FXRGB(value >> 16, (value >> 8) & 255, value & 255);
    };
    for (auto entry :
         {std::pair<const char*, UiPalette*>{"light", &next.light}, {"dark", &next.dark}}) {
      auto& p = *entry.second;
      color(entry.first, "surface", p.surface);
      color(entry.first, "panel", p.panel);
      color(entry.first, "field", p.field);
      color(entry.first, "text", p.text);
      color(entry.first, "muted", p.muted);
      color(entry.first, "border", p.border);
      color(entry.first, "accent", p.accent);
      color(entry.first, "selected", p.selected);
      color(entry.first, "hover", p.hover);
      color(entry.first, "danger", p.danger);
    }
    auto number = [&](const char* section, const char* key, int& result, int low, int high) {
      const char* raw = settings.readStringEntry(section, key, nullptr);
      if (!raw)
        return;
      std::size_t used = 0;
      std::string text(raw);
      int value = std::stoi(text, &used);
      if (used != text.size() || value < low || value > high)
        throw std::runtime_error(std::string(section) + "." + key +
                                 " is outside its supported range.");
      result = value;
    };
    for (auto entry : {std::pair<const char*, UiMetrics*>{"comfortable", &next.comfortable},
                       {"compact", &next.compact}}) {
      auto& m = *entry.second;
      number(entry.first, "gap", m.gap, 2, 16);
      number(entry.first, "inset", m.inset, 4, 24);
      number(entry.first, "height", m.height, 24, 48);
      number(entry.first, "radius", m.radius, 0, 10);
      number(entry.first, "iconSize", m.iconSize, 14, 32);
    }
    *this = next;
    error.clear();
    return true;
  } catch (const std::exception& e) {
    error = e.what();
    return false;
  }
}
} // namespace xfmd
