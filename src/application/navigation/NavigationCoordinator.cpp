#include "NavigationCoordinator.h"
#include "LinkResolver.h"
#include <algorithm>
#include <filesystem>
namespace xfmd {
bool NavigationCoordinator::openTarget(const std::string& path, std::optional<std::size_t> target) {
  if (pending)
    return false;
  pending = Request{scrolling.captureAnchor(), target, {}};
  try {
    bool opened = documents.requestOpen(path);
    pending.reset();
    return opened;
  } catch (...) {
    pending.reset();
    throw;
  }
}
bool NavigationCoordinator::openAt(const std::string& path, SourceAnchor anchor) {
  if (pending)
    return false;
  std::error_code ec;
  auto normalized =
      path.empty() ? std::string{} : std::filesystem::weakly_canonical(path, ec).string();
  if (!ec && normalized == session.view().path) {
    anchor.byte = std::min(anchor.byte, session.view().text.size());
    scrolling.restoreAnchor(anchor);
    return true;
  }
  pending = Request{scrolling.captureAnchor(), {}, anchor};
  try {
    bool opened = documents.requestOpen(path);
    pending.reset();
    return opened;
  } catch (...) {
    pending.reset();
    throw;
  }
}
bool NavigationCoordinator::followLink(const std::string& target) {
  try {
    return openTarget(LinkResolver::resolve(session.view().path, target));
  } catch (const std::exception& e) {
    if (error)
      error(e.what());
    return false;
  }
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
  auto request = pending.value_or(Request{scrolling.captureAnchor(), {}, {}});
  auto restore = request.target ? history.at(*request.target).anchor : request.destination;
  history.commit(session.view().path, request.previous, request.target);
  scrolling.invalidate(session.view().token);
  restore.byte = std::min(restore.byte, session.view().text.size());
  scrolling.restoreAnchor(restore);
}
void NavigationCoordinator::documentSaved() { history.renameCurrent(session.view().path); }
} // namespace xfmd
