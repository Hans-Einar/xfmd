#pragma once
#include <string>
#include <sys/types.h>
#include <vector>
namespace xfmd {
class DesktopFileOpener {
  struct Child {
    pid_t pid;
    std::string path;
  };
  std::vector<Child> children;

public:
  void open(const std::string& path);
  std::vector<std::string> poll();
  bool busy() const { return !children.empty(); }
  ~DesktopFileOpener();
};
} // namespace xfmd
