#pragma once
#include "contracts/IInterpreter.h"
#include "contracts/diagram/IDiagramInterpreter.h"
namespace xfmd {
class CmarkInterpreter final : public IInterpreter {
  std::shared_ptr<IDiagramInterpreter> diagrams;

public:
  explicit CmarkInterpreter(std::shared_ptr<IDiagramInterpreter> value = {})
      : diagrams(std::move(value)) {}
  ParseResult parse(const SourceSnapshot&, const ParseOptions& = {}) override;
};
} // namespace xfmd
