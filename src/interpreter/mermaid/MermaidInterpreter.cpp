#include "MermaidInterpreter.h"
#include "contracts/diagram/DiagramWire.h"
namespace xfmd {
DiagramParseResult MermaidInterpreter::parse(const DiagramSource& source) {
  try {
    diagramWire::ResultOwner output(
        xfmd_mermaid_parse_v1(1, reinterpret_cast<const std::uint8_t*>(source.text.data()),
                              source.text.size()),
        xfmd_mermaid_parse_free_v1);
    diagramWire::Reader reader(output.result);
    auto result = std::make_shared<DiagramModel>(reader.model());
    reader.finish();
    return {std::move(result), {}};
  } catch (const std::exception& error) {
    return {{}, error.what()};
  }
}
} // namespace xfmd
