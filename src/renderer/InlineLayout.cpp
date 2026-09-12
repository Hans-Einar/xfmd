#include "InlineLayout.h"
#include <algorithm>
namespace xfmd {
namespace {
std::size_t nextCharacter(const std::string& text, std::size_t at) {
  ++at;
  while (at < text.size() && (static_cast<unsigned char>(text[at]) & 0xc0) == 0x80) ++at;
  return at;
}
SourceRange slice(const InlineRun& run, std::size_t begin, std::size_t end) {
  if (run.source.quality == MappingQuality::Exact && run.text.size() == run.source.end - run.source.begin)
    return {run.source.begin + begin, run.source.begin + end};
  // Mapping within transformed text is approximate, but anchored in its own source range.
  auto length = run.source.end - run.source.begin;
  auto divisor = std::max<std::size_t>(1, run.text.size());
  return {run.source.begin + length * begin / divisor, run.source.begin + length * end / divisor, MappingQuality::Approximate};
}
}
int InlineLayout::layout(const SemanticBlock& block, int left, int top, int width,
                         FontSpec base, ITextMetrics& metrics, RenderFrame& frame) {
  int x = left, y = top;
  auto defaultExtent = metrics.measure("M", base);
  int lineHeight = defaultExtent.height + 4, ascent = defaultExtent.ascent;
  std::size_t lineStart = frame.runs.size();
  auto finishLine = [&] {
    for (auto i = lineStart; i < frame.runs.size(); ++i) {
      auto& draw = frame.runs[i];
      draw.bounds.y += ascent - draw.ascent;
      frame.anchors.push_back({draw.source, draw.bounds});
    }
    y += lineHeight; x = left; lineStart = frame.runs.size();
    lineHeight = defaultExtent.height + 4; ascent = defaultExtent.ascent;
  };
  auto emit = [&](const InlineRun& run, std::size_t begin, std::size_t end, FontSpec font) {
    auto text = run.text.substr(begin, end - begin);
    auto extent = metrics.measure(text, font);
    DrawRun draw{text, font, {x, y, extent.width, extent.height}, extent.ascent,
                 slice(run, begin, end), run.link, run.code && block.kind != BlockKind::Code};
    frame.runs.push_back(std::move(draw));
    x += extent.width; frame.contentWidth = std::max(frame.contentWidth, x + 20);
    lineHeight = std::max(lineHeight, extent.height + 4); ascent = std::max(ascent, extent.ascent);
  };
  for (const auto& run : block.runs) {
    auto font = base; font.bold |= run.bold; font.italic |= run.italic; font.mono |= run.code;
    if (run.code) font.points = block.kind == BlockKind::Code ? 11 : base.points;
    for (std::size_t begin = 0; begin < run.text.size();) {
      char c = run.text[begin];
      if (c == '\r') { ++begin; continue; }
      if (c == '\n') { finishLine(); ++begin; continue; }
      if (c == '\t') {
        InlineRun spaces = run; spaces.text = "    "; spaces.source = slice(run, begin, begin + 1);
        spaces.source.quality = MappingQuality::Approximate;
        emit(spaces, 0, 4, font); ++begin; continue;
      }
      std::size_t end = begin + 1;
      if (c != ' ') while (end < run.text.size() && run.text[end] != ' ' && run.text[end] != '\n' && run.text[end] != '\r' && run.text[end] != '\t') ++end;
      auto word = std::string_view(run.text).substr(begin, end - begin);
      int measured = metrics.measure(word, font).width;
      bool code = block.kind == BlockKind::Code;
      if (!code && x > left && x + measured > left + width) finishLine();
      if (!code && c == ' ' && x == left) { begin = end; continue; }
      if (!code && measured > width) {
        // Split long unbroken words at UTF-8 boundaries, never in a multibyte character.
        while (begin < end) {
          std::size_t stop = nextCharacter(run.text, begin);
          while (stop < end) {
            auto next = nextCharacter(run.text, stop);
            if (metrics.measure(std::string_view(run.text).substr(begin, next - begin), font).width > width) break;
            stop = next;
          }
          emit(run, begin, stop, font); begin = stop;
          if (begin < end) finishLine();
        }
      } else { emit(run, begin, end, font); begin = end; }
      if (frame.runs.size() > 1000000) throw Error(ErrorCode::TooLarge, "Rendered document is too complex.");
    }
  }
  if (lineStart < frame.runs.size() || y == top) finishLine();
  return y - top;
}
}
