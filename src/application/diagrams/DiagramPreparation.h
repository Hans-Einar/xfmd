#pragma once
#include "DiagramCache.h"
#include "contracts/IInterpreter.h"
#include "contracts/diagram/IDiagramLayout.h"
namespace xfmd {
class DiagramPreparation {
  IDiagramLayout& layout;
  DiagramCache cache;

public:
  explicit DiagramPreparation(IDiagramLayout& value) : layout(value) {}
  ParseResult prepare(ParseResult, ITextMetrics&, const std::function<bool()>& = {});
};
} // namespace xfmd
