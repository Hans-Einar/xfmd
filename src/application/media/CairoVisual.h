#pragma once
#include "contracts/VisualResource.h"
#include <cairo.h>
namespace xfmd {
struct CairoVisual final : VisualResource {
  cairo_surface_t* surface;
  bool monochrome;
  CairoVisual(cairo_surface_t* value, double w, double h, double baseline, bool mask)
      : surface(value), monochrome(mask) {
    width = w;
    height = h;
    ascent = baseline;
  }
  ~CairoVisual() override { cairo_surface_destroy(surface); }
  CairoVisual(const CairoVisual&) = delete;
  CairoVisual& operator=(const CairoVisual&) = delete;
};
} // namespace xfmd
