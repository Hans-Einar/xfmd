#include "ExportCoordinator.h"
namespace xfmd {
void ExportControl::checkpoint() const {
  if (cancelled.load())
    throw Error(ErrorCode::Io, "PDF export cancelled.");
}
ExportCoordinator::~ExportCoordinator() {
  cancel();
  if (worker.joinable())
    worker.join();
}
bool ExportCoordinator::start(ExportRequest request) {
  if (busy())
    return false;
  request.paper.validate();
  if (request.source.text.size() > maxDocumentBytes)
    throw Error(ErrorCode::TooLarge, "Document exceeds 8 MiB.");
  control = std::make_unique<ExportControl>();
  done = false;
  result = {};
  result.token = request.source.token;
  result.target = request.target;
  worker = std::thread([this, request = std::move(request)] {
    try {
      result.pages = work(request, *control);
      result.success = true;
    } catch (const std::exception& e) {
      result.error = e.what();
      result.cancelled = control->cancelled.load();
    } catch (...) {
      result.error = "Unknown PDF export failure.";
    }
    done.store(true, std::memory_order_release);
  });
  return true;
}
void ExportCoordinator::cancel() {
  if (control) {
    std::lock_guard<std::mutex> lock(control->publication);
    if (!control->published)
      control->cancelled = true;
  }
}
std::pair<unsigned, unsigned> ExportCoordinator::progress() const {
  return control ? std::make_pair(control->page.load(), control->total.load())
                 : std::make_pair(0u, 0u);
}
std::optional<ExportResult> ExportCoordinator::take() {
  if (!busy() || !done.load(std::memory_order_acquire))
    return {};
  worker.join();
  return result;
}
} // namespace xfmd
