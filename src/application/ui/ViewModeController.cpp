#include "ViewModeController.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
void ViewModeController::setMode(ViewMode mode) {
  const bool wasSplit = current == ViewMode::Split;
  const int available = std::max(0, splitter->getWidth() - splitter->getBarSize());
  const int left = editor->getWidth(), right = preview->getWidth();
  const bool usable = wasSplit && editor->shown() && preview->shown() && left >= 32 &&
                      right >= 32 && std::abs(left + right - available) <= 2;
  if (usable)
    splitFraction = double(left) / (left + right);
  if (mode == ViewMode::Split && (!wasSplit || !usable) && available > 1) {
    // FOX stretches the only visible pane to full width. Restore the two-pane
    // geometry explicitly instead of carrying that width back into Split.
    const int size = std::clamp(int(std::lround(available * splitFraction)), 1, available - 1);
    editor->setWidth(size);
    preview->setWidth(available - size);
  }
  current = mode;
  if (mode == ViewMode::Preview)
    editor->hide();
  else
    editor->show();
  if (mode == ViewMode::Editor)
    preview->hide();
  else
    preview->show();
  splitter->recalc();
  if (mode != ViewMode::Preview)
    editor->setFocus();
  if (changed)
    changed();
}
void ViewModeController::toggleSidebar() {
  if (sidebar->shown())
    sidebar->hide();
  else
    sidebar->show();
  sidebar->getParent()->recalc();
}
} // namespace xfmd
