#include "IconResources.h"
#include "IconData.h"
namespace xfmd {
void IconResources::load(FX::FXApp& app) {
  if (!FX::FXPNGIcon::supported)
    return;
  large = std::make_unique<FX::FXPNGIcon>(&app, icon64, 0, FX::IMAGE_KEEP);
  small = std::make_unique<FX::FXPNGIcon>(&app, icon16, 0, FX::IMAGE_KEEP);
  if (large->getWidth() != 64 || small->getWidth() != 16) {
    large.reset();
    small.reset();
  }
}
} // namespace xfmd
