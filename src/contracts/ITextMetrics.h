#pragma once
#include <string_view>
#include <tuple>
namespace xfmd {
struct FontSpec {
  int points = 12;
  bool bold = false, italic = false, mono = false;
  auto key() const { return std::make_tuple(points, bold, italic, mono); }
  bool operator<(const FontSpec& other) const { return key() < other.key(); }
  bool operator==(const FontSpec& other) const { return key() == other.key(); }
};
struct TextExtent { int width = 0, height = 0, ascent = 0; };
class ITextMetrics {
public:
  virtual ~ITextMetrics() = default;
  virtual TextExtent measure(std::string_view, FontSpec) = 0;
};
} // namespace xfmd
