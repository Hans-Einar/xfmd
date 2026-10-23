#include "application/sdp/SdpRuntime.h"
#include "support/TestSupport.h"
#include <cstdlib>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
void run() {
  char pattern[] = "/tmp/xfmd-runtime-test-XXXXXX";
  auto* created = mkdtemp(pattern);
  CHECK(created);
  fs::path root(created);
  const char* xdg = getenv("XDG_RUNTIME_DIR");
  const bool hadXdg = xdg != nullptr;
  const std::string previous = xdg ? xdg : "";
  struct Restore {
    fs::path root;
    bool had;
    std::string value;
    ~Restore() {
      if (had)
        setenv("XDG_RUNTIME_DIR", value.c_str(), 1);
      else
        unsetenv("XDG_RUNTIME_DIR");
      fs::remove_all(root);
    }
  } restore{root, hadXdg, previous};
  auto fails = [&](const fs::path& path) {
    setenv("XDG_RUNTIME_DIR", path.c_str(), 1);
    bool rejected = false;
    try {
      sdpRuntimeWorkspace();
    } catch (const std::exception&) {
      rejected = true;
    }
    CHECK(rejected);
  };
  fails(root / "missing");
  fails("relative-runtime");
  auto unsafe = root / "unsafe";
  fs::create_directory(unsafe);
  chmod(unsafe.c_str(), 0755);
  fails(unsafe);
  auto link = root / "link";
  fs::create_directory_symlink(root, link);
  fails(link);
  setenv("XDG_RUNTIME_DIR", root.c_str(), 1);
  auto first = sdpRuntimeWorkspace(), second = sdpRuntimeWorkspace();
  CHECK(first != second && first.parent_path() == root / "xfmd");
  struct stat info{};
  CHECK(!lstat(first.c_str(), &info) && (info.st_mode & 0777) == 0700);
  fs::remove_all(root / "xfmd");
  fs::create_directory_symlink(unsafe, root / "xfmd");
  fails(root);
  CHECK(fs::is_empty(unsafe));
}
TEST_MAIN(run)
