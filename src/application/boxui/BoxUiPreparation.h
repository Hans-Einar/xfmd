#pragma once
#include "contracts/IInterpreter.h"
#include "contracts/boxui/BoxUiFrame.h"
#include "contracts/diagram/IDiagramLayout.h"
namespace xfmd {
struct BoxUiPreparation {
  static ParseResult prepare(ParseResult, ITextMetrics&, IDiagramLayout&, const BoxUiState&,
                             const std::function<bool()>&, double width = 640);
};
} // namespace xfmd
