#include "FoxCairoCanvas.h"
#include "contracts/DocumentTypes.h"
#include <algorithm>
namespace xfmd {
FoxCairoCanvas::~FoxCairoCanvas() {
  if (surface)
    cairo_surface_destroy(surface);
}
cairo_surface_t* FoxCairoCanvas::begin(FX::FXApp& app, int width, int height) {
  width = std::max(1, width);
  height = std::max(1, height);
  if (std::size_t(width) * height > 16 * 1024 * 1024)
    throw Error(ErrorCode::TooLarge, "Preview viewport exceeds 16 million pixels.");
  if (!image || image->getWidth() != width || image->getHeight() != height) {
    if (surface) {
      cairo_surface_destroy(surface);
      surface = nullptr;
    }
    image = std::make_unique<FX::FXImage>(
        &app, nullptr, FX::IMAGE_OWNED | FX::IMAGE_KEEP | FX::IMAGE_SHMI, width, height);
    std::fill_n(image->getData(), std::size_t(width) * height, FXRGB(255, 255, 255));
    image->create();
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  }
  if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
    throw Error(ErrorCode::Io, cairo_status_to_string(cairo_surface_status(surface)));
  return surface;
}
void FoxCairoCanvas::present(FX::FXWindow& window) {
  cairo_surface_flush(surface);
  const auto* bytes = cairo_image_surface_get_data(surface);
  int stride = cairo_image_surface_get_stride(surface);
  auto* target = image->getData();
  for (int y = 0; y < image->getHeight(); ++y) {
    const auto* row = reinterpret_cast<const std::uint32_t*>(bytes + y * stride);
    for (int x = 0; x < image->getWidth(); ++x) {
      auto p = row[x];
      *target++ = FXRGB((p >> 16) & 255, (p >> 8) & 255, p & 255);
    }
  }
  image->render();
  FX::FXDCWindow dc(&window);
  dc.drawImage(image.get(), 0, 0);
}
} // namespace xfmd
