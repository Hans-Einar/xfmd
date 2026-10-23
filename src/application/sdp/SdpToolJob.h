#pragma once
#include "SdpProtocol.h"
#include <atomic>
#include <chrono>
#include <mutex>
#include <optional>
#include <thread>
namespace xfmd {
struct SdpJobResult {
  std::optional<SdpReply> reply;
  std::string error;
};
class SdpToolJob {
  std::thread worker;
  std::atomic<bool> canceled{false};
  std::mutex mutex;
  std::optional<SdpJobResult> completed;

public:
  ~SdpToolJob();
  void start(std::vector<std::string> argv,
             std::chrono::milliseconds deadline = std::chrono::seconds(65));
  void cancel();
  std::optional<SdpJobResult> take();
};
std::vector<std::string> sdpCommand();
} // namespace xfmd
