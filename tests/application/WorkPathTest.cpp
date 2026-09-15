#include "application/workspace/DirectoryScanner.h"
#include "application/workspace/WorkPathHistory.h"
#include "support/TestSupport.h"
#include <chrono>
#include <fstream>
#include <set>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
std::set<std::string> collect(DirectoryScanner& scanner) {
  std::set<std::string> paths;
  for (int n = 0; n < 1000; ++n) {
    auto batch = scanner.take();
    for (const auto& entry : batch.entries)
      paths.insert(entry.path.filename().string());
    if (!batch.busy)
      return paths;
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  throw std::runtime_error("Scanner did not finish");
}
void run() {
  CHECK(FileNameFilter{true, true, "report?"}.matches("report.md") == false);
  CHECK(FileNameFilter{true, true, "report?.*"}.matches("Report1.MD"));
  CHECK(FileNameFilter{true, true, "report?.*"}.matches("reportø.txt"));
  CHECK(!FileNameFilter{true, false, "report*"}.matches("report1.txt"));
  CHECK(!FileNameFilter{false, true, "report*"}.matches("report1.md"));
  CHECK(FileNameFilter{false, false, "port"}.matches("REPORT.png"));
  CHECK(FileNameFilter{false, false, "?.md"}.matches("ø.md"));
  CHECK(!FileNameFilter{false, false, "?.md"}.matches("øa.md"));
  CHECK(FileNameFilter{false, false, "*"}.matches("*literal.md"));
  CHECK(FileNameFilter{}.matches("anything.png"));
  CHECK(FileNameFilter{true, true, ""}.matches("hello.MD"));
  CHECK(!FileNameFilter{true, true, ""}.matches("hello.png"));
  CHECK(FileNameFilter{false, false, "a*?c*"}.matches("abccc"));
  char pattern[] = "/tmp/xfmd-paths-XXXXXX";
  fs::path dir = mkdtemp(pattern);
  struct Cleanup {
    fs::path p;
    ~Cleanup() { fs::remove_all(p); }
  } cleanup{dir};
  fs::create_directories(dir / "home/sub/deep");
  fs::create_directory(dir / "outside");
  std::ofstream(dir / "outside/leak.md") << "outside";
  std::ofstream(dir / "home/sub/deep/reportø.md") << "# Markdown";
  std::ofstream(dir / "home/sub/report1.txt") << "text";
  std::ofstream(dir / "home/readme.png") << "other";
  fs::create_symlink(dir / "outside/leak.md", dir / "home/escape.md");
  fs::create_directory_symlink(dir / "home", dir / "home/sub/loop");
  WorkPathHistory history(dir / "home");
  history.activate(dir / "home/sub/..");
  CHECK(history.root() == dir / "home");
  CHECK(history.displayPath(dir / "home") == "~");
  CHECK(history.displayPath(dir / "home/sub") == "~/sub");
  CHECK(history.displayPath("/") == "/");
  history.activate(dir / "home/sub");
  CHECK(history.broaderRoot() == dir / "home");
  history.activate(history.broaderRoot());
  CHECK(history.broaderRoot() == "/");
  CHECK(history.entries().size() == 2);
  auto before = history.entries();
  try {
    history.activate(dir / "missing");
    CHECK(false);
  } catch (const fs::filesystem_error&) {
  }
  CHECK(history.entries() == before);
  CHECK(!WorkPathHistory::contains(dir / "home", dir / "homeother"));
  WorkPathHistory restored(dir / "home");
  restored.restore(history.entries());
  CHECK(restored.entries() == before);
  DirectoryScanner scanner;
  scanner.start(dir / "home", {});
  auto direct = collect(scanner);
  CHECK(direct == std::set<std::string>({"sub", "readme.png"}));
  scanner.request(dir / "home/sub");
  CHECK(collect(scanner) == std::set<std::string>({"deep", "report1.txt"}));
  scanner.start(dir / "home", {true, true, "report?.*"});
  CHECK(collect(scanner) == std::set<std::string>({"reportø.md", "report1.txt"}));
  scanner.start(dir / "home", {true, false, "missing"});
  CHECK(collect(scanner).empty());
  fs::create_directory(dir / "home/locked");
  fs::permissions(dir / "home/locked", fs::perms::none);
  scanner.start(dir / "home", {true, false, ""});
  collect(scanner);
  if (geteuid() != 0)
    CHECK(scanner.take().errors > 0);
  fs::permissions(dir / "home/locked", fs::perms::owner_all);
  // Cancel while publication is backpressured; no worker waits forever at destruction.
  for (int i = 0; i < 4200; ++i)
    std::ofstream(dir / "outside" / (std::to_string(i) + ".md"));
  scanner.start(dir / "outside", {true, false, ""});
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  scanner.start(dir / "home", {false, true, ""});
  CHECK(collect(scanner) == std::set<std::string>({"report1.txt"}));
  for (int i = 0; i < 35; ++i) {
    fs::create_directory(dir / std::to_string(i));
    history.activate(dir / std::to_string(i));
  }
  CHECK(history.entries().size() == 32);
  for (int i = 0; i < 100; ++i) {
    scanner.start(dir / "home", {});
    scanner.stop();
  }
}
TEST_MAIN(run)
