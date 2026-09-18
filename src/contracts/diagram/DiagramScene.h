#pragma once
#include "DiagramModel.h"
#include "contracts/RenderFrame.h"
namespace xfmd {
struct DiagramNodeBox {
  Rect bounds;
  DiagramShape shape;
};
struct DiagramEdgePath {
  std::vector<Point> points;
  bool start = false, end = false;
  unsigned style = 0;
};
struct DiagramLabel {
  std::string text;
  Point origin;
  TextExtent extent;
  bool background = false;
};
struct DiagramScene final : VisualResource {
  FontSetId fonts = 0;
  // Authoritative presentation. Geometry below is inspection/test metadata.
  std::string svg;
  std::string diagnostics;
  std::vector<DiagramNodeBox> nodes;
  std::vector<Rect> groups;
  std::vector<DiagramEdgePath> edges;
  std::vector<DiagramLabel> labels;
  std::size_t bytes = 0;
};
} // namespace xfmd
