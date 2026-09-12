#include "Application.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
Application::~Application() { preview.reset(); scheduler.reset(); delete window; }
void Application::initialize(int& argc, char** argv) {
  app.init(argc, argv);
  window = new XfmdWindow(&app, commands);
  views = std::make_unique<ViewModeController>(window->editor, window->previewArea, window->sidebar, window->split);
  commands.action = [this](auto command) { execute(command); };
  commands.enabled = [this](auto command) {
    if (command == CommandRouter::Undo) return edits.canUndo();
    if (command == CommandRouter::Redo) return edits.canRedo();
    if (command == CommandRouter::Back || command == CommandRouter::Forward)
      return canNavigate && canNavigate(command == CommandRouter::Back);
    return true;
  };
  wireDocument();
  interpreter = std::make_unique<CmarkInterpreter>();
  renderer = std::make_unique<MarkdownRenderer>();
  metrics = std::make_unique<FoxTextMetrics>(app);
  host = new FoxRenderHost(window->previewArea, *renderer, *metrics);
  scheduler = std::make_unique<FoxScheduler>(app);
  preview = std::make_unique<PreviewCoordinator>(session, *interpreter, *renderer, *metrics, *scheduler);
  preview->invalidated = [this](DocumentToken token) { scrolling.invalidate(token); host->expect(token); };
  preview->present = [this](LayoutResult frame) {
    host->present(frame);
    if (host->interactive()) scrolling.setFrame(std::move(frame));
  };
  preview->failed = [this](const std::string& error) { window->status->setText(("Preview unavailable: " + error).c_str()); };
  documentOpened = [this] { preview->refresh(); };
  contentChanged = [this] { preview->schedule(); };
  host->resized = [this](int width) { preview->relayout(width); };
  scrolling.setEditor = [this](SourceAnchor anchor) { window->editor->setSourceAnchor(anchor); };
  scrolling.setPreview = [this](int y) { host->setViewport(y); };
  window->editor->viewportChanged = [this](std::size_t byte) { scrolling.onViewportChanged(ViewOrigin::Editor, byte, session.view().token); };
  host->viewportChanged = [this](int y) { scrolling.onViewportChanged(ViewOrigin::Preview, y, session.view().token); };
  views->changed = [this] { scrolling.setSplit(views->mode() == ViewMode::Split); host->recalc(); };
  app.create();
  views->setMode(ViewMode::Preview);
  window->show(PLACEMENT_SCREEN);
}
void Application::wireDocument() {
  documents.chooseUnsaved = [this] {
    auto answer = FXMessageBox::question(window, MBOX_SAVE_CANCEL_DONTSAVE, "Unsaved changes", "Save changes before leaving this document?");
    if (answer == MBOX_CLICKED_SAVE) return UnsavedChoice::Save;
    if (answer == MBOX_CLICKED_CANCEL) return UnsavedChoice::Cancel;
    return UnsavedChoice::Discard;
  };
  documents.chooseSavePath = [this] { return savePath(); };
  documents.error = [this](const std::string& message) {
    window->status->setText(message.c_str());
    FXMessageBox::error(window, MBOX_OK, "xfmd", "%s", message.c_str());
  };
  documents.opened = [this] {
    edits.reset(); updateUi();
    window->editor->setCursorPos(0);
    window->editor->setSourceAnchor({0});
    window->sidebar->setDirectory(std::filesystem::path(session.view().path).parent_path().c_str());
    if (documentOpened) documentOpened();
  };
  documents.saved = [this] { updateUi(); if (contentChanged) contentChanged(); };
  edits.changed = [this] { updateUi(); if (contentChanged) contentChanged(); };
  window->editor->edited = [this](const std::string& text) {
    try { edits.applyProjectedText(text); }
    catch (const std::exception& e) { updateUi(); documents.error(e.what()); }
  };
  window->sidebar->open = [this](const std::string& path) { open(path); };
}
void Application::updateUi() {
  window->editor->applyProjection(session.view());
  auto title = (session.dirty() ? "* " : "") + (session.view().path.empty() ? std::string("Untitled") : session.view().path) + " — xfmd";
  window->setTitle(title.c_str());
  window->status->setText((std::to_string(session.view().text.size()) + " bytes" + (session.dirty() ? " — modified" : " — saved")).c_str());
}
}
