#pragma once
#include <fx.h>
#include <fxkeys.h>
namespace xfmd {
inline bool activatesTreeItem(const FX::FXEvent& event, bool leaf) {
  if (event.state & (FX::CONTROLMASK | FX::ALTMASK | FX::SHIFTMASK | FX::METAMASK))
    return false;
  return event.code == FX::KEY_Return || event.code == FX::KEY_KP_Enter ||
         event.code == FX::KEY_space ||
         (leaf && (event.code == FX::KEY_Right || event.code == FX::KEY_KP_Right));
}
} // namespace xfmd
