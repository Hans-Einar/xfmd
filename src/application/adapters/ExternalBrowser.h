#pragma once
#include <string>
#include <sys/types.h>
#include <vector>
namespace xfmd {
class ExternalBrowser {
  std::vector<pid_t> children;

public:
  static bool accepts(const std::string&);
  void open(const std::string& target, const std::string& program = "xdg-open");
  bool poll();
  ~ExternalBrowser();
};
} // namespace xfmd
