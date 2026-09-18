#pragma once
#include "contracts/SemanticDocument.h"
#include "contracts/boxui/IBoxUiInterpreter.h"
namespace xfmd {
struct BoxUiBlockBuilder {
  static bool build(SemanticBlock&, const char*, const char*, IBoxUiInterpreter*, std::size_t&);
};
} // namespace xfmd
