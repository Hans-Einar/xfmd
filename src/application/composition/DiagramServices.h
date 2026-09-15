#pragma once
#include "application/adapters/SharedTextMetrics.h"
#include "contracts/IInterpreter.h"
#include <functional>
namespace xfmd {
using PrepareDocument =
    std::function<ParseResult(ParseResult, const SourceSnapshot&, const std::function<bool()>&)>;
class DiagramServices {
public:
  static std::unique_ptr<IInterpreter> interpreter();
  static PrepareDocument preview();
  static ParseResult prepare(ParseResult, const SourceSnapshot&, SharedTextMetrics&,
                             const std::function<bool()>&);
};
} // namespace xfmd
