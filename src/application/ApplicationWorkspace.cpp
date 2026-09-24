#include "Application.h"
#include <filesystem>
namespace xfmd {
void Application::wireWorkspace() {
  auto* workspace = window->workspacePanel;
  auto* field = window->documentPath;
  workspace->openRequested = [this] { execute(CommandRouter::Open); };
  workspace->rootChanged = [field](const auto& root) { field->setWorkPath(root); };
  field->setWorkPath(workspace->history.root().string());
  field->filterChanged = [workspace](const auto& pattern) { workspace->setNameFilter(pattern); };
  field->submitted = [this](const auto& target) { return openTypedPath(target); };
  field->feedback = [this](const auto& text) { window->status->setText(text.c_str()); };
  workspace->indexRefresh = [this, workspace] {
    references->cancel();
    auto model = preview->currentModel();
    if (model && model->token == session.view().token)
      workspace->index->present(DocumentIndex::build(*model, session.view().path),
                                session.view().path);
    else
      preview->refresh();
  };
}
bool Application::openTypedPath(const std::string& value) {
  if (value.empty()) {
    window->status->setText("Enter an existing file or folder; typing alone filters filenames.");
    return false;
  }
  try {
    std::filesystem::path path(value);
    if (path.is_relative())
      path = window->workspacePanel->history.root() / path;
    path = std::filesystem::canonical(path);
    if (std::filesystem::is_directory(path))
      return window->workspacePanel->setWorkPath(path.string());
    return openTarget(path.string());
  } catch (const std::exception& e) {
    window->status->setText((std::string("Cannot open exact path: ") + e.what()).c_str());
    return false;
  }
}
} // namespace xfmd
