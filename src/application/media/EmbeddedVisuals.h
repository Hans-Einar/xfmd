#pragma once
#include "contracts/IInterpreter.h"
namespace xfmd {
class EmbeddedVisuals {
public:
  static ParseResult prepare(ParseResult, const SourceSnapshot&);
};
} // namespace xfmd
