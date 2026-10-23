#include "Application.h"
#include "io/FileOpenPolicy.h"
#include "navigation/LinkResolver.h"
#include <filesystem>
namespace xfmd {
bool Application::openTarget(const std::string& path, bool systemDefault) {
  try {
    const auto kind = systemDefault ? FileOpenKind::Desktop : FileOpenPolicy::classify(path);
    if (kind == FileOpenKind::Desktop) {
      desktopFiles.open(path);
      scheduler->restart(6, 100, [this] { pollDesktopFiles(); });
      return true;
    }
    if (kind == FileOpenKind::Browser) {
      browser.openFile(path, preferences->active().browserProgram);
      scheduler->restart(5, 100, [this] { pollBrowser(); });
      return true;
    }
    if (!open(path))
      return false;
    if (kind == FileOpenKind::Text)
      views->setMode(ViewMode::Editor);
    return true;
  } catch (const std::exception& e) {
    documents.error(e.what());
    return false;
  }
}
bool Application::openDialogPath(const std::string& path) {
  try {
    auto resolved = std::filesystem::canonical(path);
    if (std::filesystem::is_directory(resolved))
      return window->workspacePanel->setWorkPath(resolved.string());
    const auto kind = FileOpenPolicy::classify(resolved.string());
    if (kind == FileOpenKind::Browser || kind == FileOpenKind::Desktop)
      return openTarget(resolved.string());
    // Validate the proposed root without changing the live history or tree.
    auto candidate = window->workspacePanel->history;
    candidate.activate(resolved.parent_path());
    if (!openTarget(resolved.string()))
      return false;
    return window->workspacePanel->setWorkPath(resolved.parent_path().string());
  } catch (const std::exception& e) {
    documents.error(e.what());
    return false;
  }
}
void Application::followLink(const std::string& document, const std::string& target,
                             bool systemDefault) {
  if (!systemDefault && sdpNavigation && sdpNavigation->follow(document, target))
    return;
  if (!systemDefault && documentViews && target.rfind("sdl-view:", 0) == 0) {
    documentViews->follow(target);
    return;
  }
  if (ExternalBrowser::accepts(target)) {
    openBrowser(target, systemDefault);
    return;
  }
  try {
    const auto link = LinkResolver::documentLink(document, target);
    if (!link.fragment || systemDefault) {
      openTarget(link.path, systemDefault);
      return;
    }
    // Same-buffer links must not reload or discard unsaved edits (including untitled files).
    std::error_code ec;
    const auto path = link.path.empty() ? std::string{}
                                        : std::filesystem::weakly_canonical(link.path, ec).string();
    if (ec)
      throw std::filesystem::filesystem_error("Resolve link", link.path, ec);
    if (path != session.view().path && !openTarget(path))
      return;
    if (path != session.view().path)
      return; // Browser/desktop routing did not open an internal document.
    pendingHeading.reset();
    pendingLinkAnchor = PendingLinkAnchor{session.view().token, *link.fragment};
    if (auto model = preview->currentModel(); model && model->token == session.view().token)
      applyLinkAnchor(*model);
  } catch (const std::exception& e) {
    documents.error(e.what());
  }
}
void Application::pollDesktopFiles() {
  for (const auto& error : desktopFiles.poll())
    documents.error(error);
  if (desktopFiles.busy())
    scheduler->restart(6, 100, [this] { pollDesktopFiles(); });
}
} // namespace xfmd
