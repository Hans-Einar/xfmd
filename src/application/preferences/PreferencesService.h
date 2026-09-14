#pragma once
#include "Appearance.h"
#include "application/scroll/ScrollDynamics.h"
#include <functional>
#include <string>
namespace xfmd {
struct PreferencesSnapshot {
  int version = 1;
  ScrollProfile scroll;
  Appearance appearance;
  double marginMm = 20;
  std::string browserProgram = "xdg-open";
};
using PreferencesDraft = PreferencesSnapshot;
class PreferencesService {
  PreferencesSnapshot current;
  std::function<bool(const PreferencesSnapshot&, std::string&)> save;

public:
  PreferencesService(PreferencesSnapshot initial,
                     std::function<bool(const PreferencesSnapshot&, std::string&)> store)
      : current(initial), save(std::move(store)) {}
  const PreferencesSnapshot& active() const { return current; }
  PreferencesDraft begin() const { return current; }
  static bool validate(const PreferencesSnapshot&, std::string&);
  bool commit(const PreferencesDraft&, std::string&);
  void cancel() {} // Draft belongs to the dialog; active state was never modified.
  std::function<void(const PreferencesSnapshot&)> changed;
};
} // namespace xfmd
