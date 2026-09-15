#pragma once
#include "contracts/SemanticDocument.h"
#include "contracts/diagram/IDiagramInterpreter.h"
namespace xfmd {
class MermaidBlockBuilder {
public:
  static bool build(SemanticBlock&, const char*, const char*, IDiagramInterpreter*, std::size_t&);
};
} // namespace xfmd
