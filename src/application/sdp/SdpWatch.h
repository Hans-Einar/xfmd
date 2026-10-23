#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
namespace xfmd {
// Metadata only; SDL/SDUI/KanBan interpretation remains producer-owned.
class SdpWatch {
  std::atomic<bool> stopped{true};
  std::thread worker;
  std::mutex mutex;
  std::condition_variable wake;
  bool changed = false;
  std::string error;

public:
  ~SdpWatch();
  void start(const std::string& root);
  void stop();
  bool take(std::string& diagnostic);
};
} // namespace xfmd
