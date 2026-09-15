#pragma once
#include "DocumentTypes.h"
#include "ITextMetrics.h"
#include "LayoutProfile.h"
#include "PageLayout.h"
#include "VisualResource.h"
#include <vector>
namespace xfmd {
struct Point {
  Point(LayoutUnit x = 0, LayoutUnit y = 0) : x(x), y(y) {}
  LayoutUnit x = 0, y = 0;
};
struct Rect {
  Rect(LayoutUnit x = 0, LayoutUnit y = 0, LayoutUnit w = 0, LayoutUnit h = 0)
      : x(x), y(y), width(w), height(h) {}
  LayoutUnit x = 0, y = 0, width = 0, height = 0;
  bool contains(Point p) const {
    return p.x >= x && p.y >= y && p.x < x + width && p.y < y + height;
  }
};
enum class InlineIcon { None, ExternalLink };
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
  std::size_t shapeBegin = 0, shapeCount = 0;
  std::shared_ptr<const VisualResource> visual{};
  double textScale = 1;
  std::size_t textBegin = std::string::npos, textEnd = std::string::npos;
};
enum class DecorationRole { Border, Surface, Alternate, Background };
struct Decoration {
  Rect bounds;
  std::uint32_t color = 0;
  DecorationRole role = DecorationRole::Border;
};
struct AnchorRegion {
  SourceRange source;
  Rect bounds;
  std::size_t pageIndex = 0;
};
struct RenderFrame {
  DocumentToken token;
  std::uint64_t generation = 0;
  LayoutUnit width = 0, height = 0, contentWidth = 0, maxRunHeight = 150;
  FrameKey key;
  FlowLayout flow;
  PageLayout pages;
  std::size_t glyphCount = 0;
  std::string readingText;
  std::vector<DrawRun> runs;
  std::vector<std::shared_ptr<const ShapedText>> shapeParts;
  std::vector<Decoration> decorations;
  std::vector<AnchorRegion> anchors;
};
struct LayoutRequest {
  LayoutRequest(LayoutUnit w = 800, std::uint64_t g = 0, LayoutProfile p = {},
                std::function<bool()> cancel = {})
      : width(w), generation(g), profile(p), cancelled(std::move(cancel)) {}
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
