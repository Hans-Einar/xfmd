#include "SduiTools.h"
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
namespace xfmd {
std::string sduiExecutable(const std::string& name) {
  const char* override = std::getenv(name == "sdui-fyne" ? "XFMD_SDUI_FYNE" : "XFMD_SDUI_PREVIEW");
  auto usable = [](const std::filesystem::path& path) {
    std::error_code ec;
    return path.is_absolute() && std::filesystem::is_regular_file(path, ec) &&
           access(path.c_str(), X_OK) == 0;
  };
  if (override && *override)
    return usable(override) ? override : "";
  std::error_code ec;
  auto parent = std::filesystem::read_symlink("/proc/self/exe", ec).parent_path();
  for (const auto& base : {parent / "libexec/xfmd", parent.parent_path() / "libexec/xfmd"}) {
    auto path = base / name;
    if (usable(path))
      return path.string();
  }
  return {};
}
} // namespace xfmd
