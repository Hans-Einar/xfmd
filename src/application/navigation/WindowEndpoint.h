#pragma once
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <vector>
namespace xfmd {
class WindowEndpoint {
  int listener = -1;
  std::string socketPath, window;
  std::map<std::string, unsigned long long> sequences;
  struct Client {
    int fd;
    std::chrono::steady_clock::time_point deadline;
  };
  std::vector<Client> clients;

public:
  static std::string runtimeDirectory();
  static std::string request(const std::string& window, const std::string& message);
  std::function<bool(const std::string&, const std::string&)> open;
  std::function<std::string()> info;
  explicit WindowEndpoint(const std::string& id);
  ~WindowEndpoint();
  void poll();
  std::string handle(const std::string&);
};
} // namespace xfmd
