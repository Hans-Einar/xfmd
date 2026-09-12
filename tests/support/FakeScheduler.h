#pragma once
#include "application/adapters/IScheduler.h"
#include <map>
namespace xfmd {
class FakeScheduler : public IScheduler {
  struct Timer { unsigned at; std::function<void()> callback; };
  std::map<unsigned, Timer> timers;
public:
  unsigned now = 0;
  void restart(unsigned key, unsigned milliseconds, std::function<void()> callback) override {
    timers[key] = {now + milliseconds, std::move(callback)};
  }
  void cancel(unsigned key) override { timers.erase(key); }
  void advance(unsigned milliseconds) {
    auto end = now + milliseconds;
    for (;;) {
      auto next = timers.end();
      for (auto it = timers.begin(); it != timers.end(); ++it)
        if (it->second.at <= end && (next == timers.end() || it->second.at < next->second.at)) next = it;
      if (next == timers.end()) break;
      now = next->second.at;
      auto callback = std::move(next->second.callback); timers.erase(next); callback();
    }
    now = end;
  }
  bool empty() const { return timers.empty(); }
};
}
