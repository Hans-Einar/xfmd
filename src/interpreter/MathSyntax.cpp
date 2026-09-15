#include "MathSyntax.h"
#include "SourceMapBuilder.h"
#include <algorithm>
#include <cctype>
#include <memory>
namespace xfmd {
MathSyntax::MathSyntax(const std::string& source, cmark_node* root) : masked(source) {
  std::vector<bool> protectedByte(source.size(), false);
  SourceMapBuilder mapping(source);
  std::unique_ptr<cmark_iter, decltype(&cmark_iter_free)> it(cmark_iter_new(root), cmark_iter_free);
  while (cmark_iter_next(it.get()) != CMARK_EVENT_DONE) {
    auto* node = cmark_iter_get_node(it.get());
    auto kind = cmark_node_get_type(node);
    if (kind == CMARK_NODE_CODE || kind == CMARK_NODE_CODE_BLOCK || kind == CMARK_NODE_HTML_BLOCK ||
        kind == CMARK_NODE_HTML_INLINE || kind == CMARK_NODE_IMAGE || kind == CMARK_NODE_LINK) {
      auto range = mapping.record(node);
      if (kind == CMARK_NODE_LINK && cmark_node_last_child(node))
        range.begin = mapping.record(cmark_node_last_child(node)).end;
      for (auto i = range.begin; i < std::min(range.end, source.size()); ++i)
        protectedByte[i] = true;
    }
  }
  auto escaped = [&](std::size_t at) {
    std::size_t n = 0;
    while (at && source[--at] == '\\')
      ++n;
    return n % 2 != 0;
  };
  for (std::size_t i = 0; i < source.size();) {
    if (protectedByte[i] || escaped(i)) {
      ++i;
      continue;
    }
    std::string close;
    std::size_t delimiter = 0;
    bool display = false;
    if (source.compare(i, 2, "$$") == 0) {
      close = "$$";
      delimiter = 2;
      display = true;
    } else if (source[i] == '$') {
      close = "$";
      delimiter = 1;
    } else if (source.compare(i, 2, "\\(") == 0) {
      close = "\\)";
      delimiter = 2;
    } else if (source.compare(i, 2, "\\[") == 0) {
      close = "\\]";
      delimiter = 2;
      display = true;
    }
    if (!delimiter || i + delimiter >= source.size() ||
        (!display && std::isspace(static_cast<unsigned char>(source[i + delimiter])))) {
      ++i;
      continue;
    }
    auto end = i + delimiter;
    for (; end < source.size() && end - i <= 16384; ++end) {
      if (!display && (source[end] == '\n' || source[end] == '\r'))
        break;
      if (!escaped(end) && source.compare(end, close.size(), close) == 0)
        break;
    }
    if (end >= source.size() || end - i > 16384 || source.compare(end, close.size(), close) != 0 ||
        end == i + delimiter ||
        (!display && std::isspace(static_cast<unsigned char>(source[end - 1]))) ||
        (delimiter == 1 && end + 1 < source.size() &&
         std::isdigit(static_cast<unsigned char>(source[end + 1])))) {
      ++i;
      continue;
    }
    auto stop = end + close.size();
    tokens.push_back({{i, stop}, source.substr(i + delimiter, end - i - delimiter), display});
    for (auto j = i; j < stop; ++j)
      masked[j] = 'Q';
    masked[i] = masked[stop - 1] = '`';
    i = stop;
  }
}
const MathToken* MathSyntax::at(SourceRange range) const {
  auto it = std::upper_bound(
      tokens.begin(), tokens.end(), range.begin,
      [](std::size_t value, const MathToken& token) { return value < token.range.begin; });
  if (it == tokens.begin())
    return nullptr;
  --it;
  return range.begin >= it->range.begin && range.end <= it->range.end ? &*it : nullptr;
}
} // namespace xfmd
