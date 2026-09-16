#include "DiagramPreparation.h"
#include "contracts/diagram/DiagramWire.h"
namespace xfmd {
ParseResult DiagramPreparation::prepare(ParseResult original, ITextMetrics& metrics,
                                        const std::function<bool()>& cancelled) {
  auto checkpoint = [&] {
    if (cancelled && cancelled())
      throw Error(ErrorCode::Layout, "Diagram preparation cancelled");
  };
  auto result = std::make_shared<SemanticDocument>(*original);
  for (auto& block : result->blocks) {
    checkpoint();
    if (!block.diagram)
      continue;
    try {
      diagramWire::Writer value;
      value.model(*block.diagram);
      // Value-based identity: no hash collision, source range or palette in cached scene.
      std::string key = "flowchart1/layout1/font12/" + std::to_string(metrics.fontSetId()) + "/";
      key.append(reinterpret_cast<const char*>(value.data.data()), value.data.size());
      block.diagramScene = cache.find(key);
      if (!block.diagramScene) {
        block.diagramScene = layout.layout(*block.diagram, {cancelled}, metrics);
        checkpoint();
        cache.insert(std::move(key), block.diagramScene);
      }
    } catch (const std::exception& e) {
      checkpoint();
      block.kind = BlockKind::Code;
      block.diagramScene.reset();
      auto approximate = block.source;
      approximate.quality = MappingQuality::Approximate;
      block.runs.insert(
          block.runs.begin(),
          {std::string("Mermaid: ") + e.what() + "\n", approximate, false, false, true, {}});
    }
  }
  checkpoint();
  return result;
}
} // namespace xfmd
