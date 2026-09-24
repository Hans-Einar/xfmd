#pragma once
#include "ThemeProfiles.h"
#include <array>
#include <memory>
namespace xfmd {
enum class UiIcon {
  NoIcon,
  Open,
  Up,
  Save,
  Back,
  Forward,
  Sidebar,
  Refresh,
  Search,
  Theme,
  Pdf,
  Editor,
  Split,
  Preview,
  Count
};
class IconCatalog {
  FX::FXApp& app;
  std::array<std::unique_ptr<FX::FXIcon>, std::size_t(UiIcon::Count) * 2> icons;

public:
  explicit IconCatalog(FX::FXApp& a) : app(a) {}
  void update(const UiPalette&, int size);
  FX::FXIcon* get(UiIcon id, bool enabled = true) const {
    return id == UiIcon::NoIcon ? nullptr : icons[std::size_t(id) * 2 + !enabled].get();
  }
};
} // namespace xfmd
