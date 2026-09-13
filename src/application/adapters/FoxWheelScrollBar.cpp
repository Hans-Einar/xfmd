#include "FoxWheelScrollBar.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace FX;
namespace xfmd {
namespace {
std::uint64_t nowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}
} // namespace
FoxWheelScrollBar* FoxWheelScrollBar::activeBar = nullptr;
FXDEFMAP(FoxWheelScrollBar)
wheelMap[] = {FXMAPFUNC(SEL_MOUSEWHEEL, 0, FoxWheelScrollBar::onMouseWheel),
              FXMAPFUNC(SEL_TIMEOUT, FoxWheelScrollBar::ID_MOTION, FoxWheelScrollBar::onMotionTick),
              FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, FoxWheelScrollBar::onPress),
              FXMAPFUNC(SEL_MIDDLEBUTTONPRESS, 0, FoxWheelScrollBar::onPress)};
FXIMPLEMENT(FoxWheelScrollBar, FXScrollBar, wheelMap, ARRAYNUMBER(wheelMap))
FoxWheelScrollBar::FoxWheelScrollBar(FXComposite* parent, FXObject* target, FXSelector selector,
                                     FXuint style)
    : FXScrollBar(parent, target, selector, style) {}
FoxWheelScrollBar::~FoxWheelScrollBar() { cancelMotion(); }
FXScrollBar* FoxWheelScrollBar::replace(FXScrollBar* previous) {
  auto* bar =
      new FoxWheelScrollBar(static_cast<FXComposite*>(previous->getParent()), previous->getTarget(),
                            previous->getSelector(), previous->getScrollBarStyle());
  bar->setBarSize(previous->getBarSize());
  bar->setRange(previous->getRange());
  bar->setPage(previous->getPage());
  bar->setLine(previous->getLine());
  bar->setPosition(previous->getPosition());
  delete previous;
  return bar;
}
void FoxWheelScrollBar::configureTree(FXWindow* root, const ScrollProfile& value) {
  if (auto* bar = dynamic_cast<FoxWheelScrollBar*>(root))
    bar->setProfile(value);
  for (auto* child = root->getFirst(); child; child = child->getNext())
    configureTree(child, value);
}
void FoxWheelScrollBar::cancelTree(FXWindow* root) {
  if (auto* bar = dynamic_cast<FoxWheelScrollBar*>(root))
    bar->cancelMotion();
  for (auto* child = root->getFirst(); child; child = child->getNext())
    cancelTree(child);
}
void FoxWheelScrollBar::cancelMotion() {
  getApp()->removeTimeout(this, ID_MOTION);
  getApp()->removeTimeout(this, ID_TIMEWHEEL);
  motion.reset();
  direction = 0;
  if (activeBar == this)
    activeBar = nullptr;
}
long FoxWheelScrollBar::onPress(FXObject* sender, FXSelector sel, void* data) {
  cancelMotion();
  return FXSELTYPE(sel) == SEL_LEFTBUTTONPRESS ? FXScrollBar::onLeftBtnPress(sender, sel, data)
                                               : FXScrollBar::onMiddleBtnPress(sender, sel, data);
}
long FoxWheelScrollBar::onMouseWheel(FXObject*, FXSelector, void* data) {
  const auto& event = *static_cast<FXEvent*>(data);
  if (!isEnabled() || (event.state & (LEFTBUTTONMASK | MIDDLEBUTTONMASK | RIGHTBUTTONMASK)))
    return 0;
  if (std::getenv("XFMD_TRACE_WHEEL"))
    std::fprintf(stderr, "wheel axis=%s code=%d time=%u modifiers=%u source=unknown\n",
                 (getScrollBarStyle() & SCROLLBAR_HORIZONTAL) ? "x" : "y", event.code, event.time,
                 event.state);
  if (!event.code)
    return 1;
  if (activeBar && activeBar != this)
    activeBar->cancelMotion();
  bool pending = getApp()->hasTimeout(this, ID_MOTION);
  if ((pending && (pos != observed || range != observedRange || page != observedPage)) ||
      event.code * direction < 0) {
    cancelMotion();
    pending = false;
  }
  activeBar = this;
  direction = event.code;
  const double unit = (event.state & ALTMASK) ? line
                      : (event.state & CONTROLMASK)
                          ? page
                          : std::min<double>(page, double(line) * getApp()->getWheelLines());
  auto selected = profile;
  if (event.state & (ALTMASK | CONTROLMASK)) {
    selected = {1, false, .5, 3};
    motion.reset();
  }
  const auto now = nowMs();
  destination =
      motion.advance({event.code / 120.0, now,
                      (getScrollBarStyle() & SCROLLBAR_HORIZONTAL) ? ScrollAxis::Horizontal
                                                                   : ScrollAxis::Vertical},
                     selected, unit, pending ? destination : pos, range - page);
  observed = pos;
  observedRange = range;
  observedPage = page;
  getApp()->removeTimeout(this, ID_MOTION);
  getApp()->removeTimeout(this, ID_TIMEWHEEL);
  getApp()->removeTimeout(this, ID_AUTOSCROLL);
  finish = now + 80;
  if (getScrollBarStyle() & SCROLLBAR_WHEELJUMP)
    finish = now;
  if (destination != pos)
    onMotionTick(this, 0, nullptr);
  return 1;
}
long FoxWheelScrollBar::onMotionTick(FXObject*, FXSelector, void*) {
  if (pos != observed || range != observedRange || page != observedPage) {
    cancelMotion();
    return 1;
  }
  const auto now = nowMs();
  const int remaining = destination - pos;
  int next = destination;
  if (now < finish && std::abs(remaining) > 1) {
    const int step = std::max(
        1, int(std::ceil(std::abs(remaining) * std::min(1.0, 8.0 / double(finish - now)))));
    next = pos + (remaining < 0 ? -step : step);
  }
  setPosition(next);
  observed = pos;
  const bool done = pos == destination;
  if (!done)
    getApp()->addTimeout(this, ID_MOTION, 8);
  notifying = true;
  if (target)
    target->tryHandle(this, FXSEL(done ? SEL_COMMAND : SEL_CHANGED, message),
                      reinterpret_cast<void*>(FXival(pos)));
  notifying = false;
  return 1;
}
} // namespace xfmd
