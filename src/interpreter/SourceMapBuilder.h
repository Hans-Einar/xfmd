#pragma once
#include "contracts/DocumentTypes.h"
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
  bool matches(const std::string&, SourceRange) const;
};
}
