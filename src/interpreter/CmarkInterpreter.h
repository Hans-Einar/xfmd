#pragma once
#include "contracts/IInterpreter.h"
#include "contracts/boxui/IBoxUiInterpreter.h"
#include "contracts/diagram/IDiagramInterpreter.h"
namespace xfmd {
class CmarkInterpreter final : public IInterpreter {
  std::shared_ptr<IDiagramInterpreter> diagrams;
  std::shared_ptr<IBoxUiInterpreter> boxUi;

public:
  explicit CmarkInterpreter(std::shared_ptr<IDiagramInterpreter> value = {},
                            std::shared_ptr<IBoxUiInterpreter> boxes = {})
      : diagrams(std::move(value)), boxUi(std::move(boxes)) {}
  ParseResult parse(const SourceSnapshot&, const ParseOptions& = {}) override;
};
} // namespace xfmd
