#pragma once
namespace xfmd {
struct ScrollProfile {
  double speed = 1.5;
  bool acceleration = false;
  double strength = 0.5, maxGain = 3.0;
};
// Values are logical wheel units, independent of FOX and device drivers.
class ScrollDynamics {
  double remainder = 0;
public:
  int advance(double delta, double unit, int base, int maximum);
  void reset() { remainder = 0; }
};
} // namespace xfmd
