#include "application/scroll/ScrollCoordinator.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
using namespace xfmd;
struct Metrics : ITextMetrics {
  TextExtent measure(std::string_view text, FontSpec font) override { return {int(text.size()) * 7, font.points + 4, font.points}; }
};
void run() {
  CmarkInterpreter parser; MarkdownRenderer renderer; Metrics metrics;
  std::string source = "# Heading\n\nA paragraph with repeated repeated words and many words that wrap.\n\n```\nfirst\nrepeat\nrepeat\nlast\n```\n";
  auto model = parser.parse({{1, 2}, source, {}, false});
  auto frame = renderer.layout(*model, {160, 1}, metrics);
  auto first = source.find("repeat\nrepeat"), second = first + 7;
  auto one = AnchorMapper::map({first}, *frame), two = AnchorMapper::map({second}, *frame);
  CHECK(one.quality != MappingQuality::Unavailable && two.y > one.y);
  CHECK(AnchorMapper::anchorAt(one.y, *frame).byte == first);
  CHECK(AnchorMapper::anchorAt(two.y, *frame).byte == second);
  ScrollCoordinator coordinator;
  int previewY = -1, moves = 0; std::size_t editorByte = 0;
  coordinator.setPreview = [&](int y) { previewY = y; ++moves; coordinator.onViewportChanged(ViewOrigin::Preview, y, {1, 2}); };
  coordinator.setEditor = [&](SourceAnchor anchor) { editorByte = anchor.byte; ++moves; coordinator.onViewportChanged(ViewOrigin::Editor, anchor.byte, {1, 2}); };
  coordinator.setSplit(true); coordinator.invalidate({1, 2}); coordinator.setFrame(frame);
  moves = 0;
  coordinator.onViewportChanged(ViewOrigin::Editor, second, {1, 2}, 1);
  CHECK(previewY == two.y && moves == 1);
  coordinator.onViewportChanged(ViewOrigin::Preview, one.y, {1, 2}, 2);
  CHECK(editorByte == first && moves == 2);
  coordinator.onViewportChanged(ViewOrigin::Editor, second, {1, 2}, 1); CHECK(moves == 2);
  coordinator.onViewportChanged(ViewOrigin::Editor, second, {1, 2}, 3, true); CHECK(moves == 2);
  coordinator.invalidate({1, 3});
  coordinator.onViewportChanged(ViewOrigin::Editor, second, {1, 2}, 4); CHECK(moves == 2);
  coordinator.setFrame(frame); CHECK(moves == 2);
  auto empty = renderer.layout(*parser.parse({{9, 0}, "", {}, false}), {160, 1}, metrics);
  CHECK(AnchorMapper::map({}, *empty).quality == MappingQuality::Unavailable);
  auto wider = renderer.layout(*model, {400, 2}, metrics);
  CHECK(AnchorMapper::map({second}, *wider).y <= two.y);
}
TEST_MAIN(run)
