#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
// Logical UTF-8 positions remain independent of wrapping, page gaps and decoration.
class PreviewSelection {
  std::size_t anchor = 0, caret = 0;

public:
  void clear() { anchor = caret = 0; }
  void start(std::size_t at) { anchor = caret = at; }
  void extend(std::size_t at) { caret = at; }
  void all(const RenderFrame& frame) {
    anchor = 0;
    caret = frame.readingText.size();
  }
  bool empty() const { return anchor == caret; }
  std::string text(const RenderFrame&) const;
  std::vector<Rect> rectangles(const RenderFrame&) const;
  static std::size_t hit(const RenderFrame&, Point);
};
} // namespace xfmd
