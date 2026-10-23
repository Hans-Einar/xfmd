#pragma once
#include "SdpToolJob.h"
#include <filesystem>
namespace xfmd {
class Application;
class SduiWorkflow {
  Application& app;
  SdpToolJob probe;
  std::optional<SdpTarget> target;
  std::string key, reason, host;
  bool pending = false, ready = false;
  struct Child {
    int pid;
    std::filesystem::path log;
  };
  std::vector<Child> children;
  void poll();
  std::string contextKey() const;

public:
  explicit SduiWorkflow(Application&);
  ~SduiWorkflow();
  bool canLaunch() const;
  void launch();
};
} // namespace xfmd
