#include "ScrollDynamics.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
int ScrollDynamics::advance(double delta, double unit, int base, int maximum) {
  if (!std::isfinite(delta) || !std::isfinite(unit) || unit < 0) return base;
  maximum = std::max(0, maximum);
  const double movement = remainder - delta * unit;
  const double whole = std::trunc(movement + std::copysign(1e-10, movement));
  remainder = movement - whole;
  if (std::abs(remainder) < 1e-10) remainder = 0;
  const int target = int(std::clamp(double(base) + whole, 0.0, double(maximum)));
  if ((target == 0 && movement < 0) || (target == maximum && movement > 0)) remainder = 0;
  return target;
}
} // namespace xfmd
