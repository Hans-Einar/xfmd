#pragma once
#include <fx.h>
// FOX image codecs require the base toolkit declarations first.
#include <FXPNGIcon.h>
#include <memory>
namespace xfmd {
class IconResources {
public:
  std::unique_ptr<FX::FXPNGIcon> large, small;
  void load(FX::FXApp&);
};
} // namespace xfmd
