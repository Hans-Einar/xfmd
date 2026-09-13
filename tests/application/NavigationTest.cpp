#include "application/navigation/LinkResolver.h"
#include "application/navigation/NavigationCoordinator.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
void run() {
  char pattern[] = "/tmp/xfmd-nav-XXXXXX";
  std::filesystem::path dir = mkdtemp(pattern);
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() { std::filesystem::remove_all(path); }
  } cleanup{dir};
  for (const char* name : {"a.md", "b space.md", "c.md", "d.md"})
    std::ofstream(dir / name) << name;
  DocumentSession session;
  LocalFileStore files;
  DocumentCoordinator docs(session, files);
  ScrollCoordinator scroll;
  NavigationCoordinator nav(docs, session, scroll);
  int errors = 0, commits = 0;
  docs.error = [&](const std::string&) { ++errors; };
  nav.error = docs.error;
  docs.opened = [&] {
    ++commits;
    nav.commitVisit();
  };
  CHECK(nav.openTarget((dir / "a.md").string()));
  scroll.restoreAnchor({3});
  CHECK(nav.followLink("b%20space.md"));
  CHECK(nav.history.size() == 2 && nav.history.at(0).anchor.byte == 3);
  CHECK(nav.followLink("c.md"));
  CHECK(nav.goBack() && nav.history.position() == 1 && nav.history.size() == 3);
  CHECK(nav.followLink("d.md") && !nav.history.propose(false));
  CHECK(nav.history.size() == 3 && nav.history.at(2).path == (dir / "d.md").string());
  session.applyEdit({0, 0, "dirty"});
  docs.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  auto before = commits;
  CHECK(!nav.goBack() && nav.history.position() == 2 && commits == before);
  docs.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(!nav.followLink("missing.md") && commits == before && session.dirty());
  for (const char* target :
       {"https://example.org/a.md", "javascript:alert(1)", "data:text/plain,a", "//host/a.md",
        "a.md#title", "%00.md", "a%xx.md", "javascript%3Aevil.md"}) {
    CHECK(!nav.followLink(target));
    CHECK(commits == before);
  }
  CHECK(nav.goBack() && session.view().path == (dir / "b space.md").string());
  CHECK(scroll.captureAnchor().byte == nav.history.at(1).anchor.byte);
  auto count = nav.history.size();
  CHECK(nav.openTarget(session.view().path));
  CHECK(nav.history.size() == count);
  CHECK(errors >= 9);
  auto sameToken = session.view().token;
  session.applyEdit({0, 0, "dirty"});
  sameToken = session.view().token;
  docs.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(nav.openAt(session.view().path, {4}));
  CHECK(session.view().token == sameToken && session.dirty() && scroll.captureAnchor().byte == 4);
  CHECK(!nav.openAt((dir / "a.md").string(), {2}));
  CHECK(session.view().token == sameToken && scroll.captureAnchor().byte == 4);
  docs.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(nav.openAt((dir / "a.md").string(), {2}));
  CHECK(scroll.captureAnchor().byte == 2);
  CHECK(nav.history.at(nav.history.position() - 1).anchor.byte == 4);
  HistoryStore many;
  for (int i = 0; i < 105; ++i)
    many.commit(std::to_string(i), {});
  CHECK(many.size() == 100 && many.position() == 99 && many.at(0).path == "5");
  CHECK(LinkResolver::resolve((dir / "a.md").string(), "../file.md") ==
        (dir.parent_path() / "file.md").string());
}
TEST_MAIN(run)
