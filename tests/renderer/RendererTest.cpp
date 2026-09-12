#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
using namespace xfmd;
struct FixedMetrics : ITextMetrics {
  TextExtent measure(std::string_view text, FontSpec font) override {
    int characters = 0;
    for (unsigned char c : text) if ((c & 0xc0) != 0x80) ++characters;
    return {characters * font.points / 2, font.points + 4, font.points};
  }
};
void run() {
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  FixedMetrics metrics;
  auto model = parser.parse({{9, 3}, "# Heading\n\nA **bold** and *italic* [link](next.md) and `code`.\n\n> quote\n\n- one\n- two\n\n```\nlong code line\n```\n", {}, false});
  auto wide = renderer.layout(*model, {600, 1}, metrics);
  auto narrow = renderer.layout(*model, {140, 2}, metrics);
  CHECK(wide->token == model->token && narrow->generation == 2);
  CHECK(narrow->height >= wide->height);
  bool heading = false, bold = false, italic = false, mono = false, link = false;
  for (const auto& run : narrow->runs) {
    CHECK(run.bounds.width >= 0 && run.bounds.height > 0);
    heading |= run.font.points > 12;
    bold |= run.text == "bold" && run.font.bold;
    italic |= run.text == "italic" && run.font.italic;
    mono |= run.font.mono;
    if (run.text == "link") {
      CHECK(renderer.hitTest(*narrow, {run.bounds.x + 1, run.bounds.y + 1}).link == "next.md"); link = true;
    }
  }
  CHECK(heading && bold && italic && mono && link);
  CHECK(!narrow->anchors.empty() && !narrow->decorations.empty());
  CHECK(renderer.hitTest(*narrow, {-5, -5}).link.empty());
  auto unicode = parser.parse({{1, 1}, "æøåæøåæøåæøåæøåæøåæøåæøå", {}, false});
  auto wrapped = renderer.layout(*unicode, {80, 3}, metrics);
  std::string combined;
  for (const auto& run : wrapped->runs) combined += run.text;
  CHECK(combined == "æøåæøåæøåæøåæøåæøåæøåæøå");
}
TEST_MAIN(run)
