#include "Application.h"
#include "adapters/FoxWheelScrollBar.h"
#include "build/BuildVersion.h"
#include "composition/DiagramServices.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
Application::~Application() {
  documentViews.reset();
  windowMode.reset();
  exporter.reset();
  references.reset();
  preview.reset();
  scheduler.reset();
  delete window;
}
void Application::initialize(int& argc, char** argv) {
  app.init(argc, argv);
  preferencesStore = std::make_unique<FoxPreferencesStore>(app.reg());
  preferences = std::make_unique<PreferencesService>(preferencesStore->load(),
                                                     [this](const auto& value, std::string& error) {
                                                       return preferencesStore->save(value, error);
                                                     });
  icons.load(app);
  ui = std::make_unique<UiContext>(app, preferences->active().appearance);
  window = new XfmdWindow(&app, commands, *ui);
  new FXToolTip(&app);
  window->setApplicationIcons(icons.large.get(), icons.small.get());
  windowMode = std::make_unique<FoxWindowMode>(*window);
  window->configured = [this] { windowMode->observe(); };
  editorFont = std::make_unique<FXFont>(&app, "DejaVu Sans Mono", 11);
  window->editor->setFont(editorFont.get());
  views = std::make_unique<ViewModeController>(window->editor, window->previewArea,
                                               window->workspacePanel, window->split);
  commands.action = [this](auto command) { execute(command); };
  commands.enabled = [this](auto command) {
    if (command == CommandRouter::ExportPdf)
      return !exporter || !exporter->busy();
    if (command == CommandRouter::CancelExport)
      return exporter && exporter->busy();
    if (command == CommandRouter::FitWidth || command == CommandRouter::ActualSize)
      return preview && preview->layoutProfile().mode == LayoutMode::Paged;
    if (command == CommandRouter::Undo)
      return edits.canUndo();
    if (command == CommandRouter::Redo)
      return edits.canRedo();
    if (command == CommandRouter::Back || command == CommandRouter::Forward)
      return canNavigate && canNavigate(command == CommandRouter::Back);
    return true;
  };
  commands.checked = [this](auto command) {
    if (command == CommandRouter::ToggleTheme)
      return ui && ui->appearance().theme == "dark";
    if (command == CommandRouter::Preview)
      return views && views->mode() == ViewMode::Preview;
    if (command == CommandRouter::Editor)
      return views && views->mode() == ViewMode::Editor;
    if (command == CommandRouter::Split)
      return views && views->mode() == ViewMode::Split;
    if (command == CommandRouter::Sidebar)
      return window && window->workspacePanel->shown();
    if (command == CommandRouter::FullScreen)
      return windowMode && windowMode->fullscreen();
    if (!preview || !host)
      return false;
    if (command == CommandRouter::A4)
      return preview->layoutProfile().mode == LayoutMode::Paged;
    if (command == CommandRouter::WindowWrap)
      return preview->layoutProfile().mode == LayoutMode::Continuous;
    if (command == CommandRouter::FitWidth)
      return host->fitWidth();
    if (command == CommandRouter::ActualSize)
      return !host->fitWidth();
    return false;
  };
  wireDocument();
  interpreter = DiagramServices::interpreter();
  renderer = std::make_unique<MarkdownRenderer>();
  metrics = std::make_unique<SharedTextMetrics>();
  host = new FoxRenderHost(window->previewArea, *renderer, *metrics);
  window->previewColors->changed = [this](const ReadingColors& colors, bool commit) {
    changeReadingColors(colors, commit);
  };
  scheduler = std::make_unique<FoxScheduler>(app);
  preview = std::make_unique<PreviewCoordinator>(session, *interpreter, *renderer, *metrics,
                                                 *scheduler, DiagramServices::preview());
  preview->invalidated = [this](DocumentToken token) {
    window->workspacePanel->index->invalidate(token, session.view().path);
    scrolling.invalidate(token);
    host->expect(token);
    window->status->setText("Updating preview…");
  };
  preview->layoutRequested = [this](FrameKey key) {
    host->expectLayout(key);
    scrolling.expectLayout(key);
  };
  preview->present = [this](LayoutResult frame) {
    host->present(frame);
    if (host->interactive()) {
      scrolling.setFrame(std::move(frame));
      updateUi();
    }
  };
  preview->failed = [this](const std::string& error) {
    window->status->setText(("Preview unavailable: " + error).c_str());
  };
  navigation = std::make_unique<NavigationCoordinator>(documents, session, scrolling);
  navigation->error = [this](const std::string& message) { documents.error(message); };
  back = [this] { navigation->goBack(); };
  forward = [this] { navigation->goForward(); };
  canNavigate = [this](bool back) { return navigation->history.propose(back).has_value(); };
  host->linkActivated = [this](const std::string& target, bool systemDefault) {
    followLink(session.view().path, target, systemDefault);
  };
  browser.failed = [this](const std::string& error) { documents.error(error); };
  host->linkHovered = [this](const std::string& target) { showLinkTarget(target); };
  wireIndex();
  documentOpened = [this] {
    if (documentViews)
      documentViews->documentChanged("main");
    references->cancel();
    pendingHeading.reset();
    navigation->commitVisit();
    preview->refresh();
  };
  contentChanged = [this] {
    pendingHeading.reset();
    references->cancel();
    preview->schedule();
  };
  host->resized = [this](double width) { preview->relayout(width); };
  scrolling.setEditor = [this](SourceAnchor anchor) { window->editor->setSourceAnchor(anchor); };
  scrolling.setPreview = [this](double y) { host->setViewport(y, ScrollOrigin::Sync); };
  window->editor->viewportChanged = [this](std::size_t byte) {
    scrolling.onViewportChanged(ViewOrigin::Editor, byte, session.view().token, 0, false,
                                window->editor->lastScrollOrigin);
  };
  host->viewportChanged = [this](double y) {
    scrolling.onViewportChanged(ViewOrigin::Preview, y, session.view().token, 0, false,
                                host->lastScrollOrigin);
  };
  views->changed = [this] {
    scrolling.setSplit(views->mode() == ViewMode::Split);
    if (views->mode() == ViewMode::Preview)
      host->setFocus();
    host->recalc();
  };
  preferences->changed = [this](const auto& value) {
    applyAppearance(value.appearance);
    FoxWheelScrollBar::configureTree(window, value.scroll);
    auto profile = preview->layoutProfile();
    profile.paper.margin = value.marginMm * 72 / 25.4;
    preview->setLayoutProfile(profile);
    window->editor->setViewProfile(profile, host->fitWidth());
  };
  preferences->changed(preferences->active());
  app.create();
  window->workspacePanel->setWorkPath(FXSystem::getHomeDirectory().text());
  views->setMode(ViewMode::Preview);
  window->show(PLACEMENT_SCREEN);
  preview->refresh();
}
void Application::wireDocument() {
  documents.chooseUnsaved = [this] {
    auto answer = FXMessageBox::question(window, MBOX_SAVE_CANCEL_DONTSAVE, "Unsaved changes",
                                         "Save changes before leaving this document?");
    if (answer == MBOX_CLICKED_SAVE)
      return UnsavedChoice::Save;
    if (answer == MBOX_CLICKED_CANCEL)
      return UnsavedChoice::Cancel;
    return UnsavedChoice::Discard;
  };
  documents.chooseSavePath = [this] { return savePath(); };
  documents.error = [this](const std::string& message) {
    window->status->setText(message.c_str());
    FXMessageBox::error(window, MBOX_OK, "xfmd", "%s", message.c_str());
  };
  documents.opened = [this] {
    window->workspacePanel->recentFiles->remember(session.view().path);
    edits.reset();
    updateUi();
    window->editor->setCursorPos(0);
    window->editor->setSourceAnchor({0});
    if (documentOpened)
      documentOpened();
  };
  documents.saved = [this] {
    window->workspacePanel->recentFiles->remember(session.view().path);
    if (navigation)
      navigation->documentSaved();
    updateUi();
    if (contentChanged)
      contentChanged();
  };
  edits.changed = [this] {
    updateUi();
    if (contentChanged)
      contentChanged();
  };
  window->editor->edited = [this](const std::string& text) {
    try {
      edits.applyProjectedText(text);
    } catch (const std::exception& e) {
      updateUi();
      documents.error(e.what());
    }
  };
  window->workspacePanel->recentFiles->open = [this](const std::string& path) { openTarget(path); };
  window->sidebar->open = [this](const std::string& path, bool systemDefault) {
    openTarget(path, systemDefault);
  };
}
bool Application::open(const std::string& path) {
  return navigation ? navigation->openTarget(path) : documents.requestOpen(path);
}
void Application::updateUi() {
  window->setDocumentLabel(session.view().path, session.dirty());
  if (preview && host)
    window->previewControls->sync(preview->layoutProfile().mode == LayoutMode::Paged,
                                  host->fitWidth());
  window->editor->applyProjection(session.view());
  auto title = (session.dirty() ? "* " : "") +
               (session.view().path.empty() ? std::string("Untitled") : session.view().path) +
               " — xfmd " + buildVersion();
  window->setTitle(title.c_str());
  window->status->setText((std::to_string(session.view().text.size()) + " bytes" +
                           (session.dirty() ? " — modified" : " — saved"))
                              .c_str());
}
bool Application::startPath(const std::string& path) {
  std::error_code ec;
  const auto resolved = std::filesystem::canonical(path, ec);
  if (ec) {
    window->status->setText(("Cannot open path: " + path + ": " + ec.message()).c_str());
    return false;
  }
  if (std::filesystem::is_directory(resolved, ec)) {
    bool changed = window->workspacePanel->setWorkPath(resolved.string());
    if (!changed)
      window->status->setText(window->workspacePanel->searchStatus->getText());
    return changed;
  }
  if (!open(resolved.string()))
    return false;
  return window->workspacePanel->setWorkPath(resolved.parent_path().string());
}
} // namespace xfmd
