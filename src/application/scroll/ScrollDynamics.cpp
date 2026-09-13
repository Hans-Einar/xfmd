#include "ScrollDynamics.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
int ScrollDynamics::advance(double delta, double unit, int base, int maximum) {
  if (!std::isfinite(delta) || !std::isfinite(unit) || unit < 0)
    return base;
  maximum = std::max(0, maximum);
  const double movement = remainder - delta * unit;
  const double whole = std::trunc(movement + std::copysign(1e-10, movement));
  remainder = movement - whole;
  if (std::abs(remainder) < 1e-10)
    remainder = 0;
  const int target = int(std::clamp(double(base) + whole, 0.0, double(maximum)));
  if ((target == 0 && movement < 0) || (target == maximum && movement > 0))
    remainder = 0;
  return target;
}
} // namespace xfmd
namespace xfmd {
void ScrollDynamics::reset() {
  remainder = direction = 0;
  last = 0;
  received = false;
  history = {};
}
int ScrollDynamics::advance(ScrollInput input, const ScrollProfile& profile, double unit, int base,
                            int maximum) {
  if (input.origin != ScrollOrigin::UserWheel)
    return std::clamp(base, 0, std::max(0, maximum));
  if (!std::isfinite(input.delta) || input.delta == 0)
    return base;
  if (received &&
      (input.timestamp < last || input.timestamp - last >= 200 || input.delta * direction < 0))
    reset();
  bool continuation = received;
  received = true;
  last = input.timestamp;
  direction = input.delta;
  auto bucket = input.timestamp / 8;
  auto& slot = history[bucket % history.size()];
  if (slot.bucket != bucket)
    slot = {bucket, 0};
  slot.amount += std::abs(input.delta);
  double total = 0;
  for (const auto& sample : history)
    if (sample.bucket <= bucket && bucket - sample.bucket < 10)
      total += sample.amount;
  double gain = 1;
  if (profile.acceleration && (continuation || std::abs(input.delta) > 1))
    gain = std::clamp(1 + profile.strength * std::max(0.0, total / .080 / 8 - 1), 1.0,
                      profile.maxGain);
  return advance(input.delta, unit * profile.speed * gain, base, maximum);
}
} // namespace xfmd
