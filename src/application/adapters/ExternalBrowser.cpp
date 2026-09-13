#include "ExternalBrowser.h"
#include <algorithm>
#include <cerrno>
#include <cstring>
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
void ExternalBrowser::open(const std::string& target) {
  if (!accepts(target))
    throw std::runtime_error("Only HTTP(S) can be opened in the browser.");
  poll();
  if (children.size() >= 16)
    throw std::runtime_error("Too many browser requests still running.");
  const char* argv[] = {"xdg-open", target.c_str(), nullptr};
  pid_t child;
  const int error =
      posix_spawnp(&child, argv[0], nullptr, nullptr, const_cast<char**>(argv), environ);
  if (error)
    throw std::runtime_error(std::string("Cannot start browser: ") + std::strerror(error));
  children.push_back(child);
}
bool ExternalBrowser::poll() {
  children.erase(std::remove_if(children.begin(), children.end(),
                                [](pid_t child) {
                                  int status;
                                  auto result = waitpid(child, &status, WNOHANG);
                                  return result == child || (result < 0 && errno == ECHILD);
                                }),
                 children.end());
  return !children.empty();
}
ExternalBrowser::~ExternalBrowser() { poll(); }
} // namespace xfmd
