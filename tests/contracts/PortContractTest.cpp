#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include "support/TestSupport.h"
using namespace xfmd;
struct TextInterpreter final : IInterpreter {
  ParseResult parse(const SourceSnapshot& source, const ParseOptions&) override {
    auto result = std::make_shared<SemanticDocument>();
    result->token = source.token;
    result->sourceSize = source.text.size();
    SemanticBlock block;
    block.runs.push_back({source.text, {0, source.text.size()}, false, false, false, {}});
    result->blocks.push_back(block);
    return result;
  }
};
struct Metrics final : ITextMetrics {
  TextExtent measure(std::string_view text, FontSpec) override {
    return {double(text.size()) * 8, 16, 12};
  }
};
struct TextRenderer final : IRenderer {
  LayoutResult layout(const SemanticDocument& model, const LayoutRequest& request,
                      ITextMetrics& metrics) override {
    auto result = std::make_shared<RenderFrame>();
    result->token = model.token;
    result->generation = request.generation;
    result->width = request.width;
    result->height = metrics.measure(model.blocks.at(0).runs.at(0).text, {}).height;
    return result;
  }
  HitResult hitTest(const RenderFrame&, Point) const override { return {}; }
};
void run() {
  TextInterpreter parser;
  TextRenderer renderer;
  Metrics metrics;
  IInterpreter& interpretPort = parser;
  IRenderer& renderPort = renderer;
  auto model = interpretPort.parse({{7, 9}, "æøå", "", false});
  auto frame = renderPort.layout(*model, {400, 3}, metrics);
  CHECK(frame->token == DocumentToken{7, 9});
  CHECK(frame->generation == 3 && frame->width == 400 && frame->height == 16);
  CHECK(model->sourceSize == 6);
  CHECK(renderPort.hitTest(*frame, {0, 0}).link.empty());
  CHECK((Rect{0, 0, 10, 10}.contains({9, 9})));
  CHECK(!(Rect{0, 0, 10, 10}.contains({10, 10})));
}
TEST_MAIN(run)
