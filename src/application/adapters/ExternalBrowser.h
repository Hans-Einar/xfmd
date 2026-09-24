#pragma once
#include <functional>
#include <string>
#include <sys/types.h>
#include <vector>
namespace xfmd {
class ExternalBrowser {
  std::vector<pid_t> children;
  void launch(const std::string&, const std::string&);

public:
  static bool accepts(const std::string&);
  void open(const std::string& target, const std::string& program = "xdg-open");
  void openFile(const std::string& path, const std::string& program = "xdg-open");
  std::function<void(const std::string&)> failed;
  bool poll();
  ~ExternalBrowser();
};
} // namespace xfmd
