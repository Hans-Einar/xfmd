#pragma once
#include "contracts/diagram/IDiagramInterpreter.h"
namespace xfmd {
class MermaidInterpreter final : public IDiagramInterpreter {
public:
  DiagramParseResult parse(const DiagramSource&) override;
};
} // namespace xfmd
