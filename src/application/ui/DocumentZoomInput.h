#pragma once
#include <fx.h>
#include <fxkeys.h>
namespace xfmd {
inline int documentZoomKey(const FX::FXEvent& event) {
  if (!(event.state & FX::CONTROLMASK) || (event.state & FX::ALTMASK))
    return 0;
  switch (event.code) {
  case FX::KEY_plus:
  case FX::KEY_equal:
  case FX::KEY_KP_Add:
    return 1;
  case FX::KEY_minus:
  case FX::KEY_KP_Subtract:
    return -1;
  default:
    return 0;
  }
}
} // namespace xfmd
