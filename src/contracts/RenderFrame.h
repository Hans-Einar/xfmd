#pragma once
#include "DocumentTypes.h"
#include "ITextMetrics.h"
#include "LayoutProfile.h"
#include <vector>
namespace xfmd {
struct Point {
  Point(LayoutUnit x=0,LayoutUnit y=0):x(x),y(y){}
  LayoutUnit x = 0, y = 0;
};
struct Rect {
  Rect(LayoutUnit x=0,LayoutUnit y=0,LayoutUnit w=0,LayoutUnit h=0):x(x),y(y),width(w),height(h){}
  LayoutUnit x = 0, y = 0, width = 0, height = 0;
  bool contains(Point p) const {
    return p.x >= x && p.y >= y && p.x < x + width && p.y < y + height;
  }
};
enum class InlineIcon { None, Globe };
struct DrawRun {
  std::string text;
  FontSpec font;
  Rect bounds;
  LayoutUnit ascent = 0;
  SourceRange source;
  std::string link;
  bool codeBackground = false;
  InlineIcon icon = InlineIcon::None;
  std::shared_ptr<const ShapedText> shaped{};
};
struct Decoration {
  Rect bounds;
  std::uint32_t color = 0;
};
struct AnchorRegion {
  SourceRange source;
  Rect bounds;
};
struct RenderFrame {
  DocumentToken token;
  std::uint64_t generation = 0;
  LayoutUnit width = 0, height = 0, contentWidth = 0;
  FrameKey key;
  std::vector<DrawRun> runs;
  std::vector<Decoration> decorations;
  std::vector<AnchorRegion> anchors;
};
struct LayoutRequest {
  LayoutRequest(LayoutUnit w=800,std::uint64_t g=0,LayoutProfile p={},std::function<bool()> cancel={}):width(w),generation(g),profile(p),cancelled(std::move(cancel)){}
  LayoutUnit width = 800;
  std::uint64_t generation = 0;
  LayoutProfile profile;
  std::function<bool()> cancelled;
};
struct HitResult {
  std::string link;
  SourceRange source;
};
} // namespace xfmd
