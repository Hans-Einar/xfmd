#pragma once
#include "contracts/diagram/DiagramScene.h"
#include <map>
namespace xfmd {
struct DiagramText {
  std::vector<DiagramLabel> lines;
  double width = 0, height = 0;
};
class DiagramTextLayout {
public:
  static std::map<std::string, DiagramText> measure(const DiagramModel&, ITextMetrics&);
};
} // namespace xfmd
