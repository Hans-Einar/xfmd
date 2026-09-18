#include "DiagramPreparation.h"
#include "application/adapters/SvgDiagramCache.h"
#include "contracts/diagram/DiagramWire.h"
#include <cstdlib>
namespace xfmd {
ParseResult DiagramPreparation::prepare(ParseResult original, ITextMetrics& metrics,
                                        const std::function<bool()>& cancelled) {
  auto checkpoint = [&] {
    if (cancelled && cancelled())
      throw Error(ErrorCode::Layout, "Diagram preparation cancelled");
  };
  auto result = std::make_shared<SemanticDocument>(*original);
  std::size_t sceneBytes = 0;
  for (auto& block : result->blocks) {
    checkpoint();
    if (!block.diagram)
      continue;
    try {
      diagramWire::Writer value;
      value.model(*block.diagram);
      // Value-based identity: no hash collision, source range or palette in cached scene.
      std::string key = "model3/semantic1-sequence2/svg3/leaders1/wrap120-v1/font12/" +
                        std::to_string(metrics.fontSetId()) + "/";
      for (const char* name : {"XFMD_MERMAID_ROUTER", "XFMD_MERMAID_CROSSING_JUMPS"}) {
        const char* setting = std::getenv(name);
        key += setting ? setting : "<default>";
        key += '/';
      }
      key.append(reinterpret_cast<const char*>(value.data.data()), value.data.size());
      block.diagramScene = cache.find(key);
      if (!block.diagramScene) {
        block.diagramScene = layout.layout(*block.diagram, {cancelled}, metrics);
        checkpoint();
        SvgDiagramCache::validate(block.diagramScene->svg);
        cache.insert(std::move(key), block.diagramScene);
      }
      if (block.diagramScene->bytes > 64 * 1024 * 1024 - sceneBytes)
        throw Error(ErrorCode::TooLarge, "Document diagram scenes exceed 64 MiB");
      sceneBytes += block.diagramScene->bytes;
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
