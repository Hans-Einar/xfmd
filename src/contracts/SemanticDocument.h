#pragma once
#include "DocumentTypes.h"
#include "VisualResource.h"
#include <memory>
#include <vector>

namespace xfmd {
struct BoxUiModel;
struct BoxUiFrame;
struct DiagramModel;
struct DiagramScene;
struct InlineRun {
  std::string text;
  SourceRange source;
  bool bold = false, italic = false, code = false;
  std::string link;
  std::size_t linkId = 0; // Shared by style fragments of one link; distinct for adjacent links.
  EmbeddedContent embedded{};
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
enum class BlockKind { Paragraph, Heading, Code, Rule, Html, Table, Diagram, BoxUi };
struct SemanticBlock {
  BlockKind kind = BlockKind::Paragraph;
  SourceRange source;
  int level = 0, indent = 0, quoteDepth = 0;
  std::string marker;
  std::vector<InlineRun> runs;
  std::shared_ptr<const SemanticTable> table;
  std::shared_ptr<const DiagramModel> diagram;
  std::shared_ptr<const DiagramScene> diagramScene;
  std::string diagramSource;
  std::shared_ptr<const BoxUiModel> boxUi;
  std::shared_ptr<const BoxUiFrame> boxUiFrame;
};
struct SemanticDocument {
  DocumentToken token;
  std::size_t sourceSize = 0;
  std::vector<SemanticBlock> blocks;
};
} // namespace xfmd
