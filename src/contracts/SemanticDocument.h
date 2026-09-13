#pragma once
#include "DocumentTypes.h"
#include <memory>
#include <vector>

namespace xfmd {
struct InlineRun {
  std::string text;
  SourceRange source;
  bool bold = false, italic = false, code = false;
  std::string link;
  std::size_t linkId = 0; // Shared by style fragments of one link; distinct for adjacent links.
};
enum class ColumnAlignment { Left, Center, Right };
struct TableCell {
  SourceRange source;
  std::vector<InlineRun> runs;
};
struct TableRow {
  SourceRange source;
  bool header = false;
  std::vector<TableCell> cells;
};
struct SemanticTable {
  std::vector<ColumnAlignment> alignments;
  std::vector<TableRow> rows;
};
enum class BlockKind { Paragraph, Heading, Code, Rule, Html, Table };
struct SemanticBlock {
  BlockKind kind = BlockKind::Paragraph;
  SourceRange source;
  int level = 0, indent = 0, quoteDepth = 0;
  std::string marker;
  std::vector<InlineRun> runs;
  std::shared_ptr<const SemanticTable> table;
};
struct SemanticDocument {
  DocumentToken token;
  std::size_t sourceSize = 0;
  std::vector<SemanticBlock> blocks;
};
} // namespace xfmd
