#pragma once
#include "MathSyntax.h"
#include "contracts/boxui/IBoxUiInterpreter.h"
#include "SourceMapBuilder.h"
#include "contracts/SemanticDocument.h"
#include "contracts/diagram/IDiagramInterpreter.h"
#include <map>
namespace xfmd {
class ModelBuilder {
  const MathSyntax* math;
  IDiagramInterpreter* diagrams;
  IBoxUiInterpreter* boxUi;
  std::size_t diagramCount = 0;
  SemanticDocument model;
  SourceMapBuilder mapping;
  SemanticBlock* active = nullptr;
  std::map<cmark_node*, int> listCounts;
  void beginBlock(cmark_node*);
  bool appendTable(cmark_node*, cmark_event_type);
  std::shared_ptr<SemanticTable> table;
  SemanticBlock cell;
  std::size_t cells = 0;

public:
  explicit ModelBuilder(const SourceSnapshot&, const MathSyntax* = nullptr,
                        IDiagramInterpreter* = nullptr, IBoxUiInterpreter* = nullptr);
  void appendNode(cmark_node*, cmark_event_type);
  SemanticDocument finish();
};
} // namespace xfmd
