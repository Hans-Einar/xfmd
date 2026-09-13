#include "application/adapters/SharedTextMetrics.h"
#include "application/scroll/AnchorMapper.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <cmath>
#include <fstream>
#include <iterator>
using namespace xfmd;
void run() {
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  SharedTextMetrics metrics;
  std::ifstream file(XFMD_TABLE_FIXTURE);
  std::string source{std::istreambuf_iterator<char>(file), {}};
  auto model = parser.parse({{4, 2}, source, {}, false});
  const SemanticTable* table = nullptr;
  for (const auto& block : model->blocks)
    if (block.table)
      table = block.table.get();
  CHECK(table && table->rows.size() == 6 && table->alignments.size() == 3);
  CHECK(table->rows.front().header);
  CHECK(table->alignments[1] == ColumnAlignment::Center &&
        table->alignments[2] == ColumnAlignment::Right);
  bool code = false, bold = false, unicode = false, local = false;
  for (const auto& row : table->rows) {
    CHECK(row.cells.size() == 3);
    for (const auto& cell : row.cells) {
      CHECK(cell.source.begin <= cell.source.end && cell.source.end <= source.size());
      for (const auto& run : cell.runs) {
        CHECK(run.source.begin <= run.source.end && run.source.end <= source.size());
        if (run.source.quality == MappingQuality::Exact)
          CHECK(source.substr(run.source.begin, run.source.end - run.source.begin) == run.text);
        code |= run.code && run.text == "a|b";
        bold |= run.bold && run.text == "Tabeller";
        unicode |= run.text.find("æøå") != std::string::npos;
        local |= run.link == "basics.md";
      }
    }
  }
  CHECK(code && bold && unicode && local);
  auto wide = renderer.layout(*model, {750, 1}, metrics);
  auto narrow = renderer.layout(*model, {250, 2}, metrics);
  CHECK(narrow->height > wide->height && !narrow->decorations.empty());
  for (std::size_t i = 1; i < narrow->runs.size(); ++i)
    CHECK(narrow->runs[i - 1].bounds.y <= narrow->runs[i].bounds.y);
  bool hit = false, marker = false;
  for (const auto& run : narrow->runs) {
    if (run.link == "basics.md") {
      CHECK(renderer.hitTest(*narrow, {run.bounds.x + 1, run.bounds.y + 1}).link == run.link);
      hit = true;
    }
    if (run.icon == InlineIcon::ExternalLink) {
      CHECK(run.text == "↗ " && run.shaped && run.shaped->width == run.bounds.width);
      for (const auto& part : run.shaped->segments)
        for (const auto& glyph : part.glyphs)
          CHECK(!(glyph.index & 0x10000000));
      marker = true;
    }
  }
  CHECK(hit && marker);
  // Exact equal-width columns allow independent alignment checks.
  auto aligned = parser.parse({{1, 1}, "|x|x|x|\n|:---|:---:|---:|\n|x|x|x|\n", {}, false});
  auto a = renderer.layout(*aligned, {348, 1}, metrics);
  std::vector<const DrawRun*> xs;
  for (const auto& run : a->runs)
    if (run.text == "x")
      xs.push_back(&run);
  CHECK(xs.size() == 6);
  CHECK(std::abs(xs[0]->bounds.x - 29) < .01);
  CHECK(std::abs(xs[1]->bounds.x + xs[1]->bounds.width / 2 - 174) < .01);
  CHECK(std::abs(xs[2]->bounds.x + xs[2]->bounds.width - 319) < .01);
  std::string many = "| Name | Details |\n|---|---|\n";
  for (int i = 0; i < 100; ++i)
    many += "| Row" + std::to_string(i) + " | **Text** with Unicode æøå and wrapping content. |\n";
  auto m = parser.parse({{2, 1}, many, {}, false});
  auto pages = renderer.layout(*m, {600, 1, {LayoutMode::Paged, {}}}, metrics);
  CHECK(pages->pages.slices.size() >= 3);
  for (const auto& run : pages->runs) {
    auto y = std::fmod(run.bounds.y, pages->pages.paper.height);
    CHECK(y >= pages->pages.paper.margin - .01);
    CHECK(y + run.bounds.height <= pages->pages.paper.height - pages->pages.paper.margin + .01);
  }
  // Each row maps to one page, including wrapped text, with no duplicated/lost text.
  for (const auto& row : m->blocks.front().table->rows) {
    auto anchor = AnchorMapper::map({row.source.begin, 0, MappingQuality::Exact}, *pages);
    CHECK(anchor.quality != MappingQuality::Unavailable);
  }
  auto uneven = parser.parse({{1, 1}, "a|b\n---|---\nx\ny|z|ignored\n", {}, false});
  CHECK(uneven->blocks[0].table && uneven->blocks[0].table->rows.size() == 3);
  CHECK(uneven->blocks[0].table->rows[1].cells.size() == 2);
  auto plain = parser.parse({{1, 1}, many, {}, true});
  CHECK(!plain->blocks[0].table);
  auto other = parser.parse({{1, 1}, "~~strike~~ https://example.org\n\n- [x] task", {}, false});
  CHECK(other->blocks[0].runs[0].text == "~~strike~~ https://example.org");
  bool limited = false;
  std::string huge = "a|b\n---|---\n" + std::string(20000, 'x') + "|y\n";
  try {
    renderer.layout(*parser.parse({{1, 1}, huge, {}, false}), {600, 1, {LayoutMode::Paged, {}}},
                    metrics);
  } catch (const Error& e) {
    limited = e.code == ErrorCode::Layout;
  }
  CHECK(limited);
  std::string columns, separator;
  for (int i = 0; i < 65; ++i) {
    columns += "x|";
    separator += "---|";
  }
  limited = false;
  try {
    parser.parse({{1, 1}, columns + "\n" + separator + "\n", {}, false});
  } catch (const Error& e) {
    limited = e.code == ErrorCode::TooLarge;
  }
  CHECK(limited);
  auto quoted = parser.parse({{1, 1}, "> a|b\n> ---|---\n> x|y\n", {}, false});
  CHECK(quoted->blocks[0].table && quoted->blocks[0].quoteDepth == 1);
  auto incomplete = parser.parse({{1, 1}, "a|b\n---|unfinished\n", {}, false});
  CHECK(!incomplete->blocks[0].table);
}
TEST_MAIN(run)
