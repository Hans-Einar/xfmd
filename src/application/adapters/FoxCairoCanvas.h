#pragma once
#include <cairo.h>
#include <fx.h>
#include <memory>
namespace xfmd {
// Reusable viewport buffer: Cairo draws, FOX owns X11 pixel transfer and lifetime.
class FoxCairoCanvas {
  std::unique_ptr<FX::FXImage> image;
  cairo_surface_t* surface = nullptr;

public:
  FoxCairoCanvas() = default;
  FoxCairoCanvas(const FoxCairoCanvas&) = delete;
  FoxCairoCanvas& operator=(const FoxCairoCanvas&) = delete;
  ~FoxCairoCanvas();
  cairo_surface_t* begin(FX::FXApp&, int width, int height);
  void present(FX::FXWindow&);
};
} // namespace xfmd
