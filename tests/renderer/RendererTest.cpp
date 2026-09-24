#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
using namespace xfmd;
struct FixedMetrics : ITextMetrics {
  TextExtent measure(std::string_view text, FontSpec font) override {
    int characters = 0;
    for (unsigned char c : text)
      if ((c & 0xc0) != 0x80)
        ++characters;
    return {characters * font.points / 2.0, font.points + 4.0, double(font.points)};
  }
};
void run() {
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  FixedMetrics metrics;
  auto model = parser.parse({{9, 3},
                             "# Heading\n\nA **bold** and *italic* [link](next.md) and "
                             "`code`.\n\n> quote\n\n- one\n- two\n\n```\nlong code line\n```\n",
                             {},
                             false});
  auto small =
      parser.parse({{12, 1}, "## Narrow heading\n\nOrdinary wrapping words.\n", {}, false});
  auto zoomed = renderer.layout(*small, {75, 1}, metrics);
  CHECK(zoomed->contentWidth <= 75);
  auto smaller = renderer.layout(*small, {50, 2}, metrics);
  CHECK(smaller->contentWidth <= 50);
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
      CHECK(renderer.hitTest(*narrow, {run.bounds.x + 1, run.bounds.y + 1}).link == "next.md");
      link = true;
    }
  }
  CHECK(heading && bold && italic && mono && link);
  CHECK(!narrow->anchors.empty() && !narrow->decorations.empty());
  CHECK(renderer.hitTest(*narrow, {-5, -5}).link.empty());
  auto links = parser.parse({{2, 1},
                             "[relative **bold**](../doc.md)[again](../doc.md) "
                             "[absolute](/tmp/doc.MD) [web](https://example.org/doc.md)\n",
                             {},
                             false});
  auto marked = renderer.layout(*links, {900, 4}, metrics);
  int relativeMarkers = 0, absoluteMarkers = 0, globes = 0;
  for (const auto& run : marked->runs) {
    bool marker = run.text == "# " || run.text == "/# " || run.icon == InlineIcon::ExternalLink;
    relativeMarkers += run.text == "# ";
    absoluteMarkers += run.text == "/# ";
    globes += run.icon == InlineIcon::ExternalLink;
    if (marker) {
      CHECK(run.source.quality == MappingQuality::Approximate);
      CHECK(run.source.begin == run.source.end);
      CHECK(renderer.hitTest(*marked, {run.bounds.x + 1, run.bounds.y + 1}).link == run.link);
    }
  }
  CHECK(relativeMarkers == 2 && absoluteMarkers == 1 && globes == 1);
  auto unicode = parser.parse({{1, 1}, "æøåæøåæøåæøåæøåæøåæøåæøå", {}, false});
  auto wrapped = renderer.layout(*unicode, {80, 3}, metrics);
  std::string combined;
  for (const auto& run : wrapped->runs)
    combined += run.text;
  CHECK(combined == "æøåæøåæøåæøåæøåæøåæøåæøå");
}
TEST_MAIN(run)
