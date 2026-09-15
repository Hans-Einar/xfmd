#pragma once
#include "CairoVisual.h"
#include <string>
namespace xfmd {
class ImageDecoder {
public:
  static std::shared_ptr<const VisualResource> load(const std::string&);
};
} // namespace xfmd
