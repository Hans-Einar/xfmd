#include "application/io/FileOpenPolicy.h"
#include "application/io/LocalFileStore.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
void run() {
  char pattern[] = "/tmp/xfmd-file-policy-XXXXXX";
  std::filesystem::path root = mkdtemp(pattern);
  struct Cleanup {
    std::filesystem::path path;
    ~Cleanup() { std::filesystem::remove_all(path); }
  } cleanup{root};
  auto fixture = [&](const char* name, const std::string& bytes) {
    auto p = root / name;
    std::ofstream(p, std::ios::binary).write(bytes.data(), bytes.size());
    return p.string();
  };
  LocalFileStore store;
  for (const char* name :
       {"README", "settings.json", "source.CPP", "empty", "space æ.log", "fake.png"}) {
    auto path = fixture(name, name == std::string("empty") ? "" : "# literal æ\r\n");
    CHECK(FileOpenPolicy::classify(path) == FileOpenKind::Text);
    auto loaded = store.read(path);
    CHECK(loaded.plainText);
    auto updated = loaded.text + "more\n";
    store.writeAtomic({{1, 1}, updated, path, true}, path, loaded.identity);
    CHECK(store.read(path).text == updated);
  }
  CHECK(FileOpenPolicy::classify(fixture("document.MD", "# heading")) == FileOpenKind::Markdown);
  CHECK(FileOpenPolicy::classify(fixture("document.TXT", "# literal")) == FileOpenKind::Text);
  CHECK(FileOpenPolicy::classify(fixture("page.HTmL", "<h1>hello</h1>")) == FileOpenKind::Browser);
  for (const auto& bytes :
       {std::string("\0binary", 7), std::string("\x01\x02", 2), std::string("\xffinvalid", 8)})
    CHECK(FileOpenPolicy::classify(fixture("binary.data", bytes)) == FileOpenKind::Desktop);
  auto large = fixture("large.data", "");
  std::filesystem::resize_file(large, maxDocumentBytes + 1);
  CHECK(FileOpenPolicy::classify(large) == FileOpenKind::Desktop);
  auto invalid = fixture("invalid.txt", std::string("\0", 1));
  CHECK(FileOpenPolicy::classify(invalid) == FileOpenKind::Text);
  bool rejected = false;
  try {
    store.read(invalid);
  } catch (const Error&) {
    rejected = true;
  }
  CHECK(rejected);
  rejected = false;
  try {
    FileOpenPolicy::classify((root / "missing").string());
  } catch (const std::exception&) {
    rejected = true;
  }
  CHECK(rejected);
  CHECK(store.read(fixture("lines", "\xef\xbb\xbfline\r\nnext\n")).text ==
        "\xef\xbb\xbfline\r\nnext\n");
}
TEST_MAIN(run)
