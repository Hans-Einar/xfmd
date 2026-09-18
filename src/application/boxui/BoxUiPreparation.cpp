#include "BoxUiPreparation.h"
#include "application/adapters/SvgDiagramCache.h"
#include "contracts/diagram/DiagramLimits.h"
#include "renderer/boxui/BoxUiLayout.h"
#include <algorithm>
#include <chrono>
namespace xfmd {
ParseResult BoxUiPreparation::prepare(ParseResult original, ITextMetrics& metrics,
                                      IDiagramLayout& diagrams, const BoxUiState& state,
                                      const std::function<bool()>& cancelled, double width) {
  auto result = std::make_shared<SemanticDocument>(*original);
  std::size_t total = 0;
  for (auto& b : result->blocks)
    if (b.diagramScene)
      total += b.diagramScene->bytes;
  for (auto& b : result->blocks) {
    if (!b.boxUi)
      continue;
    auto start = std::chrono::steady_clock::now();
    auto checkpoint = [&] {
      if (cancelled && cancelled())
        throw Error(ErrorCode::Layout, "BoxUI cancelled");
      if (std::chrono::steady_clock::now() - start >
          std::chrono::milliseconds(diagramLayoutBudgetMilliseconds))
        throw Error(ErrorCode::Layout, "BoxUI preparation budget exceeded");
    };
    try {
      checkpoint();
      BoxUiLayoutRequest request;
      request.key = {result->token,         b.boxUi->documentId, state.epoch,
                     state.bindingRevision, state.stateRevision, state.stateRevision};
      auto it = state.blocks.find(b.boxUi->documentId);
      if (it != state.blocks.end())
        request.snapshot = it->second;
      request.width = std::clamp(width, 320., 8192.);
      request.height = std::max(480., request.width * .75);
      request.cancelled = cancelled;
      request.budgetMs = diagramLayoutBudgetMilliseconds;
      for (auto& c : b.boxUi->children) {
        checkpoint();
        BoxUiPreparedChild child;
        child.ref = c.ref;
        child.error = c.error;
        if (c.model)
          try {
            auto scene = diagrams.layout(*c.model, {cancelled}, metrics);
            child.svg = scene->svg;
            child.width = scene->width / .75;
            child.height = scene->height / .75;
          } catch (const std::exception& e) {
            child.error = e.what();
          }
        request.children.push_back(std::move(child));
      }
      checkpoint();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - start)
                         .count();
      request.budgetMs = std::max(1, int(diagramLayoutBudgetMilliseconds) - int(elapsed));
      BoxUiLayout layout;
      b.boxUiFrame = layout.prepare(*b.boxUi, request, metrics);
      checkpoint();
      SvgDiagramCache::validate(b.boxUiFrame->staticScene->svg);
      SvgDiagramCache::validate(b.boxUiFrame->previewScene->svg);
      if (b.boxUiFrame->bytes > 64 * 1024 * 1024 - total)
        throw Error(ErrorCode::TooLarge, "Combined visual budget exceeded");
      total += b.boxUiFrame->bytes;
    } catch (const std::exception& e) {
      if (cancelled && cancelled())
        throw;
      b.boxUiFrame.reset();
      b.kind = BlockKind::Code;
      b.runs = {{std::string("BoxUI: ") + e.what() + "\n" + b.diagramSource,
                 b.source,
                 false,
                 false,
                 true,
                 {}}};
    }
  }
  return result;
}
} // namespace xfmd
