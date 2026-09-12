#include "CmarkInterpreter.h"
#include "ModelBuilder.h"
#include <cmark.h>
#include <memory>
namespace xfmd {
ParseResult CmarkInterpreter::parse(const SourceSnapshot& source, const ParseOptions& options) {
  if (source.text.size() > options.maxBytes) throw Error(ErrorCode::TooLarge, "Input exceeds parser size limit.");
  if (source.plainText) {
    auto result = std::make_shared<SemanticDocument>();
    result->token = source.token; result->sourceSize = source.text.size();
    auto begin = source.text.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3u : 0u;
    SemanticBlock block; block.kind = BlockKind::Code; block.source = {begin, source.text.size()};
    block.runs.push_back({source.text.substr(begin), block.source, false, false, true, {}});
    result->blocks.push_back(std::move(block));
    return result;
  }
  auto start = source.text.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3u : 0u;
  std::unique_ptr<cmark_node, decltype(&cmark_node_free)> root(
      cmark_parse_document(source.text.data() + start, source.text.size() - start, CMARK_OPT_DEFAULT), cmark_node_free);
  if (!root) throw Error(ErrorCode::Parse, "Unable to parse Markdown.");
  std::unique_ptr<cmark_iter, decltype(&cmark_iter_free)> it(cmark_iter_new(root.get()), cmark_iter_free);
  if (!it) throw Error(ErrorCode::Parse, "Unable to traverse Markdown.");
  ModelBuilder builder(source);
  cmark_event_type event;
  while ((event = cmark_iter_next(it.get())) != CMARK_EVENT_DONE) builder.appendNode(cmark_iter_get_node(it.get()), event);
  return std::make_shared<const SemanticDocument>(builder.finish());
}
}
