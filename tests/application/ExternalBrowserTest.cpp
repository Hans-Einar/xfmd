#include "application/adapters/ExternalBrowser.h"
#include "support/TestSupport.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void run() {
  for (const char* target :
       {"javascript:evil", "file:///tmp/a", "//host", "http://", "https://a\nb"})
    CHECK(!ExternalBrowser::accepts(target));
  CHECK(ExternalBrowser::accepts("https://example.org/test?x=1#test"));
  char pattern[] = "/tmp/xfmd-browser-XXXXXX";
  std::filesystem::path dir = mkdtemp(pattern);
  const std::string previous = std::getenv("PATH");
  struct Cleanup {
    std::filesystem::path path;
    std::string previous;
    ~Cleanup() {
      setenv("PATH", previous.c_str(), 1);
      std::filesystem::remove_all(path);
    }
  } cleanup{dir, previous};
  std::ofstream(dir / "xdg-open")
      << "#!/bin/sh\nprintf '%s\\n' \"$#\" \"$1\" > \"$XFMD_BROWSER_ARGS\"\n";
  std::filesystem::permissions(dir / "xdg-open", std::filesystem::perms::owner_all);
  setenv("PATH", dir.c_str(), 1);
  setenv("XFMD_BROWSER_ARGS", (dir / "args").c_str(), 1);
  ExternalBrowser browser;
  const std::string url = "https://example.org/;echo${IFS}oops$(touch${IFS}bad)";
  browser.open(url);
  for (int i = 0; i < 1000 && browser.poll(); ++i)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  CHECK(!browser.poll());
  std::ifstream result(dir / "args");
  std::string count, argument;
  std::getline(result, count);
  std::getline(result, argument);
  CHECK(count == "1" && argument == url);
  const auto program = dir / "browser with spaces;literal";
  std::filesystem::copy(dir / "xdg-open", program);
  std::filesystem::remove(dir / "args");
  browser.open(url, program.string());
  for (int i = 0; i < 1000 && browser.poll(); ++i)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  CHECK(!browser.poll());
  std::ifstream custom(dir / "args");
  std::getline(custom, count);
  std::getline(custom, argument);
  CHECK(count == "1" && argument == url);
  bool failed = false;
  try {
    browser.open(url, "missing-browser");
  } catch (const std::exception& e) {
    failed = std::string(e.what()).find("missing-browser") != std::string::npos;
  }
  CHECK(failed);
  for (const auto& programName : {std::string{}, std::string("a\0b", 3)}) {
    failed = false;
    try {
      browser.open(url, programName);
    } catch (const std::exception&) {
      failed = true;
    }
    CHECK(failed);
  }
}
TEST_MAIN(run)
