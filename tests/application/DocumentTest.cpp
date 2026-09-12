#include "application/document/DocumentCoordinator.h"
#include "application/document/EditController.h"
#include "application/document/TextProjection.h"
#include "application/io/InputPolicy.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
void run() {
  char pattern[] = "/tmp/xfmd-document-XXXXXX";
  fs::path dir = mkdtemp(pattern);
  struct Cleanup { fs::path path; ~Cleanup() { fs::remove_all(path); } } cleanup{dir};
  auto a = (dir / "æ space.md").string();
  auto b = (dir / "next.MD").string();
  const std::string original = "\xef\xbb\xbf# æøå\r\nrepeat\nrepeat\r\n";
  std::ofstream(a, std::ios::binary) << original;
  std::ofstream(b) << "next";
  chmod(a.c_str(), 0640);
  CHECK(setxattr(a.c_str(), "user.xfmd-test", "keep", 4, 0) == 0);
  LocalFileStore files;
  DocumentSession session;
  DocumentCoordinator docs(session, files);
  EditController edits(session);
  int opens = 0, errors = 0;
  docs.opened = [&] { ++opens; edits.reset(); };
  docs.error = [&](const std::string&) { ++errors; };
  CHECK(docs.requestOpen(a));
  CHECK(opens == 1 && !session.dirty());
  CHECK(session.view().text == original);
  TextProjection projection(original);
  CHECK(projection.text == "# æøå\nrepeat\nrepeat\n");
  edits.applyProjectedText("# Æøå\nrepeat\nrepeat\n");
  CHECK(session.dirty());
  CHECK(session.view().text == "\xef\xbb\xbf# Æøå\r\nrepeat\nrepeat\r\n");
  edits.undo(); CHECK(!session.dirty() && session.view().text == original);
  edits.redo(); CHECK(session.dirty());
  docs.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(!docs.requestOpen(b) && opens == 1);
  docs.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(!docs.requestOpen((dir / "missing.md").string()) && session.dirty());
  CHECK(docs.save());
  CHECK(!session.dirty());
  struct stat stat{}; CHECK(::stat(a.c_str(), &stat) == 0 && (stat.st_mode & 0777) == 0640);
  char attr[4]; CHECK(getxattr(a.c_str(), "user.xfmd-test", attr, sizeof(attr)) == 4);
  CHECK(std::string(attr, 4) == "keep");
  edits.applyProjectedText("changed\n");
  auto before = files.read(a).text;
  files.checkpoint = [](const char*) { throw Error(ErrorCode::Io, "injected disk failure"); };
  CHECK(!docs.save() && session.dirty() && files.read(a).text == before);
  files.checkpoint = {};
  std::ofstream(a) << "external";
  CHECK(!docs.save() && files.read(a).text == "external" && session.dirty());
  auto saveAs = (dir / "new.md").string();
  CHECK(docs.save(saveAs));
  edits.applyProjectedText("second\n");
  CHECK(docs.save()); // New-file identity must survive temporary link cleanup.
  CHECK(files.read(saveAs).text == session.view().text);
  fs::create_hard_link(saveAs, dir / "hard.md");
  CHECK(!docs.save());
  CHECK(docs.requestOpen(b));
  CHECK(opens == 2 && !session.dirty() && !edits.canUndo());
  for (const auto& text : {std::string("\0", 1), std::string("\xc0\xaf", 2), std::string("\xed\xa0\x80", 3), std::string("\xf4\x90\x80\x80", 4)}) {
    bool rejected = false;
    try { InputPolicy::validate(text); } catch (const Error&) { rejected = true; }
    CHECK(rejected);
  }
  bool tooLarge = false;
  try { InputPolicy::validate(std::string(maxDocumentBytes + 1, 'a')); } catch (const Error& e) { tooLarge = e.code == ErrorCode::TooLarge; }
  CHECK(tooLarge);
  CHECK(errors >= 4);
  CHECK(TextProjection("\xef\xbb\xbf").sourceOffset(0) == 3);
  CHECK(edits.find("next", 0) == 0);
}
TEST_MAIN(run)
