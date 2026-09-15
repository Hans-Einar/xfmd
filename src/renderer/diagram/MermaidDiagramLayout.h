#pragma once
#include "contracts/diagram/IDiagramLayout.h"
namespace xfmd {
class MermaidDiagramLayout final : public IDiagramLayout {
public:
  std::shared_ptr<const DiagramScene> layout(const DiagramModel&, const DiagramLayoutRequest&,
                                             ITextMetrics&) override;
};
} // namespace xfmd
