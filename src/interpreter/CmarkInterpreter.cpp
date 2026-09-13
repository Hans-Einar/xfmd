#include "CmarkInterpreter.h"
#include "ModelBuilder.h"
#include <cmark-gfm-core-extensions.h>
#include <cmark-gfm-extension_api.h>
#include <cmark-gfm.h>
#include <memory>
namespace xfmd {
ParseResult CmarkInterpreter::parse(const SourceSnapshot& source, const ParseOptions& options) {
  if (source.text.size() > options.maxBytes)
    throw Error(ErrorCode::TooLarge, "Input exceeds parser size limit.");
  if (source.plainText) {
    auto result = std::make_shared<SemanticDocument>();
    result->token = source.token;
    result->sourceSize = source.text.size();
    auto begin = source.text.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3u : 0u;
    SemanticBlock block;
    block.kind = BlockKind::Code;
    block.source = {begin, source.text.size()};
    block.runs.push_back({source.text.substr(begin), block.source, false, false, true, {}});
    result->blocks.push_back(std::move(block));
    return result;
  }
  auto start = source.text.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3u : 0u;
  // C++ static initialization serializes the library's process-global registry.
  static auto* tables = [] {
    cmark_gfm_core_extensions_ensure_registered();
    return cmark_find_syntax_extension("table");
  }();
  std::unique_ptr<cmark_parser, decltype(&cmark_parser_free)> parser(
      cmark_parser_new(CMARK_OPT_DEFAULT), cmark_parser_free);
  if (!parser || !tables || !cmark_parser_attach_syntax_extension(parser.get(), tables))
    throw Error(ErrorCode::Parse, "Unable to initialize Markdown table parser.");
  cmark_parser_feed(parser.get(), source.text.data() + start, source.text.size() - start);
  std::unique_ptr<cmark_node, decltype(&cmark_node_free)> root(cmark_parser_finish(parser.get()),
                                                               cmark_node_free);
  if (!root)
    throw Error(ErrorCode::Parse, "Unable to parse Markdown.");
  std::unique_ptr<cmark_iter, decltype(&cmark_iter_free)> it(cmark_iter_new(root.get()),
                                                             cmark_iter_free);
  if (!it)
    throw Error(ErrorCode::Parse, "Unable to traverse Markdown.");
  ModelBuilder builder(source);
  cmark_event_type event;
  while ((event = cmark_iter_next(it.get())) != CMARK_EVENT_DONE)
    builder.appendNode(cmark_iter_get_node(it.get()), event);
  return std::make_shared<const SemanticDocument>(builder.finish());
}
} // namespace xfmd
