#include "DocumentZoom.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include <algorithm>
#include <cmath>
#include <limits>
namespace xfmd {
void DocumentZoom::setPercent(double value) {
  if (!std::isfinite(value))
    return;
  selected = ZoomMode::Manual;
  applied = std::clamp(value, 25.0, 300.0);
  refresh();
}
void DocumentZoom::step(double steps) {
  if (std::isfinite(steps) && steps != 0)
    setPercent(applied + 10 * steps);
}
void DocumentZoom::setMode(ZoomMode mode) {
  if (mode != ZoomMode::Manual && preview.layoutProfile().mode != LayoutMode::Paged)
    return;
  selected = mode;
  refresh();
}
void DocumentZoom::refresh() {
  if (updating)
    return;
  updating = true;
  const auto& profile = preview.layoutProfile();
  if (profile.mode != LayoutMode::Paged && selected != ZoomMode::Manual) {
    selected = ZoomMode::Manual;
    applied = std::clamp(applied, 25.0, 300.0);
  }
  if (selected != ZoomMode::Manual) {
    double available = std::numeric_limits<double>::max();
    auto include = [&](auto& area) {
      double dimension = selected == ZoomMode::FitWidth ? area.contentViewportWidth()
                                                        : area.contentViewportHeight();
      if (dimension > 32)
        available = std::min(available, dimension - 32);
    };
    if (views.mode() != ViewMode::Preview)
      include(editor);
    if (views.mode() != ViewMode::Editor)
      include(host);
    if (available != std::numeric_limits<double>::max()) {
      const double paper =
          selected == ZoomMode::FitWidth ? profile.paper.width : profile.paper.height;
      applied = std::clamp(100 * available / (paper * host.screenScale()), 5.0, 800.0);
    }
  }
  const auto anchor = scrolling.captureAnchor();
  const bool scaleChanged = std::abs(host.viewScale() - applied / 100) > .000001;
  if (scaleChanged) {
    FoxWheelScrollBar::cancelTree(&editor);
    FoxWheelScrollBar::cancelTree(&host);
  }
  editor.setViewProfile(profile, applied / 100);
  host.setViewScale(applied / 100);
  if (scaleChanged)
    scrolling.restoreAnchor(anchor);
  if (changed)
    changed(applied, selected);
  updating = false;
}
} // namespace xfmd
