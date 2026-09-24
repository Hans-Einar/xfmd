#include "ExternalBrowser.h"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <spawn.h>
#include <stdexcept>
#include <sys/wait.h>
extern char** environ;
namespace xfmd {
bool ExternalBrowser::accepts(const std::string& target) {
  std::size_t start = target.compare(0, 8, "https://") == 0  ? 8
                      : target.compare(0, 7, "http://") == 0 ? 7
                                                             : 0;
  if (!start || target.size() <= start || target[start] == '/' || target[start] == '#')
    return false;
  return std::none_of(target.begin(), target.end(),
                      [](unsigned char c) { return c <= 32 || c == 127; });
}
void ExternalBrowser::open(const std::string& target, const std::string& program) {
  if (!accepts(target))
    throw std::runtime_error("Only HTTP(S) can be opened in the browser.");
  launch(target, program);
}
void ExternalBrowser::openFile(const std::string& path, const std::string& program) {
  const auto resolved = std::filesystem::canonical(path);
  if (!std::filesystem::is_regular_file(resolved))
    throw std::runtime_error("Browser target must be a regular file.");
  std::string url = "file://";
  constexpr char hex[] = "0123456789ABCDEF";
  for (unsigned char c : resolved.string()) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '/' ||
        c == '-' || c == '_' || c == '.' || c == '~')
      url += char(c);
    else {
      url += '%';
      url += hex[c >> 4];
      url += hex[c & 15];
    }
  }
  launch(url, program);
}
void ExternalBrowser::launch(const std::string& target, const std::string& program) {
  poll();
  if (children.size() >= 16)
    throw std::runtime_error("Too many browser requests still running.");
  if (program.empty() || program.find('\0') != std::string::npos)
    throw std::runtime_error("Choose a browser program in Edit → Preferences.");
  const char* argv[] = {program.c_str(), target.c_str(), nullptr};
  pid_t child;
  const int error =
      posix_spawnp(&child, argv[0], nullptr, nullptr, const_cast<char**>(argv), environ);
  if (error)
    throw std::runtime_error("Cannot start browser '" + program + "': " + std::strerror(error) +
                             ". Check Edit → Preferences.");
  children.push_back(child);
}
bool ExternalBrowser::poll() {
  bool launchFailed = false;
  children.erase(std::remove_if(children.begin(), children.end(),
                                [&launchFailed](pid_t child) {
                                  int status = 0;
                                  auto result = waitpid(child, &status, WNOHANG);
                                  if (result == child &&
                                      (!WIFEXITED(status) || WEXITSTATUS(status)))
                                    launchFailed = true;
                                  return result == child || (result < 0 && errno == ECHILD);
                                }),
                 children.end());
  if (launchFailed && failed)
    failed("The browser could not open the requested target.");
  return !children.empty();
}
ExternalBrowser::~ExternalBrowser() {
  failed = {};
  poll();
}
} // namespace xfmd
