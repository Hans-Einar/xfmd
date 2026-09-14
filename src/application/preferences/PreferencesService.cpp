#include "PreferencesService.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
bool PreferencesService::validate(const PreferencesSnapshot& s, std::string& error) {
  const auto within = [](double value, double min, double max) {
    return std::isfinite(value) && value >= min && value <= max;
  };
  if (s.version != 1)
    error = "Unsupported preferences version.";
  else if (!within(s.scroll.speed, .25, 4))
    error = "Scroll speed must be between 0.25 and 4.";
  else if (!within(s.scroll.strength, 0, 2))
    error = "Acceleration strength must be between 0 and 2.";
  else if (!within(s.scroll.maxGain, 1, 5))
    error = "Maximum acceleration must be between 1 and 5.";
  else if (!within(s.marginMm, 5, 50))
    error = "Page margins must be between 5 and 50 mm.";
  else if ((s.appearance.theme != "light" && s.appearance.theme != "dark") ||
           (s.appearance.buttons != "flat" && s.appearance.buttons != "classic") ||
           s.appearance.fontSize < 8 || s.appearance.fontSize > 18)
    error = "Choose Light/Dark, Flat/Classic and a UI font size from 8 to 18.";
  else if (!s.lightReading.valid() || !s.darkReading.valid())
    error = "Reading colors require hue -1..359 and brightness 0..100.";
  else if (s.browserProgram.empty() || s.browserProgram.size() > 4096 ||
           std::any_of(s.browserProgram.begin(), s.browserProgram.end(),
                       [](unsigned char c) { return c < 32 || c == 127; }))
    error = "Choose a browser program name or path (without arguments).";
  else {
    error.clear();
    return true;
  }
  return false;
}
bool PreferencesService::commit(const PreferencesDraft& draft, std::string& error) {
  if (!validate(draft, error))
    return false;
  if (!save(draft, error))
    return false;
  current = draft;
  if (changed)
    changed(current);
  return true;
}
} // namespace xfmd
