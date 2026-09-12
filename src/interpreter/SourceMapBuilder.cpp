#include "SourceMapBuilder.h"
#include <algorithm>
namespace xfmd {
SourceMapBuilder::SourceMapBuilder(const std::string& source) : source(source) {
  std::size_t start = source.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3 : 0;
  lines.push_back(start);
  for (std::size_t i = start; i < source.size(); ++i) {
    if (source[i] == '\r') { if (i + 1 < source.size() && source[i + 1] == '\n') ++i; lines.push_back(i + 1); }
    else if (source[i] == '\n') lines.push_back(i + 1);
  }
}
std::size_t SourceMapBuilder::offset(int line, int column, bool end) const {
  if (line < 1 || std::size_t(line) > lines.size()) return source.size();
  auto start = lines[std::size_t(line - 1)];
  auto limit = std::size_t(line) < lines.size() ? lines[std::size_t(line)] : source.size();
  return std::min(limit, start + std::size_t(std::max(0, column - (end ? 0 : 1))));
}
SourceRange SourceMapBuilder::record(cmark_node* node) const {
  if (!cmark_node_get_start_line(node)) return {0, 0, MappingQuality::Unavailable};
  auto begin = offset(cmark_node_get_start_line(node), cmark_node_get_start_column(node), false);
  auto end = offset(cmark_node_get_end_line(node), cmark_node_get_end_column(node), true);
  // Tabs affect parser indentation columns; preserve block location but avoid claiming exact intra-run positions.
  auto quality = source.find('\t', begin) < end ? MappingQuality::Approximate : MappingQuality::Exact;
  return {begin, std::max(begin, end), quality};
}
bool SourceMapBuilder::matches(const std::string& text, SourceRange range) const {
  return range.end >= range.begin && range.end <= source.size() &&
         source.compare(range.begin, range.end - range.begin, text) == 0;
}
}
