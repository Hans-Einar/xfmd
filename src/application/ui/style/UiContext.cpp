#include "UiContext.h"
#include "application/preferences/PreferencesService.h"
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
using namespace FX;
namespace xfmd {
UiContext::UiContext(FXApp& a, const Appearance& appearance)
    : app(a), current(appearance), icons(a) {
  const char* config = std::getenv("XDG_CONFIG_HOME");
  auto base = config && *config
                  ? std::filesystem::path(config)
                  : std::filesystem::path(FXSystem::getHomeDirectory().text()) / ".config";
  path = (base / "xfmd" / "appearance.ini").string();
  profiles.load(path, startupError);
  refresh();
}
void UiContext::setAppearance(const Appearance& value) {
  PreferencesSnapshot candidate;
  candidate.appearance = value;
  std::string error;
  if (!PreferencesService::validate(candidate, error))
    throw std::invalid_argument(error);
  if (value == current)
    return;
  current = value;
  refresh();
}
bool UiContext::reload(std::string& error) {
  if (!profiles.load(path, error))
    return false;
  startupError.clear();
  refresh();
  return true;
}
void UiContext::refresh() {
  auto* font = app.getNormalFont();
  FXFontDesc descriptor;
  font->getFontDesc(descriptor);
  if (descriptor.size != current.fontSize * 10) {
    bool created = font->id();
    if (created)
      font->destroy();
    descriptor.size = current.fontSize * 10;
    font->setFontDesc(descriptor);
    if (created)
      font->create();
  }
  const auto& p = palette();
  app.setBaseColor(p.surface);
  app.setBackColor(p.field);
  app.setForeColor(p.text);
  app.setBorderColor(p.border);
  app.setHiliteColor(p.panel);
  app.setShadowColor(p.border);
  app.setSelbackColor(p.selected);
  app.setSelforeColor(p.text);
  icons.update(p, metrics().iconSize);
}
} // namespace xfmd
