#pragma once
#include "contracts/IInterpreter.h"
namespace xfmd {
class CmarkInterpreter final : public IInterpreter {
public:
  ParseResult parse(const SourceSnapshot&, const ParseOptions& = {}) override;
};
}
