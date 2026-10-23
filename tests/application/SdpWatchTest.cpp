#include "application/sdp/SdpWatch.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
using namespace std::chrono_literals;
void changed(SdpWatch& watch) {
  for (int i = 0; i < 100; ++i) {
    std::string error;
    bool result = watch.take(error);
    CHECK(error.empty());
    if (result)
      return;
    std::this_thread::sleep_for(50ms);
  }
  CHECK(false);
}
void run() {
  auto root = fs::temp_directory_path() / ("xfmd-watch-" + std::to_string(getpid()));
  fs::create_directories(root);
  struct Cleanup {
    fs::path path;
    ~Cleanup() { fs::remove_all(path); }
  } cleanup{root};
  SdpWatch watch;
  watch.start(root.string());
  changed(watch);
  fs::create_directories(root / "SDP/SDL");
  auto source = root / "SDP/SDL/example.design";
  std::ofstream(source) << "first";
  changed(watch);
  const auto timestamp = fs::last_write_time(source);
  std::ofstream(source) << "other"; // Same-size edit must still invalidate navigation.
  fs::last_write_time(source, timestamp + 1s);
  changed(watch);
  fs::rename(source, root / "SDP/SDL/renamed.design");
  changed(watch);
  fs::create_directories(root / "outside");
  std::ofstream(root / "outside/file") << "original";
  fs::create_directory_symlink(root / "outside", root / "SDP/linked");
  changed(watch);
  std::ofstream(root / "outside/file") << "changed outside";
  std::this_thread::sleep_for(1000ms);
  std::string error;
  CHECK(!watch.take(error) && error.empty());
  fs::remove_all(root / "SDP");
  changed(watch);
  watch.start((root / "outside").string());
  changed(watch);
  fs::create_directories(root / "SDP");
  std::this_thread::sleep_for(1000ms);
  CHECK(!watch.take(error) && error.empty()); // Old root is no longer observed.
  auto before = std::chrono::steady_clock::now();
  watch.stop();
  CHECK(std::chrono::steady_clock::now() - before < 500ms);
  auto deep = root / "SDP";
  for (int i = 0; i < 34; ++i) {
    deep /= "child";
    fs::create_directory(deep);
  }
  watch.start(root.string());
  for (int i = 0; i < 100 && error.empty(); ++i) {
    watch.take(error);
    std::this_thread::sleep_for(50ms);
  }
  CHECK(error.find("limit") != std::string::npos && error.find("Refresh") != std::string::npos);
}
TEST_MAIN(run)
