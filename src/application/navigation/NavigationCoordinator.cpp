#include "NavigationCoordinator.h"
#include "LinkResolver.h"
namespace xfmd {
bool NavigationCoordinator::openTarget(const std::string& path, std::optional<std::size_t> target) {
  if (pending) return false;
  pending = Request{scrolling.captureAnchor(), target};
  try {
    bool opened = documents.requestOpen(path);
    pending.reset(); return opened;
  } catch (...) { pending.reset(); throw; }
}
bool NavigationCoordinator::followLink(const std::string& target) {
  try { return openTarget(LinkResolver::resolve(session.view().path, target)); }
  catch (const std::exception& e) { if (error) error(e.what()); return false; }
}
bool NavigationCoordinator::goBack() {
  auto target = history.propose(true);
  return target && openTarget(history.at(*target).path, target);
}
bool NavigationCoordinator::goForward() {
  auto target = history.propose(false);
  return target && openTarget(history.at(*target).path, target);
}
void NavigationCoordinator::commitVisit() {
  auto request = pending.value_or(Request{scrolling.captureAnchor(), {}});
  auto restore = request.target ? history.at(*request.target).anchor : SourceAnchor{};
  history.commit(session.view().path, request.previous, request.target);
  scrolling.invalidate(session.view().token);
  scrolling.restoreAnchor(restore);
}
void NavigationCoordinator::documentSaved() { history.renameCurrent(session.view().path); }
}
