#pragma once
#include "contracts/RenderFrame.h"
#include <cairo.h>
#include <string>
namespace xfmd {
// Thread-local native SVG handles; no native types escape into contracts.
class SvgDiagramCache {
public:
  static void validate(const std::string& svg);
  static void paint(cairo_t*, const std::string& svg, Rect, const std::string& stylesheet);
};
} // namespace xfmd
