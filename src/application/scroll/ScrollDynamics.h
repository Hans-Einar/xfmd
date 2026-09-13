#pragma once
#include <array>
#include <cstdint>
namespace xfmd {
struct ScrollProfile {
  double speed = 1.5;
  bool acceleration = false;
  double strength = 0.5, maxGain = 3.0;
};
enum class ScrollAxis { Horizontal, Vertical };
enum class ScrollOrigin { UserWheel, UserDrag, Keyboard, Sync, Restore };
struct ScrollInput {
  double delta = 0;
  std::uint64_t timestamp = 0; // Monotonic milliseconds, not finger/gesture time.
  ScrollAxis axis = ScrollAxis::Vertical;
  ScrollOrigin origin = ScrollOrigin::UserWheel;
};
class ScrollDynamics {
  struct Sample {
    std::uint64_t bucket = 0;
    double amount = 0;
  };
  std::array<Sample, 10> history{};
  double remainder = 0, direction = 0;
  std::uint64_t last = 0;
  bool received = false;

public:
  int advance(double delta, double unit, int base, int maximum);
  int advance(ScrollInput, const ScrollProfile&, double unit, int base, int maximum);
  void reset();
};
} // namespace xfmd
