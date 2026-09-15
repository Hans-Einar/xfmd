#include "PreviewSelection.h"
#include <algorithm>
#include <cmath>
#include <limits>
namespace xfmd {
namespace {
struct Cluster {
  std::size_t begin, end;
  double x, width;
  bool rtl;
};
std::vector<Cluster> clusters(const RenderFrame& frame, const DrawRun& run) {
  std::vector<Cluster> result;
  double x = run.bounds.x;
  std::size_t offset = run.textBegin;
  auto append = [&](const ShapedText& shape) {
    for (const auto& segment : shape.segments) {
      std::vector<std::size_t> ends{segment.text.size()};
      for (const auto& glyph : segment.glyphs)
        ends.push_back(glyph.cluster);
      std::sort(ends.begin(), ends.end());
      for (const auto& glyph : segment.glyphs) {
        auto end = *std::upper_bound(ends.begin(), ends.end(), glyph.cluster);
        const auto begin = offset + segment.byteOffset + glyph.cluster;
        if (!result.empty() && result.back().begin == begin) {
          result.back().width += (glyph.advance * run.textScale);
        } else
          result.push_back({begin, offset + segment.byteOffset + end, x,
                            (glyph.advance * run.textScale), bool(segment.level & 1)});
        x += (glyph.advance * run.textScale);
      }
    }
    for (const auto& segment : shape.segments)
      offset += segment.text.size();
  };
  if (run.shaped)
    append(*run.shaped);
  for (std::size_t i = 0; i < run.shapeCount; ++i)
    append(*frame.shapeParts.at(run.shapeBegin + i));
  if (result.empty() && run.textBegin != std::string::npos)
    result.push_back({run.textBegin, run.textEnd, x, run.bounds.width, false});
  for (auto& cluster : result) {
    cluster.begin = std::min(cluster.begin, run.textEnd);
    cluster.end = std::min(cluster.end, run.textEnd);
  }
  return result;
}
} // namespace
std::string PreviewSelection::text(const RenderFrame& frame) const {
  auto begin = std::min({anchor, caret, frame.readingText.size()});
  auto end = std::min(std::max(anchor, caret), frame.readingText.size());
  return frame.readingText.substr(begin, end - begin);
}
std::size_t PreviewSelection::hit(const RenderFrame& frame, Point point) {
  const DrawRun* nearest = nullptr;
  double distance = std::numeric_limits<double>::max();
  for (const auto& run : frame.runs) {
    if (run.textBegin == std::string::npos)
      continue;
    double dy = std::max({run.bounds.y - point.y, point.y - run.bounds.y - run.bounds.height, 0.0});
    double dx = std::max({run.bounds.x - point.x, point.x - run.bounds.x - run.bounds.width, 0.0});
    double score = dy * dy * 10000 + dx * dx;
    if (score < distance) {
      distance = score;
      nearest = &run;
    }
  }
  if (!nearest)
    return 0;
  std::size_t position = nearest->textBegin;
  double closest = std::numeric_limits<double>::max();
  for (const auto& cluster : clusters(frame, *nearest)) {
    for (int side = 0; side < 2; ++side) {
      auto delta = std::abs(point.x - (cluster.x + side * cluster.width));
      if (delta < closest) {
        closest = delta;
        position = (bool(side) != cluster.rtl) ? cluster.end : cluster.begin;
      }
    }
  }
  return std::min(position, frame.readingText.size());
}
std::vector<Rect> PreviewSelection::rectangles(const RenderFrame& frame) const {
  std::vector<Rect> result;
  if (empty())
    return result;
  auto begin = std::min(anchor, caret), end = std::max(anchor, caret);
  for (const auto& run : frame.runs) {
    if (run.textBegin == std::string::npos || run.textEnd <= begin || run.textBegin >= end)
      continue;
    for (const auto& cluster : clusters(frame, run)) {
      if (cluster.end <= begin || cluster.begin >= end)
        continue;
      Rect box{cluster.x, run.bounds.y, cluster.width, run.bounds.height};
      if (!result.empty() && result.back().y == box.y && result.back().height == box.height &&
          std::abs(result.back().x + result.back().width - box.x) < .01)
        result.back().width += box.width;
      else
        result.push_back(box);
    }
  }
  return result;
}
} // namespace xfmd
