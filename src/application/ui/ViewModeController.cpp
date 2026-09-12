#include "ViewModeController.h"
namespace xfmd {
void ViewModeController::setMode(ViewMode mode) {
  current = mode;
  if (mode == ViewMode::Preview) editor->hide(); else editor->show();
  if (mode == ViewMode::Editor) preview->hide(); else preview->show();
  splitter->recalc();
  if (mode != ViewMode::Preview) editor->setFocus();
  if (changed) changed();
}
void ViewModeController::toggleSidebar() {
  if (sidebar->shown()) sidebar->hide(); else sidebar->show();
  sidebar->getParent()->recalc();
}
}
