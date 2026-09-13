#pragma once
#include <string_view>
#include "ITextShaper.h"
#include <tuple>
namespace xfmd {
struct FontSpec {
  int points = 12;
  bool bold = false, italic = false, mono = false;
  auto key() const { return std::make_tuple(points, bold, italic, mono); }
  bool operator<(const FontSpec& other) const { return key() < other.key(); }
  bool operator==(const FontSpec& other) const { return key() == other.key(); }
};
struct TextExtent {
  TextExtent(LayoutUnit w=0,LayoutUnit h=0,LayoutUnit a=0):width(w),height(h),ascent(a){}
  LayoutUnit width = 0, height = 0, ascent = 0;
};
class ITextMetrics : public ITextShaper {
public:
  virtual ~ITextMetrics() = default;
  std::shared_ptr<const ShapedText> shape(std::string_view, FontSpec) override { return {}; }
  virtual FontSetId fontSetId() const { return 0; }
  virtual TextExtent measure(std::string_view, FontSpec) = 0;
};
} // namespace xfmd
