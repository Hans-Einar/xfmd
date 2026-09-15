#pragma once
#include "contracts/DocumentTypes.h"
#include <cmark-gfm.h>
#include <vector>
namespace xfmd {
struct MathToken {
  SourceRange range;
  std::string formula;
  bool display;
};
class MathSyntax {
public:
  std::string masked;
  std::vector<MathToken> tokens;
  MathSyntax(const std::string&, cmark_node*);
  const MathToken* at(SourceRange) const;
};
} // namespace xfmd
