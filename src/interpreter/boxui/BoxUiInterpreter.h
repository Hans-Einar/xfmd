#pragma once
#include "contracts/boxui/IBoxUiInterpreter.h"
#include "contracts/diagram/IDiagramInterpreter.h"
namespace xfmd {
class BoxUiInterpreter final : public IBoxUiInterpreter {
  std::shared_ptr<IDiagramInterpreter> diagrams;

public:
  explicit BoxUiInterpreter(std::shared_ptr<IDiagramInterpreter> value)
      : diagrams(std::move(value)) {}
  std::shared_ptr<const BoxUiModel> parse(const std::string&) override;
};
} // namespace xfmd
