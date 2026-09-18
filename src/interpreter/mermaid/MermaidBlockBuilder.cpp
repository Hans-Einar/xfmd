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
  auto approximate = block.source;
  approximate.quality = MappingQuality::Approximate;
  block.diagramSource = literal ? literal : "";
  block.runs.push_back({block.diagramSource, approximate, false, false, true, {}});
  DiagramParseResult result;
  if (++count > 64)
    result.error = "At most 64 diagrams per document";
  else if (!parser)
    result.error = "Diagram interpreter is unavailable";
  else
    result = parser->parse({block.diagramSource, approximate});
  block.diagram = result.model;
  if (result.model)
    block.kind = BlockKind::Diagram;
  else
    block.runs.insert(block.runs.begin(),
                      {"Mermaid: " + result.error + "\n", approximate, false, false, true, {}});
  return true;
}
} // namespace xfmd
