#include "application/navigation/HeadingAnchor.h"
#include "application/navigation/LinkResolver.h"
#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  auto absolute =
      LinkResolver::documentLink("/source/start.md", "/somePath/someFile.md#SomeLocationInTheFile");
  CHECK(absolute.path == "/somePath/someFile.md" && *absolute.fragment == "SomeLocationInTheFile");
  auto escaped =
      LinkResolver::documentLink("/source/start.md", "../my%20%23file.md#Hello%20%C3%A6");
  CHECK(escaped.path == "/my #file.md" && *escaped.fragment == "Hello æ");
  CHECK(LinkResolver::documentLink("/source/start.md", "#").fragment->empty());
  CHECK(LinkResolver::documentLink("", "#new").path.empty());
  for (const auto* bad : {"a.md?query#h", "javascript:bad#h", "//host/a.md#h", "a.md#%00",
                          "a.md#%xx", "a.md#%09", "%00.md#h", "javascript%3Abad#h"}) {
    bool rejected = false;
    try {
      LinkResolver::documentLink("/start.md", bad);
    } catch (const std::exception&) {
      rejected = true;
    }
    CHECK(rejected);
  }
  const std::string text = "# Start\n\n## SomeLocationInTheFile\n\n## Hello *world*!\n\n## Hello "
                           "*world*!\n\n## Hello æ\n";
  auto model = CmarkInterpreter().parse({{1, 1}, text, "/target.md"});
  CHECK(HeadingAnchor::find(*model, "SomeLocationInTheFile"));
  CHECK(HeadingAnchor::find(*model, "SomeLocationInTheFile")->byte == text.find("## Some"));
  CHECK(HeadingAnchor::find(*model, "somelocationinthefile")->byte == text.find("## Some"));
  CHECK(HeadingAnchor::find(*model, "hello-world")->byte == text.find("## Hello"));
  CHECK(HeadingAnchor::find(*model, "hello-world-1")->byte ==
        text.find("## Hello", text.find("## Hello") + 1));
  CHECK(HeadingAnchor::find(*model, "Hello world!")->byte == text.find("## Hello"));
  CHECK(HeadingAnchor::find(*model, "hello-æ")->byte == text.rfind("## Hello"));
  CHECK(HeadingAnchor::find(*model, "")->byte == 0);
  CHECK(!HeadingAnchor::find(*model, "missing"));
}
TEST_MAIN(run)
