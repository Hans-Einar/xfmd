#include "FoxWindowMode.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <algorithm>
#include <limits>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxWindowMode)
modeMap[] = {FXMAPFUNC(SEL_TIMEOUT, FoxWindowMode::ID_RESTORE, FoxWindowMode::settle)};
FXIMPLEMENT(FoxWindowMode, FXObject, modeMap, ARRAYNUMBER(modeMap))
namespace {
bool has(Display* display, Window window, const char* property, Atom wanted) {
  Atom type;
  int format;
  unsigned long count, rest;
  unsigned char* data = nullptr;
  bool found = false;
  if (XGetWindowProperty(display, window, XInternAtom(display, property, False), 0, 256, False,
                         XA_ATOM, &type, &format, &count, &rest, &data) == Success &&
      data && type == XA_ATOM && format == 32) {
    auto* atoms = reinterpret_cast<Atom*>(data);
    found = std::find(atoms, atoms + count, wanted) != atoms + count;
  }
  if (data)
    XFree(data);
  return found;
}
} // namespace
FoxWindowMode::~FoxWindowMode() {
  if (window)
    window->getApp()->removeTimeout(this, ID_RESTORE);
}
bool FoxWindowMode::fullscreen() const {
  if (!window || !window->id())
    return false;
  auto* d = static_cast<Display*>(window->getApp()->getDisplay());
  return has(d, window->id(), "_NET_WM_STATE", XInternAtom(d, "_NET_WM_STATE_FULLSCREEN", False));
}
bool FoxWindowMode::requestFullscreen(bool value) {
  if (!window->id())
    return false;
  auto* d = static_cast<Display*>(window->getApp()->getDisplay());
  Atom state = XInternAtom(d, "_NET_WM_STATE", False),
       full = XInternAtom(d, "_NET_WM_STATE_FULLSCREEN", False);
  if (!has(d, DefaultRootWindow(d), "_NET_SUPPORTED", full))
    return false;
  if (value && !fullscreen() && !pending.value_or(false))
    before = {window->getX(), window->getY(), window->getWidth(), window->getHeight()};
  pending = value;
  XEvent event{};
  event.xclient.type = ClientMessage;
  event.xclient.window = window->id();
  event.xclient.message_type = state;
  event.xclient.format = 32;
  event.xclient.data.l[0] = value ? 1 : 0;
  event.xclient.data.l[1] = full;
  event.xclient.data.l[3] = 1;
  XSendEvent(d, DefaultRootWindow(d), False, SubstructureRedirectMask | SubstructureNotifyMask,
             &event);
  XFlush(d);
  return true;
}
void FoxWindowMode::observe() {
  bool current = fullscreen();
  if (pending && *pending == current)
    pending.reset();
  if (observed && !current)
    window->getApp()->addTimeout(this, ID_RESTORE, 150);
  observed = current;
}
WindowRect FoxWindowMode::ensureVisible(WindowRect rect, const std::vector<WindowRect>& screens) {
  if (screens.empty())
    return rect;
  const WindowRect* best = &screens.front();
  double distance = std::numeric_limits<double>::max();
  for (const auto& screen : screens) {
    int overlapX =
        std::min(rect.x + rect.width, screen.x + screen.width) - std::max(rect.x, screen.x);
    int overlapY = std::min(rect.y + 32, screen.y + screen.height) - std::max(rect.y, screen.y);
    if (overlapX >= std::min(128, rect.width) && overlapY >= 24)
      return rect;
    double dx = double(rect.x) - screen.x, dy = double(rect.y) - screen.y;
    if (dx * dx + dy * dy < distance) {
      distance = dx * dx + dy * dy;
      best = &screen;
    }
  }
  rect.width = std::min(rect.width, best->width);
  rect.height = std::min(rect.height, best->height);
  rect.x = std::clamp(rect.x, best->x, best->x + best->width - rect.width);
  rect.y = std::clamp(rect.y, best->y, best->y + best->height - rect.height);
  return rect;
}
long FoxWindowMode::settle(FXObject*, FXSelector, void*) {
  if (fullscreen())
    return 1;
  auto* d = static_cast<Display*>(window->getApp()->getDisplay());
  int count = 0, major = 1, minor = 5;
  std::vector<WindowRect> screens;
  if (XRRQueryVersion(d, &major, &minor) && (major > 1 || (major == 1 && minor >= 5))) {
    auto* monitors = XRRGetMonitors(d, DefaultRootWindow(d), True, &count);
    for (int i = 0; monitors && i < count; ++i)
      screens.push_back({monitors[i].x, monitors[i].y, monitors[i].width, monitors[i].height});
    if (monitors)
      XRRFreeMonitors(monitors);
  }
  if (screens.empty())
    screens.push_back(
        {0, 0, DisplayWidth(d, DefaultScreen(d)), DisplayHeight(d, DefaultScreen(d))});
  WindowRect current{window->getX(), window->getY(), window->getWidth(), window->getHeight()};
  auto visible = ensureVisible(current, screens);
  if (visible.x != current.x || visible.y != current.y || visible.width != current.width ||
      visible.height != current.height) {
    // EWMH owns the normal/maximized restore. Intervene only if the result lost its title area.
    if (before.width > 0 && !window->isMaximized()) {
      current.width = before.width;
      current.height = before.height;
      visible = ensureVisible(current, screens);
    }
    window->position(visible.x, visible.y, visible.width, visible.height);
  }
  return 1;
}
} // namespace xfmd
