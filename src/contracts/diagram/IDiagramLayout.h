#pragma once
#include "DiagramScene.h"
namespace xfmd {
struct DiagramLayoutRequest {
  std::function<bool()> cancelled;
};
class IDiagramLayout {
public:
  virtual ~IDiagramLayout() = default;
  virtual std::shared_ptr<const DiagramScene>
  layout(const DiagramModel&, const DiagramLayoutRequest&, ITextMetrics&) = 0;
};
} // namespace xfmd
