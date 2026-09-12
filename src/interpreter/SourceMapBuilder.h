#pragma once
#include "contracts/SemanticDocument.h"
#include <cmark.h>
#include <vector>
namespace xfmd {
class SourceMapBuilder {
  const std::string& source;
  std::vector<std::size_t> lines;
  std::size_t offset(int line, int column, bool end) const;
public:
  explicit SourceMapBuilder(const std::string&);
  SourceRange record(cmark_node*) const;
  std::vector<InlineRun> codeLines(cmark_node*, const std::string&) const;
  bool matches(const std::string&, SourceRange) const;
};
}
