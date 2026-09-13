#include "FoxWheelScrollBar.h"
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxWheelScrollBar)
wheelMap[] = {FXMAPFUNC(SEL_MOUSEWHEEL, 0, FoxWheelScrollBar::onMouseWheel)};
FXIMPLEMENT(FoxWheelScrollBar, FXScrollBar, wheelMap, ARRAYNUMBER(wheelMap))
FoxWheelScrollBar::FoxWheelScrollBar(FXComposite* parent, FXObject* target, FXSelector selector,
                                     FXuint style)
    : FXScrollBar(parent, target, selector, style) {}
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
void FoxWheelScrollBar::configureTree(FXWindow* root,const ScrollProfile& value) {
  if(auto* bar=dynamic_cast<FoxWheelScrollBar*>(root))bar->setProfile(value);
  for(auto* child=root->getFirst();child;child=child->getNext())configureTree(child,value);
}
long FoxWheelScrollBar::onMouseWheel(FXObject*, FXSelector, void* data) {
  const auto& event = *static_cast<FXEvent*>(data);
  if (!isEnabled() || (event.state & (LEFTBUTTONMASK | MIDDLEBUTTONMASK | RIGHTBUTTONMASK)))
    return 0;
  if (std::getenv("XFMD_TRACE_WHEEL"))
    std::fprintf(stderr, "wheel axis=%s code=%d time=%u modifiers=%u source=unknown\n",
                 (getScrollBarStyle() & SCROLLBAR_HORIZONTAL) ? "x" : "y",
                 event.code, event.time, event.state);
  if (!event.code)
    return 1;
  // Preserve FOX's Alt=line, Ctrl=page and configured wheel-lines behavior.
  std::int64_t unit =
      (event.state & ALTMASK) ? line
      : (event.state & CONTROLMASK)
          ? page
          : std::min<std::int64_t>(page, std::int64_t(line) * getApp()->getWheelLines());
  const int base = getApp()->hasTimeout(this, ID_TIMEWHEEL) ? dragpoint : pos;
  getApp()->removeTimeout(this, ID_TIMEWHEEL);
  getApp()->removeTimeout(this, ID_AUTOSCROLL);
  const double scale=(event.state & (ALTMASK|CONTROLMASK)) ? 1.0 : profile.speed;
  dragpoint = motion.advance(event.code / 120.0, unit * scale, base, range - page);
  if (dragpoint == pos) {
    dragpoint = 0;
    return 1;
  }
  // Reuse FOX's bounded animation and standard changed/command notifications.
  // Every timer step is nonzero and the inherited timer clamps its final step.
  int distance = dragpoint - pos;
  int step = distance;
  if (!(getScrollBarStyle() & SCROLLBAR_WHEELJUMP) && (distance > 16 || distance < -16))
    step = distance / 16;
  if (getScrollBarStyle() & SCROLLBAR_WHEELJUMP)
    return onTimeWheel(this, FXSEL(SEL_TIMEOUT, ID_TIMEWHEEL),
                       reinterpret_cast<void*>(FXival(step)));
  getApp()->addTimeout(this, ID_TIMEWHEEL, 5, reinterpret_cast<void*>(FXival(step)));
  return 1;
}
} // namespace xfmd
