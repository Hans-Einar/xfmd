#include "BlockLayout.h"
#include "InlineLayout.h"
#include <algorithm>
namespace xfmd {
void BlockLayout::layout(const SemanticDocument& model, const LayoutRequest& request, ITextMetrics& metrics, RenderFrame& frame) {
  int y = 20;
  for (const auto& block : model.blocks) {
    FontSpec font;
    if (block.kind == BlockKind::Heading) {
      const int sizes[] = {26, 22, 19, 16, 14, 12};
      font.points = sizes[std::clamp(block.level, 1, 6) - 1]; font.bold = true;
      y += 6;
    }
    if (block.kind == BlockKind::Code) { font.mono = true; font.points = 11; }
    int left = 24 + block.indent * 24 + block.quoteDepth * 16;
    int width = std::max(40, request.width - left - 24);
    int start = y;
    if (block.kind == BlockKind::Rule) {
      frame.decorations.push_back({{left, y + 8, width, 1}, 0xb8bec7}); y += 20;
    } else {
      if (!block.marker.empty()) {
        auto size = metrics.measure(block.marker, font);
        frame.runs.push_back({block.marker, font, {left - size.width - 8, y, size.width, size.height}, size.ascent, block.source, {}, false});
      }
      y += InlineLayout::layout(block, left, y, width, font, metrics, frame);
      if (block.kind == BlockKind::Code) frame.decorations.push_back({{left - 6, start - 4, std::max(width + 12, frame.contentWidth - left), y - start + 8}, 0xf0f2f5});
      for (int depth = 0; depth < block.quoteDepth; ++depth)
        frame.decorations.push_back({{left - 12 - 16 * depth, start, 3, y - start}, 0xc4cbd5});
    }
    // Block region covers hidden syntax, empty blocks and unpainted whitespace.
    frame.anchors.push_back({block.source, {left, start, width, std::max(1, y - start)}});
    y += block.indent ? 5 : 10;
  }
  frame.height = std::max(60, y + 20);
  std::stable_sort(frame.runs.begin(), frame.runs.end(), [](const DrawRun& a, const DrawRun& b) { return a.bounds.y < b.bounds.y; });
}
}
