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
      currentFrame.reset();
      relayout(width);
    }
  }
  if (worker.busy())
    scheduler.restart(2, 10, [this] { poll(); });
}
void PreviewCoordinator::relayout(double newWidth) {
  width = std::max(40.0, newWidth);
  if (profile.mode == LayoutMode::Paged && currentFrame &&
      currentFrame->token == session.view().token && currentFrame->key.profile == profile)
    return;
  invalidate();
  if (!model || model->token != session.view().token)
    return;
  try {
    requested = {session.view().token, profile, metrics.fontSetId(), ++generation,
                 profile.mode == LayoutMode::Paged ? 0 : width};
    if (layoutRequested)
      layoutRequested(requested);
    acceptFrame(renderer.layout(*model, {width, generation, profile}, metrics));
  } catch (const std::exception& e) {
    if (failed)
      failed(e.what());
  }
}
void PreviewCoordinator::setLayoutProfile(LayoutProfile value) {
  value.paper.validate();
  if (value == profile)
    return;
  profile = value;
  relayout(width);
}
bool PreviewCoordinator::acceptFrame(LayoutResult frame) {
  if (!frame || frame->token != session.view().token || !(frame->key == requested))
    return false;
  currentFrame = frame;
  if (present)
    present(std::move(frame));
  return true;
}
} // namespace xfmd
