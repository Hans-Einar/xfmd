#include "PreviewCoordinator.h"
#include <algorithm>
namespace xfmd {
void PreviewCoordinator::invalidate() { if (invalidated) invalidated(session.view().token); }
void PreviewCoordinator::refresh() {
  invalidate();
  try { model = interpreter.parse(session.snapshot()); relayout(width); }
  catch (const std::exception& e) { if (failed) failed(e.what()); }
}
void PreviewCoordinator::relayout(int newWidth) {
  width = std::max(40, newWidth);
  invalidate();
  if (!model || model->token != session.view().token) return;
  try {
    auto frame = renderer.layout(*model, {width, ++generation}, metrics);
    if (frame->token == session.view().token && present) present(std::move(frame));
  } catch (const std::exception& e) { if (failed) failed(e.what()); }
}
}
