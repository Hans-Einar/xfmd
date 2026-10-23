#include "SdpRuntime.h"
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
namespace xfmd {
namespace {
void validate(const std::filesystem::path& path) {
  struct stat info{};
  if (!path.is_absolute() || lstat(path.c_str(), &info) || !S_ISDIR(info.st_mode) ||
      info.st_uid != getuid() || (info.st_mode & 077) || (info.st_mode & 0700) != 0700)
    throw std::runtime_error("SDP runtime directory must exist, be owned by this user and have "
                             "mode 0700: " +
                             path.string());
}
} // namespace
std::filesystem::path sdpRuntimeWorkspace() {
  const char* xdg = std::getenv("XDG_RUNTIME_DIR");
  const std::filesystem::path base =
      xdg && *xdg ? std::string(xdg) : "/run/user/" + std::to_string(getuid());
  validate(base);
  const auto directory = base / "xfmd";
  if (::mkdir(directory.c_str(), 0700) && errno != EEXIST)
    throw std::runtime_error("Cannot create SDP runtime directory: " + directory.string());
  validate(directory);
  auto pattern = (directory / "xfmd-sdp-XXXXXX").string();
  if (!mkdtemp(pattern.data()))
    throw std::runtime_error("Cannot allocate SDP runtime workspace: " + directory.string());
  return pattern;
}
} // namespace xfmd
