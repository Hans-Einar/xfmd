#include "application/ui/style/ThemeProfiles.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
void run() {
  ThemeProfiles profiles;
  Appearance a;
  auto initial = profiles.palette(a).accent;
  auto dir = std::filesystem::temp_directory_path() / ("xfmd-theme-" + std::to_string(getpid()));
  std::filesystem::create_directories(dir);
  auto path = dir / "appearance.ini";
  std::string error;
  std::ofstream(path) << "[light]\naccent=#123456\n[compact]\nheight=30\n";
  CHECK(profiles.load(path.string(), error));
  CHECK(profiles.palette(a).accent == FXRGB(18, 52, 86));
  a.compact = true;
  CHECK(profiles.metrics(a).height == 30);
  a.fontSize = 15;
  CHECK(profiles.metrics(a).height == 45);
  std::ofstream(path) << "[light]\naccent=wrong\n";
  CHECK(!profiles.load(path.string(), error));
  CHECK(profiles.palette(a).accent == FXRGB(18, 52, 86));
  std::ofstream(path) << "[compact]\nheight=100000\n";
  CHECK(!profiles.load(path.string(), error));
  CHECK(profiles.metrics(a).height == 45);
  std::filesystem::remove(path);
  CHECK(profiles.load(path.string(), error));
  CHECK(profiles.palette(a).accent == initial);
  std::filesystem::remove_all(dir);
}
TEST_MAIN(run)
