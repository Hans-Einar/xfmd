#include "BlockLayout.h"
#include "InlineLayout.h"
#include "TableLayout.h"
#include "diagram/DiagramPlacement.h"
#include <algorithm>
namespace xfmd {
void BlockLayout::layout(const SemanticDocument& model, const LayoutRequest& request,
                         ITextMetrics& metrics, RenderFrame& frame) {
  double y = 20;
  for (const auto& block : model.blocks) {
    if (request.cancelled && request.cancelled())
      throw Error(ErrorCode::Layout, "Layout cancelled.");
    auto firstLine = frame.flow.lines.size();
    FontSpec font;
    if (block.kind == BlockKind::Heading) {
      const int sizes[] = {26, 22, 19, 16, 14, 12};
      font.points = sizes[std::clamp(block.level, 1, 6) - 1];
      font.bold = true;
      y += 6;
    }
    if (block.kind == BlockKind::Code) {
      font.mono = true;
      font.points = 11;
    }
    const double gutter =
        request.profile.mode == LayoutMode::Continuous ? std::min(24.0, request.width * .15) : 24;
    double left = gutter + block.indent * 24 + block.quoteDepth * 16;
    double width = std::max(1.0, request.width - left - gutter);
    double start = y;
    if (block.kind == BlockKind::Diagram && block.diagramScene) {
      y += DiagramPlacement::append(block, left, y, width, request, metrics, frame);
    } else if (block.kind == BlockKind::Table && block.table) {
      y += TableLayout::layout(*block.table, left, y, width, metrics, frame, request);
    } else if (block.kind == BlockKind::Rule) {
      frame.decorations.push_back({{left, y + 8, width, 1}, 0xb8bec7});
      frame.flow.lines.push_back({y, 20, 0});
      y += 20;
    } else {
      if (!block.marker.empty()) {
        auto size = metrics.measure(block.marker, font);
        frame.runs.push_back({block.marker,
                              font,
                              {left - size.width - 8, y, size.width, size.height},
                              size.ascent,
                              block.source,
                              {},
                              false});
        frame.runs.back().shaped = size.shaped;
      }
      y += InlineLayout::layout(block, left, y, width, font, metrics, frame,
                                request.profile.mode == LayoutMode::Paged, request.cancelled,
                                ColumnAlignment::Left, std::min(20.0, gutter));
      if (block.kind == BlockKind::Code)
        frame.decorations.push_back(
            {{left - 6, start - 4, std::max(width + 12, frame.contentWidth - left), y - start + 8},
             0xf0f2f5,
             DecorationRole::Surface});
      for (int depth = 0; depth < block.quoteDepth; ++depth)
        frame.decorations.push_back({{left - 12 - 16 * depth, start, 3, y - start}, 0xc4cbd5});
    }
    const auto count = frame.flow.lines.size() - firstLine;
    for (std::size_t i = firstLine; i < frame.flow.lines.size(); ++i) {
      if (block.kind == BlockKind::Table)
        continue;
      if (block.kind == BlockKind::Heading)
        frame.flow.lines[i].keepFollowing = 2;
      else if (count > 1 && (i == firstLine || i + 2 == frame.flow.lines.size()))
        frame.flow.lines[i].keepFollowing = 1;
    }
    // Block region covers hidden syntax, empty blocks and unpainted whitespace.
    frame.anchors.push_back({block.source, {left, start, width, std::max(1.0, y - start)}});
    y += block.indent ? 5 : 10;
  }
  frame.height = std::max(60.0, y + 20);
}
} // namespace xfmd
