#pragma once
#include <functional>
namespace xfmd {
class IScheduler {
public:
  virtual ~IScheduler() = default;
  virtual void restart(unsigned key, unsigned milliseconds, std::function<void()>) = 0;
  virtual void cancel(unsigned key) = 0;
};
} // namespace xfmd
