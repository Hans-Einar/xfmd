#pragma once
#include "contracts/SemanticDocument.h"
#include "SourceMapBuilder.h"
#include <map>
namespace xfmd {
class ModelBuilder {
  SemanticDocument model;
  SourceMapBuilder mapping;
  SemanticBlock* active = nullptr;
  std::map<cmark_node*, int> listCounts;
  void beginBlock(cmark_node*);
public:
  explicit ModelBuilder(const SourceSnapshot&);
  void appendNode(cmark_node*, cmark_event_type);
  SemanticDocument finish();
};
}
