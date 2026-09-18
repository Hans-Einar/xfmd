#include "BoxUiBlockBuilder.h"
#include <sstream>
namespace xfmd {
bool BoxUiBlockBuilder::build(SemanticBlock& block, const char* info, const char* literal,
                              IBoxUiInterpreter* parser, std::size_t& count) {
  std::string source = literal ? literal : "", language = info ? info : "";
  std::istringstream words(source);
  std::string header;
  words >> header;
  if (language != "boxui" && !(language == "mermaid" && header == "boxui"))
    return false;
  block.kind = BlockKind::BoxUi;
  block.diagramSource = source;
  try {
    if (++count > 64)
      throw Error(ErrorCode::TooLarge, "Combined diagram count exceeds 64");
    if (!parser)
      throw Error(ErrorCode::Unsupported, "BoxUI interpreter unavailable");
    block.boxUi = parser->parse(source);
  } catch (const std::exception& e) {
    block.kind = BlockKind::Code;
    block.runs.push_back(
        {std::string("BoxUI: ") + e.what() + "\n" + source, block.source, false, false, true, {}});
  }
  return true;
}
} // namespace xfmd
