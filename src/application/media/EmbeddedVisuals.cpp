#include "EmbeddedVisuals.h"
#include "ImageDecoder.h"
#include "MathTypesetter.h"
#include "application/navigation/LinkResolver.h"
#include <map>
namespace xfmd {
ParseResult EmbeddedVisuals::prepare(ParseResult source, const SourceSnapshot& snapshot) {
  auto model = std::make_shared<SemanticDocument>(*source);
  std::map<std::string, std::shared_ptr<const VisualResource>> cache;
  std::size_t count = 0;
  double pixels = 0;
  auto prepareRuns = [&](std::vector<InlineRun>& runs) {
    for (auto& run : runs) {
      auto& item = run.embedded;
      if (item.kind == EmbeddedKind::None)
        continue;
      try {
        if (++count > 256)
          throw Error(ErrorCode::TooLarge, "Document exceeds 256 embedded resources.");
        auto key = std::to_string(int(item.kind)) + (item.display ? "d:" : "i:") + item.source;
        auto found = cache.find(key);
        if (found != cache.end())
          item.visual = found->second;
        else {
          auto visual =
              item.kind == EmbeddedKind::Image
                  ? ImageDecoder::load(LinkResolver::resourcePath(snapshot.path, item.source))
                  : MathTypesetter::render(item.source, item.display);
          pixels += visual->width * visual->height;
          if (pixels > 16000000)
            throw Error(ErrorCode::TooLarge, "Document image budget exceeded.");
          item.visual = visual;
          cache.emplace(std::move(key), std::move(visual));
        }
      } catch (const std::exception& e) {
        run.text = "[" + (run.text.empty() ? item.source : run.text) + ": " + e.what() + "]";
        run.italic = true;
      }
    }
  };
  for (auto& block : model->blocks) {
    prepareRuns(block.runs);
    if (block.table) {
      auto table = std::make_shared<SemanticTable>(*block.table);
      for (auto& row : table->rows)
        for (auto& cell : row.cells)
          prepareRuns(cell.runs);
      block.table = std::move(table);
    }
  }
  return model;
}
} // namespace xfmd
