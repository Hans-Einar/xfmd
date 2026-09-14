#include "DesktopFileOpener.h"
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <spawn.h>
#include <stdexcept>
#include <sys/wait.h>
extern char** environ;
namespace xfmd {
void DesktopFileOpener::open(const std::string& path) {
  if (path.find('\0') != std::string::npos)
    throw std::runtime_error("Invalid file path.");
  auto target = std::filesystem::canonical(path);
  if (!std::filesystem::is_regular_file(target))
    throw std::runtime_error("Only regular files can be opened externally.");
  if (children.size() >= 16)
    throw std::runtime_error("Too many file-opening requests still running.");
  // An absolute path cannot be interpreted as an xdg-open option or URI scheme.
  auto name = target.string();
  const char* argv[] = {"xdg-open", name.c_str(), nullptr};
  pid_t pid;
  const int error =
      posix_spawnp(&pid, argv[0], nullptr, nullptr, const_cast<char**>(argv), environ);
  if (error)
    throw std::runtime_error(std::string("Cannot start xdg-open: ") + std::strerror(error));
  children.push_back({pid, std::move(name)});
}
std::vector<std::string> DesktopFileOpener::poll() {
  std::vector<std::string> errors;
  for (auto it = children.begin(); it != children.end();) {
    int status = 0;
    auto result = waitpid(it->pid, &status, WNOHANG);
    if (result == it->pid) {
      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        errors.push_back("xdg-open could not open: " + it->path);
      it = children.erase(it);
    } else if (result < 0 && errno == ECHILD)
      it = children.erase(it);
    else
      ++it;
  }
  return errors;
}
DesktopFileOpener::~DesktopFileOpener() { poll(); }
} // namespace xfmd
