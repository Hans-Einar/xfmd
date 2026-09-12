#pragma once
#include "DocumentTypes.h"
#include <vector>

namespace xfmd {
struct InlineRun {
  std::string text;
  SourceRange source;
  bool bold = false, italic = false, code = false;
  std::string link;
};
enum class BlockKind { Paragraph, Heading, Code, Rule, Html };
struct SemanticBlock {
  BlockKind kind = BlockKind::Paragraph;
  SourceRange source;
  int level = 0, indent = 0, quoteDepth = 0;
  std::string marker;
  std::vector<InlineRun> runs;
};
struct SemanticDocument {
  DocumentToken token;
  std::size_t sourceSize = 0;
  std::vector<SemanticBlock> blocks;
};
} // namespace xfmd
