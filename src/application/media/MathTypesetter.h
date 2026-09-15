#pragma once
#include "CairoVisual.h"
#include <string>
namespace xfmd {
class MathTypesetter {
public:
  static std::shared_ptr<const VisualResource> render(const std::string&, bool display);
};
} // namespace xfmd
