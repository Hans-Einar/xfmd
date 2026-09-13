#include "FoxPreferencesStore.h"
#include <cmath>
namespace xfmd {
PreferencesSnapshot FoxPreferencesStore::load() {
  PreferencesSnapshot s;
  auto read = [&](const char* section, const char* key, double fallback, double min, double max) {
    const char* value = registry.readStringEntry(section, key, nullptr);
    if (!value)
      return fallback;
    try {
      std::size_t count = 0;
      std::string text(value);
      double result = std::stod(text, &count);
      if (count == text.size() && std::isfinite(result) && result >= min && result <= max)
        return result;
    } catch (...) {
    }
    return fallback;
  };
  const auto version = read("Preferences", "version",
                            registry.existingEntry("Preferences", "version") ? -1 : 1, -1, 1000000);
  readOnly = version != 1;
  s.scroll.speed = read("Scroll", "speed", 1.5, .25, 4);
  s.scroll.strength = read("Scroll", "strength", .5, 0, 2);
  s.scroll.maxGain = read("Scroll", "maxGain", 3, 1, 5);
  s.scroll.acceleration = registry.readBoolEntry("Scroll", "acceleration", false);
  s.marginMm = read("Page", "marginMm", 20, 5, 50);
  s.browserProgram = registry.readStringEntry("Programs", "browser", "xdg-open");
  std::string error;
  if (!PreferencesService::validate(s, error))
    s.browserProgram = "xdg-open";
  return s;
}
bool FoxPreferencesStore::save(const PreferencesSnapshot& s, std::string& error) {
  if (readOnly) {
    error = "Preferences were written by a newer version; keeping them unchanged.";
    return false;
  }
  if (!PreferencesService::validate(s, error))
    return false;
  FX::FXSettings before(registry);
  registry.writeIntEntry("Preferences", "version", s.version);
  registry.writeRealEntry("Scroll", "speed", s.scroll.speed);
  registry.writeBoolEntry("Scroll", "acceleration", s.scroll.acceleration);
  registry.writeRealEntry("Scroll", "strength", s.scroll.strength);
  registry.writeRealEntry("Scroll", "maxGain", s.scroll.maxGain);
  registry.writeRealEntry("Page", "marginMm", s.marginMm);
  registry.writeStringEntry("Programs", "browser", s.browserProgram.c_str());
  if (!registry.write()) {
    static_cast<FX::FXSettings&>(registry) = before;
    error = "Could not save preferences. Your previous settings are still active.";
    return false;
  }
  return true;
}
} // namespace xfmd
