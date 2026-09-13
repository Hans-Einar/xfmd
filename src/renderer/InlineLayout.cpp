#include "InlineLayout.h"
#include "LinkMarker.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
namespace {
std::size_t nextCharacter(std::string_view text, std::size_t at) {
  ++at;
  while (at < text.size() && (static_cast<unsigned char>(text[at]) & 0xc0) == 0x80)
    ++at;
  return at;
}
SourceRange slice(const InlineRun& run, std::size_t begin, std::size_t end) {
  if (run.source.quality == MappingQuality::Exact &&
      run.text.size() == run.source.end - run.source.begin)
    return {run.source.begin + begin, run.source.begin + end};
  // Mapping within transformed text is approximate, but anchored in its own source range.
  auto length = run.source.end - run.source.begin;
  auto divisor = std::max<std::size_t>(1, run.text.size());
  return {run.source.begin + length * begin / divisor, run.source.begin + length * end / divisor,
          MappingQuality::Approximate};
}
} // namespace
double InlineLayout::layout(const SemanticBlock& block, double left, double top, double width, FontSpec base,
                         ITextMetrics& metrics, RenderFrame& frame, bool wrapCode, const std::function<bool()>& cancelled) {
  double x = left, y = top;
  auto defaultExtent = metrics.measure("M", base);
  double lineHeight = defaultExtent.height + 4, ascent = defaultExtent.ascent;
  std::size_t lineStart = frame.runs.size();
  auto finishLine = [&] {
    frame.flow.lines.push_back({y,lineHeight,0});
    for (auto i = lineStart; i < frame.runs.size(); ++i) {
      auto& draw = frame.runs[i];
      draw.bounds.y += ascent - draw.ascent;
      frame.anchors.push_back({draw.source, {draw.bounds.x, y, draw.bounds.width, lineHeight}});
    }
    y += lineHeight;
    x = left;
    lineStart = frame.runs.size();
    lineHeight = defaultExtent.height + 4;
    ascent = defaultExtent.ascent;
  };
  auto emit = [&](const InlineRun& run, std::size_t begin, std::size_t end, FontSpec font,
                  const TextExtent* known = nullptr) {
    auto text = run.text.substr(begin, end - begin);
    auto extent = known ? *known : metrics.measure(text, font);
    DrawRun draw{text,
                 font,
                 {x, y, extent.width, extent.height},
                 extent.ascent,
                 slice(run, begin, end),
                 run.link,
                 run.code && block.kind != BlockKind::Code};
    draw.shaped=extent.shaped;
    if(draw.shaped)for(const auto& part:draw.shaped->segments)frame.glyphCount+=part.glyphs.size();
    if(frame.glyphCount>1000000 || frame.runs.size()>=100000)
      throw Error(ErrorCode::TooLarge,"Document exceeds the rendering complexity limit.");
    bool merged = false;
    if (frame.runs.size() > lineStart) {
      auto& previous = frame.runs.back();
      if (previous.icon == InlineIcon::None && previous.font == draw.font &&
          previous.link == draw.link && previous.codeBackground == draw.codeBackground &&
          previous.source.begin != previous.source.end &&
          previous.source.end == draw.source.begin &&
          previous.source.quality == draw.source.quality && previous.bounds.y == draw.bounds.y &&
          previous.ascent == draw.ascent && previous.bounds.height == draw.bounds.height) {
        if(draw.shaped) {
          if(previous.shapeParts.empty()) {previous.shapeParts.reserve(16);previous.text.reserve(128);}
          previous.shapeParts.push_back(draw.shaped);
        }
        previous.text += draw.text;
        previous.bounds.width += draw.bounds.width;
        previous.source.end = draw.source.end;
        merged = true;
      }
    }
    if (!merged)
      frame.runs.push_back(std::move(draw));
    x += extent.width;
    frame.contentWidth = std::max(frame.contentWidth, x + 20);
    lineHeight = std::max(lineHeight, extent.height + 4);
    ascent = std::max(ascent, extent.ascent);
  };
  std::string previousLink;
  std::size_t previousLinkId = 0;
  for (const auto& run : block.runs) {
    if (!run.link.empty() && (run.link != previousLink || run.linkId != previousLinkId)) {
      if (auto marker = LinkMarker::make(run, base, metrics)) {
        if (x > left && x + marker->bounds.width + metrics.measure("M", base).width > left + width)
          finishLine();
        marker->bounds.x = x;
        marker->bounds.y = y;
        x += marker->bounds.width;
        lineHeight = std::max(lineHeight, marker->bounds.height + 4);
        ascent = std::max(ascent, marker->ascent);
        frame.contentWidth = std::max(frame.contentWidth, x + 20);
        frame.runs.push_back(std::move(*marker));
      }
    }
    previousLink = run.link;
    previousLinkId = run.linkId;
    auto font = base;
    font.bold |= run.bold;
    font.italic |= run.italic;
    font.mono |= run.code;
    if (run.code)
      font.points = block.kind == BlockKind::Code ? 11 : base.points;
    const auto spaceExtent = metrics.measure(" ", font);
    for (std::size_t begin = 0; begin < run.text.size();) {
      if(cancelled && cancelled())throw Error(ErrorCode::Layout,"Layout cancelled.");
      char c = run.text[begin];
      if (c == '\r') {
        ++begin;
        continue;
      }
      if (c == '\n') {
        frame.anchors.push_back({slice(run, begin, begin + 1), {x, y, 1, lineHeight}});
        finishLine();
        ++begin;
        continue;
      }
      if (c == '\t') {
        InlineRun spaces = run;
        spaces.text = "    ";
        spaces.source = slice(run, begin, begin + 1);
        spaces.source.quality = MappingQuality::Approximate;
        if(wrapCode && x>left && x+4*spaceExtent.width>left+width)finishLine();
        emit(spaces, 0, 4, font);
        ++begin;
        continue;
      }
      std::size_t end = begin + 1;
      if (c != ' ')
        while (end < run.text.size() && run.text[end] != ' ' && run.text[end] != '\n' &&
               run.text[end] != '\r' && run.text[end] != '\t')
          ++end;
      auto word = std::string_view(run.text).substr(begin, end - begin);
      auto wordExtent = c == ' ' ? spaceExtent : metrics.measure(word, font);
      double measured = wordExtent.width;
      bool code = block.kind == BlockKind::Code && !wrapCode;
      if (!code && x > left && x + measured > left + width)
        finishLine();
      if (!code && block.kind != BlockKind::Code && c == ' ' && x == left) {
        begin = end;
        continue;
      }
      if (!code && measured > width) {
        // Shaper clusters preserve combining sequences and ligatures at visual wraps.
        std::vector<std::size_t> boundaries;
        if(auto shaped=metrics.shape(word,font)) {
          for(const auto& part:shaped->segments)for(const auto& glyph:part.glyphs)
            boundaries.push_back(part.byteOffset+glyph.cluster);
          boundaries.push_back(word.size());
          std::sort(boundaries.begin(),boundaries.end());
          boundaries.erase(std::unique(boundaries.begin(),boundaries.end()),boundaries.end());
        } else {
          for(std::size_t at=0;at<word.size();) {at=nextCharacter(word,at);boundaries.push_back(at);}
        }
        const auto wordStart=begin;
        while(begin<end) {
          auto lower=std::upper_bound(boundaries.begin(),boundaries.end(),begin-wordStart);
          if(lower==boundaries.end())throw Error(ErrorCode::Layout,"Invalid text cluster mapping.");
          // Binary search avoids quadratic shaping of long unbroken words.
          auto best=lower;auto hi=boundaries.end();
          while(lower<hi) {
            auto mid=lower+(hi-lower)/2;
            if(metrics.measure(std::string_view(run.text).substr(begin,wordStart+*mid-begin),font).width<=width) {
              best=mid;lower=mid+1;
            } else hi=mid;
          }
          const auto stop=wordStart+*best;
          emit(run,begin,stop,font);begin=stop;
          if(begin<end)finishLine();
        }
      } else {
        emit(run, begin, end, font, &wordExtent);
        begin = end;
      }
      if (frame.runs.size() > 1000000)
        throw Error(ErrorCode::TooLarge, "Rendered document is too complex.");
    }
  }
  if (lineStart < frame.runs.size() || y == top)
    finishLine();
  return y - top;
}
} // namespace xfmd
