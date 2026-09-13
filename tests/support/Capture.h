#pragma once
#include <cairo-xlib.h>
#include <fx.h>
#include <stdexcept>
inline void captureDesktop(FX::FXApp& app, const char* path) {
  auto* display = static_cast<Display*>(app.getDisplay());
  const int screen = DefaultScreen(display);
  auto* surface = cairo_xlib_surface_create(
      display, RootWindow(display, screen), DefaultVisual(display, screen),
      DisplayWidth(display, screen), DisplayHeight(display, screen));
  auto result = cairo_surface_write_to_png(surface, path);
  cairo_surface_destroy(surface);
  if (result != CAIRO_STATUS_SUCCESS)
    throw std::runtime_error("Screenshot failed");
}
