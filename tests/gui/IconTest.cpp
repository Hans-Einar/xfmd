#include "application/Application.h"
#include "support/Capture.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
void test() {
  int argc = 1;
  char name[] = "icon-test";
  char* argv[] = {name, nullptr};
  xfmd::Application app;
  app.initialize(argc, argv);
  CHECK(app.icons.large && app.icons.small);
  CHECK(app.icons.large->getWidth() == 64 && app.icons.small->getWidth() == 16);
  auto* d = static_cast<Display*>(app.app.getDisplay());
  auto* hints = XGetWMHints(d, app.window->id());
  CHECK(hints && (hints->flags & IconPixmapHint));
  Window root;
  int x, y;
  unsigned w, h, border, depth;
  CHECK(XGetGeometry(d, hints->icon_pixmap, &root, &x, &y, &w, &h, &border, &depth));
  CHECK(w == 64 && h == 64);
  XFree(hints);
  app.app.runWhileEvents();
  if (const char* path = std::getenv("XFMD_CAPTURE_FILE"))
    captureDesktop(app.app, path);
}
TEST_MAIN(test)
