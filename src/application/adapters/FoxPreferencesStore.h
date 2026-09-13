#pragma once
#include "application/preferences/PreferencesService.h"
#include <fx.h>
namespace xfmd {
class FoxPreferencesStore {
  FX::FXRegistry& registry;
  bool readOnly = false;
public:
  explicit FoxPreferencesStore(FX::FXRegistry& value):registry(value){}
  PreferencesSnapshot load();
  bool save(const PreferencesSnapshot&, std::string&);
};
} // namespace xfmd
