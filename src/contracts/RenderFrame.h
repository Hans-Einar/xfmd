#pragma once
#include "DocumentTypes.h"
#include "ITextMetrics.h"
#include <vector>
namespace xfmd {
struct Point {
  int x = 0, y = 0;
};
struct Rect {
  int x = 0, y = 0, width = 0, height = 0;
  bool contains(Point p) const {
    return p.x >= x && p.y >= y && p.x < x + width && p.y < y + height;
  }
};
enum class InlineIcon { None, Globe };
struct DrawRun {
  std::string text;
  FontSpec font;
  Rect bounds;
  int ascent = 0;
  SourceRange source;
  std::string link;
  bool codeBackground = false;
  InlineIcon icon = InlineIcon::None;
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
  int width = 0, height = 0, contentWidth = 0;
  std::vector<DrawRun> runs;
  std::vector<Decoration> decorations;
  std::vector<AnchorRegion> anchors;
};
struct LayoutRequest {
  int width = 800;
  std::uint64_t generation = 0;
};
struct HitResult {
  std::string link;
  SourceRange source;
};
} // namespace xfmd
