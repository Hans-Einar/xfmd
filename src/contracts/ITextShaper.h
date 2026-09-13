#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
namespace xfmd {
using LayoutUnit=double; // Physical points, 1/72 inch.
using FontSetId=std::uint64_t;
struct Glyph {
  std::uint32_t index=0,cluster=0;
  LayoutUnit advance=0,xOffset=0,yOffset=0;
};
struct GlyphSegment {
  std::string text,fontFace;
  std::vector<Glyph> glyphs;
  unsigned level=0;
  std::size_t byteOffset=0;
};
struct ShapedText {
  LayoutUnit width=0,height=0,ascent=0;
  std::vector<GlyphSegment> segments;
};
struct FontSpec;
class ITextShaper {
public:
  virtual ~ITextShaper()=default;
  virtual std::shared_ptr<const ShapedText> shape(std::string_view,FontSpec)=0;
};
} // namespace xfmd
