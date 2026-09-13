#include "Application.h"
#include "interpreter/CmarkInterpreter.h"
#include <algorithm>
namespace xfmd {
namespace {
SourceAnchor referenceAnchor(const IndexAction& wanted, const SemanticDocument& model) {
  auto headings = DocumentIndex::topLevel(model);
  std::size_t occurrence = 0;
  auto found = std::find_if(headings.begin(), headings.end(), [&](const auto& heading) {
    return heading.title == wanted.heading && occurrence++ == wanted.occurrence;
  });
  return found == headings.end() ? SourceAnchor{} : found->anchor;
}
} // namespace
void Application::wireIndex() {
  referenceInterpreter = std::make_unique<CmarkInterpreter>();
  references = std::make_unique<ReferenceWorker>(*referenceInterpreter, files);
  auto* panel = window->workspacePanel->index;
  panel->activated = [this](const IndexAction& action) { activateIndex(action); };
  panel->referenceRequested = [this, panel](const std::string& path) {
    auto model = preview->currentModel();
    if (path == session.view().path && model && model->token == session.view().token) {
      panel->presentReference({path, {}, DocumentIndex::topLevel(*model)});
      return;
    }
    if (!references->submit(path))
      panel->presentReference({path, "Busy — collapse and expand to retry.", {}});
    else
      scheduler->restart(4, 20, [this] { pollReferences(); });
  };
  preview->modelReady = [this, panel](ParseResult model) {
    references->cancel();
    panel->present(DocumentIndex::build(*model, session.view().path), session.view().path);
    if (pendingHeading) {
      auto wanted = std::move(*pendingHeading);
      pendingHeading.reset();
      auto anchor = referenceAnchor(wanted, *model);
      scrolling.restoreAnchor(anchor);
      window->editor->setSourceAnchor(anchor);
    }
  };
}
void Application::pollReferences() {
  while (auto result = references->take())
    window->workspacePanel->index->presentReference(*result);
  if (references->busy())
    scheduler->restart(4, 20, [this] { pollReferences(); });
}
void Application::pollBrowser() {
  if (browser.poll())
    scheduler->restart(5, 250, [this] { pollBrowser(); });
}
void Application::openBrowser(const std::string& target) {
  try {
    browser.open(target, preferences->active().browserProgram);
    scheduler->restart(5, 250, [this] { pollBrowser(); });
  } catch (const std::exception& e) {
    documents.error(e.what());
  }
}
void Application::activateIndex(const IndexAction& action) {
  if (!action.error.empty()) {
    documents.error(action.error);
    return;
  }
  if (action.kind == IndexActionKind::Hyperlink && ExternalBrowser::accepts(action.target)) {
    openBrowser(action.target);
    return;
  }
  const auto token = session.view().token;
  const auto path = action.kind == IndexActionKind::Hyperlink ? session.view().path : action.path;
  if (!navigation->openAt(path, action.anchor))
    return;
  if (!action.heading.empty()) {
    if (session.view().token != token)
      pendingHeading = action;
    else if (auto model = preview->currentModel(); model && model->token == token) {
      const auto anchor = referenceAnchor(action, *model);
      scrolling.restoreAnchor(anchor);
      window->editor->setSourceAnchor(anchor);
      return;
    }
  }
  // Editor-only mode must navigate even when synchronized scrolling is disabled.
  window->editor->setSourceAnchor({std::min(action.anchor.byte, session.view().text.size())});
}
} // namespace xfmd
