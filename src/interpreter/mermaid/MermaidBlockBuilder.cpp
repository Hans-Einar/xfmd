#include "MermaidBlockBuilder.h"
#include <sstream>
namespace xfmd {
bool MermaidBlockBuilder::build(SemanticBlock& block, const char* info, const char* literal,
                                IDiagramInterpreter* parser, std::size_t& count) {
  std::istringstream tokens(info ? info : "");
  std::string language;
  tokens >> language;
  if (language != "mermaid")
    return false;
  block.diagramSource = literal ? literal : "";
  block.runs.push_back({block.diagramSource, block.source, false, false, true, {}});
  DiagramParseResult result;
  if (++count > 16)
    result.error = "At most 16 diagrams per document";
  else if (!parser)
    result.error = "Diagram interpreter is unavailable";
  else
    result = parser->parse({block.diagramSource, block.source});
  block.diagram = result.model;
  if (result.model)
    block.kind = BlockKind::Diagram;
  else
    block.runs.insert(block.runs.begin(),
                      {"Mermaid: " + result.error + "\n", block.source, false, false, true, {}});
  return true;
}
} // namespace xfmd
