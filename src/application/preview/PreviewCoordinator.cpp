#include "PreviewCoordinator.h"
#include <algorithm>
namespace xfmd {
void PreviewCoordinator::invalidate() {
  if (invalidated)
    invalidated(session.view().token);
}
PreviewCoordinator::~PreviewCoordinator() {
  scheduler.cancel(1);
  scheduler.cancel(2);
}
void PreviewCoordinator::schedule() {
  invalidate();
  scheduler.restart(1, 300, [this] { refresh(); });
}
void PreviewCoordinator::refresh() {
  scheduler.cancel(1);
  invalidate();
  worker.submit(session.snapshot());
  scheduler.restart(2, 10, [this] { poll(); });
}
void PreviewCoordinator::poll() {
  auto completion = worker.take();
  if (completion && completion->token == session.view().token) {
    if (!completion->error.empty()) {
      if (failed)
        failed(completion->error);
    } else {
      model = std::move(completion->model);
      relayout(width);
    }
  }
  if (worker.busy())
    scheduler.restart(2, 10, [this] { poll(); });
}
void PreviewCoordinator::relayout(int newWidth) {
  width = std::max(40, newWidth);
  invalidate();
  if (!model || model->token != session.view().token)
    return;
  try {
    auto frame = renderer.layout(*model, {width, ++generation}, metrics);
    if (frame->token == session.view().token && present)
      present(std::move(frame));
  } catch (const std::exception& e) {
    if (failed)
      failed(e.what());
  }
}
} // namespace xfmd
