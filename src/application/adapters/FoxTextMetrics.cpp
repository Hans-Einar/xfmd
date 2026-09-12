#include "FoxTextMetrics.h"
#include <algorithm>
namespace xfmd {
FX::FXFont* FoxTextMetrics::font(FontSpec spec, std::string_view text) {
  auto create = [&](const std::string& family) {
    auto key = std::make_pair(spec, family);
    auto it = fonts.find(key);
    if (it != fonts.end())
      return it->second.get();
    auto font =
        std::make_unique<FX::FXFont>(&app, family.c_str(), std::clamp(spec.points, 6, 72),
                                     spec.bold ? FX::FXFont::Bold : FX::FXFont::Normal,
                                     spec.italic ? FX::FXFont::Italic : FX::FXFont::Straight);
    font->create();
    auto* result = font.get();
    fonts.emplace(std::move(key), std::move(font));
    return result;
  };
  auto supports = [&](FX::FXFont* font) {
    for (std::size_t i = 0; i < text.size();) {
      auto c = static_cast<unsigned char>(text[i++]);
      unsigned point = c, count = 0;
      if ((c & 0xe0) == 0xc0) {
        point = c & 31;
        count = 1;
      } else if ((c & 0xf0) == 0xe0) {
        point = c & 15;
        count = 2;
      } else if ((c & 0xf8) == 0xf0) {
        point = c & 7;
        count = 3;
      }
      while (count-- && i < text.size())
        point = (point << 6) | (static_cast<unsigned char>(text[i++]) & 63);
      if (!font->hasChar(point))
        return false;
    }
    return true;
  };
  auto* primary = create(spec.mono ? "DejaVu Sans Mono" : "DejaVu Sans");
  if (supports(primary))
    return primary;
  for (const char* family : {"Noto Sans CJK JP", "Droid Sans Fallback", "sans"}) {
    auto* fallback = create(family);
    if (supports(fallback))
      return fallback;
  }
  return primary;
}
std::vector<std::pair<std::string_view, FX::FXFont*>>
FoxTextMetrics::segments(std::string_view text, FontSpec spec) {
  std::vector<std::pair<std::string_view, FX::FXFont*>> result;
  auto* primary = font(spec);
  bool ascii = std::all_of(text.begin(), text.end(), [](unsigned char c) { return c < 128; });
  if (ascii) {
    result.push_back({text, primary});
    return result;
  }
  std::size_t begin = 0;
  FX::FXFont* current = nullptr;
  for (std::size_t i = 0; i < text.size();) {
    auto start = i++;
    while (i < text.size() && (static_cast<unsigned char>(text[i]) & 0xc0) == 0x80)
      ++i;
    auto* chosen = font(spec, text.substr(start, i - start));
    if (current && chosen != current) {
      result.push_back({text.substr(begin, start - begin), current});
      begin = start;
    }
    current = chosen;
  }
  if (current)
    result.push_back({text.substr(begin), current});
  return result;
}
TextExtent FoxTextMetrics::measure(std::string_view text, FontSpec spec) {
  auto& cache = measurements[spec];
  auto key = std::string(text);
  auto found = cache.find(key);
  if (found != cache.end())
    return found->second;
  TextExtent result;
  int descent = 0;
  for (const auto& segment : segments(text, spec)) {
    result.width += segment.second->getTextWidth(segment.first.data(), int(segment.first.size()));
    descent = std::max(descent, segment.second->getFontDescent());
    result.ascent = std::max(result.ascent, segment.second->getFontAscent());
  }
  result.height = result.ascent + descent;
  if (cachedCount < 4096 && text.size() <= 256) {
    cache.emplace(std::move(key), result);
    ++cachedCount;
  }
  return result;
}
} // namespace xfmd
