#include "TableLayout.h"
#include "InlineLayout.h"
#include <algorithm>
#include <numeric>
namespace xfmd {
namespace {
constexpr double padding = 5, minimum = 34;
// Bounded preferred-width estimate; actual wrapping still shapes every character.
double preferred(const TableCell& cell, bool header, ITextMetrics& metrics) {
  double width = 2 * padding;
  for (const auto& run : cell.runs) {
    FontSpec font;
    font.bold = header || run.bold;
    font.italic = run.italic;
    font.mono = run.code;
    auto size = std::min<std::size_t>(256, run.text.size());
    while (size < run.text.size() && size &&
           (static_cast<unsigned char>(run.text[size]) & 0xc0) == 0x80)
      --size;
    width += metrics.measure(std::string_view(run.text).substr(0, size), font).width;
    if (!run.link.empty())
      width += metrics.measure("↗ ", font).width;
    if (width >= 240)
      return 240;
  }
  return std::max(minimum, width);
}
} // namespace
double TableLayout::layout(const SemanticTable& table, double left, double top, double width,
                           ITextMetrics& metrics, RenderFrame& frame,
                           const LayoutRequest& request) {
  const auto columns = table.alignments.size();
  if (!columns || columns > 64)
    throw Error(ErrorCode::Layout, "Tables support between 1 and 64 columns.");
  const double tableWidth = std::max(width, minimum * columns);
  if (request.profile.mode == LayoutMode::Paged && tableWidth > width + .01)
    throw Error(
        ErrorCode::Layout,
        "Table columns do not fit A4. Reduce margins or split the table into fewer columns.");
  std::vector<double> widths(columns, minimum), minima(columns, minimum);
  std::size_t cells = 0;
  for (const auto& row : table.rows) {
    if (request.cancelled && request.cancelled())
      throw Error(ErrorCode::Layout, "Layout cancelled.");
    cells += row.cells.size();
    if (cells > 50000)
      throw Error(ErrorCode::TooLarge, "Table exceeds 50000 cells.");
    for (std::size_t i = 0; i < std::min(columns, row.cells.size()); ++i) {
      const double wanted = preferred(row.cells[i], row.header, metrics);
      widths[i] = std::max(widths[i], wanted);
      if (row.header)
        minima[i] = std::min(96.0, wanted);
    }
  }
  const double base = minimum * columns;
  double headerExtra = std::accumulate(minima.begin(), minima.end(), 0.0) - base;
  if (headerExtra > tableWidth - base && headerExtra > 0)
    for (auto& w : minima)
      w = minimum + (w - minimum) * (tableWidth - base) / headerExtra;
  double floor = std::accumulate(minima.begin(), minima.end(), 0.0);
  double desired = std::accumulate(widths.begin(), widths.end(), 0.0) - floor;
  for (std::size_t i = 0; i < columns; ++i)
    widths[i] = minima[i] + (desired > 0 ? (widths[i] - minima[i]) / desired : 1.0 / columns) *
                                (tableWidth - floor);
  double y = top;
  for (std::size_t rowIndex = 0; rowIndex < table.rows.size(); ++rowIndex) {
    if (request.cancelled && request.cancelled())
      throw Error(ErrorCode::Layout, "Layout cancelled.");
    const auto& row = table.rows[rowIndex];
    const auto firstRun = frame.runs.size(), firstLine = frame.flow.lines.size();
    const auto firstAnchor = frame.anchors.size();
    double x = left, height = 0;
    for (std::size_t i = 0; i < columns; ++i) {
      SemanticBlock block;
      block.source = row.source;
      if (i < row.cells.size()) {
        block.source = row.cells[i].source;
        block.runs = row.cells[i].runs;
      }
      FontSpec font;
      font.bold = row.header;
      double h =
          InlineLayout::layout(block, x + padding, y + padding, widths[i] - 2 * padding, font,
                               metrics, frame, true, request.cancelled, table.alignments[i]);
      height = std::max(height, h + 2 * padding);
      frame.flow.lines.resize(firstLine); // The whole row is one pagination unit.
      frame.anchors.push_back({block.source, {x, y, widths[i], h + 2 * padding}});
      x += widths[i];
    }
    if (request.profile.mode == LayoutMode::Paged &&
        height > request.profile.paper.height - 2 * request.profile.paper.margin)
      throw Error(ErrorCode::Layout, "A table row is taller than the printable page. Shorten it or "
                                     "split it into multiple rows.");
    frame.flow.lines.push_back({y, height, row.header ? 1u : 0u});
    std::sort(
        frame.runs.begin() + firstRun, frame.runs.end(), [](const DrawRun& a, const DrawRun& b) {
          return a.bounds.y < b.bounds.y || (a.bounds.y == b.bounds.y && a.bounds.x < b.bounds.x);
        });
    // Full cell regions cover empty cells and padding, in addition to inline anchors.
    for (auto a = firstAnchor; a < frame.anchors.size(); ++a)
      if (frame.anchors[a].bounds.y == y)
        frame.anchors[a].bounds.height = height;
    frame.anchors.push_back({row.source, {left, y, tableWidth, height}});
    frame.decorations.push_back({{left, y, tableWidth, height},
                                 row.header     ? 0xe9edf3u
                                 : rowIndex % 2 ? 0xffffffu
                                                : 0xf7f9fbu});
    auto edge = [&](double xx, double yy, double w, double h) {
      frame.decorations.push_back({{xx, yy, w, h}, 0xb8bec7});
    };
    edge(left, y, tableWidth, .6);
    edge(left, y + height - .6, tableWidth, .6);
    x = left;
    for (auto w : widths) {
      edge(x, y, .6, height);
      x += w;
    }
    edge(left + tableWidth - .6, y, .6, height);
    y += height;
  }
  frame.contentWidth = std::max(frame.contentWidth, left + tableWidth + 24);
  return y - top;
}
} // namespace xfmd
