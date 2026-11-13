#include "SduiTools.h"
#include "SdpToolJob.h"
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
namespace xfmd {
namespace {
const char* overrideFor(const std::string& name) {
  return std::getenv(name == "sdui-fyne" ? "XFMD_SDUI_FYNE" : "XFMD_SDUI_PREVIEW");
}
}
std::string sduiExecutable(const std::string& name) {
  const char* override = overrideFor(name);
  const auto command = sdpCommand();
  const std::filesystem::path path = override && *override ? override : command.front();
  std::error_code ec;
  return path.is_absolute() && std::filesystem::is_regular_file(path, ec) &&
                 access(path.c_str(), X_OK) == 0 ? path.string() : "";
}
std::vector<std::string> sduiCommand(const std::string& operation) {
  const std::string name = operation == "run" ? "sdui-fyne" : "sdui-preview";
  auto executable = sduiExecutable(name);
  if (executable.empty()) return {};
  const char* override = overrideFor(name);
  if (override && *override) {
    if (operation == "check") return {executable, "-check"};
    return {executable};
  }
  return {executable, "sdui", operation};
}
} // namespace xfmd
