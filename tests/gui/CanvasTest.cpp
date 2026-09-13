#include "application/adapters/FoxCairoCanvas.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cmath>
void test() {
  int argc = 1;
  char name[] = "canvas-test";
  char* argv[] = {name, nullptr};
  FX::FXApp app("canvas-test", "xfmd");
  app.init(argc, argv);
  FX::FXMainWindow window(&app, "Canvas", nullptr, nullptr, FX::DECOR_ALL, 0, 0, 200, 150);
  app.create();
  window.show();
  app.runWhileEvents();
  xfmd::FoxCairoCanvas canvas;
  auto* display = static_cast<Display*>(app.getDisplay());
  for (int width : {80, 120, 80}) {
    auto* surface = canvas.begin(app, width, 100);
    auto* cr = cairo_create(surface);
    cairo_set_source_rgb(cr, .2, .4, .8);
    cairo_paint(cr);
    cairo_destroy(cr);
    canvas.present(window);
    auto* image = XGetImage(display, window.id(), 10, 10, 1, 1, AllPlanes, ZPixmap);
    CHECK(image);
    auto pixel = XGetPixel(image, 0, 0);
    auto channel = [&](unsigned long mask) { return double(pixel & mask) * 255 / mask; };
    CHECK(std::abs(channel(image->red_mask) - 51) < 1);
    CHECK(std::abs(channel(image->green_mask) - 102) < 1);
    CHECK(std::abs(channel(image->blue_mask) - 204) < 1);
    XDestroyImage(image);
  }
}
TEST_MAIN(test)
