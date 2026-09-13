#include "application/index/DocumentIndex.h"
#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  CmarkInterpreter parser;
  std::string source = "## Første **kapittel**\n\n#### Underkapittel\n\n## Neste\n\n"
      "[one **bold**](other%20file.md#intro) [again](./other%20file.md)\n\n"
      "| File | Web |\n| --- | --- |\n| [ref](sub/../third.md) | [site](https://example.org/a.md) |\n\n"
      "[unsafe](javascript:evil) [local](#first) [text](readme.txt)\n";
  auto model = parser.parse({{1, 2}, source, "/tmp/index/start.md", false});
  auto index = DocumentIndex::build(*model, "/tmp/index/start.md");
  CHECK((index.token == DocumentToken{1, 2}));
  CHECK(index.headings.size() == 3 && index.headings[0].title == "Første kapittel");
  CHECK(index.headings[1].level == 4 && index.headings[1].anchor.byte == source.find("####"));
  CHECK(index.markdown.size() == 2);
  CHECK(index.markdown[0].path == "/tmp/index/other file.md");
  CHECK(index.markdown[0].label == "one bold");
  CHECK(index.markdown[1].path == "/tmp/index/third.md");
  CHECK(index.hyperlinks.size() == 4);
  auto top = DocumentIndex::topLevel(*model);
  CHECK(top.size() == 2 && top[1].title == "Neste");
  auto unsaved = DocumentIndex::build(*model, "");
  CHECK(!unsaved.markdown.empty() && !unsaved.markdown[0].error.empty());
  auto empty = parser.parse({{}, "plain text\n```\n# not a heading\n```\n", "", false});
  CHECK(DocumentIndex::build(*empty, "").headings.empty());
}
TEST_MAIN(run)
