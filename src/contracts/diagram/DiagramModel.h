#pragma once
#include "SequenceModel.h"
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
namespace xfmd {
enum class DiagramShape : std::uint32_t { Rectangle, Rounded, Diamond, Circle };
struct DiagramNode {
  std::string id, label;
  DiagramShape shape = DiagramShape::Rectangle;
};
struct DiagramEdge {
  std::uint32_t from = 0, to = 0;
  std::string label;
  bool arrowStart = false, arrowEnd = false;
  std::uint32_t style = 0;
};
struct DiagramGroup {
  std::string id, label;
  std::uint32_t direction = 4, parent = UINT32_MAX;
  std::vector<std::uint32_t> nodes;
};
struct DiagramModel {
  std::optional<SequenceModel> sequence;
  std::uint32_t direction = 0;
  std::vector<DiagramNode> nodes;
  std::vector<DiagramEdge> edges;
  std::vector<DiagramGroup> groups;
};
} // namespace xfmd
