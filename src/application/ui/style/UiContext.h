#pragma once
#include "IconCatalog.h"
#include "application/ui/controls/ButtonPainter.h"
namespace xfmd {
class UiContext {
  FX::FXApp& app;
  Appearance current;
  ThemeProfiles profiles;
  std::string path, startupError;

public:
  IconCatalog icons;
  UiContext(FX::FXApp&, const Appearance&);
  const Appearance& appearance() const { return current; }
  const UiPalette& palette() const { return profiles.palette(current); }
  UiMetrics metrics() const { return profiles.metrics(current); }
  const ButtonPainter& painter() const {
    return current.buttons == "classic" ? classicButtonPainter() : flatButtonPainter();
  }
  const std::string& profilePath() const { return path; }
  const std::string& error() const { return startupError; }
  void setAppearance(const Appearance&);
  bool reload(std::string&);
  void apply(FX::FXWindow*);
  void refresh();
};
} // namespace xfmd
