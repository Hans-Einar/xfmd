#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  CmarkInterpreter interpreter;
  SourceSnapshot source{{2, 5},
                        "\xef\xbb\xbf# æøå\r\n\r\nrepeat &amp; **repeat**\n\n> - item\n>   - "
                        "nested\n\n```\nrepeat\n```\n\n![alt](https://invalid/x)\n\n<div>x</div>\n",
                        "a.md",
                        false};
  auto model = interpreter.parse(source);
  CHECK(model->token == source.token);
  CHECK(model->sourceSize == source.text.size());
  CHECK(model->blocks.size() >= 7);
  CHECK(model->blocks[0].kind == BlockKind::Heading);
  CHECK(model->blocks[0].runs[0].text == "æøå");
  CHECK(model->blocks[0].runs[0].source.begin == 5);
  bool entity = false, bold = false, nested = false, code = false, html = false, image = false;
  for (const auto& block : model->blocks) {
    CHECK(block.source.begin <= block.source.end && block.source.end <= source.text.size());
    nested |= block.indent == 2 && block.quoteDepth == 1;
    code |= block.kind == BlockKind::Code;
    html |= block.kind == BlockKind::Html;
    for (const auto& run : block.runs) {
      CHECK(run.source.begin <= run.source.end && run.source.end <= source.text.size());
      entity |= run.text.find("repeat & ") != std::string::npos;
      bold |= run.text == "repeat" && run.bold && run.source.begin > 20;
      image |= run.text == "[image: ";
    }
  }
  CHECK(entity && bold && nested && code && html && image);
  source.plainText = true;
  auto plain = interpreter.parse(source);
  CHECK(plain->blocks.size() == 1 && plain->blocks[0].runs[0].text == source.text.substr(3));
  CHECK(interpreter.parse({{1, 0}, "", {}, false})->blocks.empty());
  auto incomplete = interpreter.parse({{1, 0}, "**unfinished [link", {}, false});
  CHECK(!incomplete->blocks.empty());
  bool limited = false;
  try {
    interpreter.parse(source, {2});
  } catch (const Error& e) {
    limited = e.code == ErrorCode::TooLarge;
  }
  CHECK(limited);
}
TEST_MAIN(run)
