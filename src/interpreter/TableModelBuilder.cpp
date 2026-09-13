#include "ModelBuilder.h"
#include <cmark-gfm-core-extensions.h>
#include <cmark-gfm-extension_api.h>
#include <string_view>
namespace xfmd {
bool ModelBuilder::appendTable(cmark_node* node, cmark_event_type event) {
  if (!cmark_node_get_syntax_extension(node))
    return false;
  std::string_view kind = cmark_node_get_type_string(node);
  if (kind == "table") {
    if (event == CMARK_EVENT_ENTER) {
      beginBlock(node);
      active->kind = BlockKind::Table;
      table = std::make_shared<SemanticTable>();
      const auto columns = cmark_gfm_extensions_get_table_columns(node);
      if (!columns || columns > 64)
        throw Error(ErrorCode::TooLarge, "Tables support between 1 and 64 columns.");
      auto* alignments = cmark_gfm_extensions_get_table_alignments(node);
      for (unsigned i = 0; i < columns; ++i)
        table->alignments.push_back(alignments[i] == 'c'   ? ColumnAlignment::Center
                                    : alignments[i] == 'r' ? ColumnAlignment::Right
                                                           : ColumnAlignment::Left);
      active->table = table;
      active = nullptr;
    } else {
      table.reset();
      active = nullptr;
    }
  } else if (kind == "table_header" || kind == "table_row") {
    if (event == CMARK_EVENT_ENTER) {
      if (!table)
        throw Error(ErrorCode::Parse, "Table row without table.");
      table->rows.push_back(
          {mapping.record(node), bool(cmark_gfm_extensions_get_table_row_is_header(node)), {}});
    }
    active = nullptr;
  } else if (kind == "table_cell") {
    if (event == CMARK_EVENT_ENTER) {
      if (!table || table->rows.empty())
        throw Error(ErrorCode::Parse, "Cell without table row.");
      if (++cells > 50000)
        throw Error(ErrorCode::TooLarge, "Document exceeds 50000 table cells.");
      cell = SemanticBlock{};
      cell.source = mapping.record(node);
      if (cell.source.quality == MappingQuality::Unavailable) {
        cell.source = table->rows.back().source;
        cell.source.quality = MappingQuality::Approximate;
      }
      active = &cell;
    } else {
      table->rows.back().cells.push_back({cell.source, std::move(cell.runs)});
      active = nullptr;
    }
  } else {
    return false;
  }
  return true;
}
} // namespace xfmd
