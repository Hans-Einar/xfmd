#pragma once
#include "SourceMapBuilder.h"
#include "contracts/SemanticDocument.h"
#include <map>
namespace xfmd {
class ModelBuilder {
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
  explicit ModelBuilder(const SourceSnapshot&);
  void appendNode(cmark_node*, cmark_event_type);
  SemanticDocument finish();
};
} // namespace xfmd
