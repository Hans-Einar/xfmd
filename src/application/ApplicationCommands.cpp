#include "Application.h"
#include "ui/PreferencesDialog.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
std::string Application::savePath() {
  return FXFileDialog::getSaveFilename(window, "Save document",
                                       session.view().path.empty() ? "Untitled.md"
                                                                   : session.view().path.c_str(),
                                       "Markdown (*.md)\nText (*.txt)")
      .text();
}
void Application::execute(CommandRouter::Command command) {
  switch (command) {
  case CommandRouter::FullScreen:
  case CommandRouter::LeaveFullScreen:
    if (!app.getModalWindow() &&
        (command == CommandRouter::FullScreen || windowMode->requested())) {
      if (!windowMode->requestFullscreen(command == CommandRouter::FullScreen &&
                                         !windowMode->requested()))
        window->status->setText("This window manager does not support fullscreen.");
    }
    break;
  case CommandRouter::ExportPdf:
    chooseExport();
    break;
  case CommandRouter::CancelExport:
    if (exporter)
      exporter->cancel();
    break;
  case CommandRouter::WindowWrap:
  case CommandRouter::A4: {
    auto profile = preview->layoutProfile();
    profile.mode = command == CommandRouter::A4 ? LayoutMode::Paged : LayoutMode::Continuous;
    preview->setLayoutProfile(profile);
    break;
  }
  case CommandRouter::FitWidth:
    host->setViewScale(true);
    break;
  case CommandRouter::ActualSize:
    host->setViewScale(false, 1);
    break;
  case CommandRouter::Preferences: {
    PreferencesDialog dialog(window, *preferences);
    dialog.execute(PLACEMENT_OWNER);
    break;
  }
  case CommandRouter::Open: {
    auto path = FXFileDialog::getOpenFilename(window, "Open document", session.view().path.c_str(),
                                              "Markdown and text (*.md,*.txt)");
    if (!path.empty())
      open(path.text());
    break;
  }
  case CommandRouter::Save:
    documents.save();
    break;
  case CommandRouter::SaveAs: {
    auto path = savePath();
    if (path.empty())
      break;
    bool overwrite = std::filesystem::exists(path);
    if (overwrite && FXMessageBox::question(window, MBOX_YES_NO, "Replace file", "Replace %s?",
                                            path.c_str()) != MBOX_CLICKED_YES)
      break;
    documents.save(path, overwrite);
    break;
  }
  case CommandRouter::Close:
    if (documents.requestClose())
      app.exit(0);
    break;
  case CommandRouter::Undo:
    edits.undo();
    updateUi();
    break;
  case CommandRouter::Redo:
    edits.redo();
    updateUi();
    break;
  case CommandRouter::Find: {
    FXString query;
    if (FXInputDialog::getString(query, window, "Find", "Text to find:") && !query.empty()) {
      auto found = edits.find(query.text(), window->editor->getCursorPos());
      if (found != std::string::npos) {
        views->setMode(views->mode() == ViewMode::Preview ? ViewMode::Split : views->mode());
        window->editor->setSelection(int(found), query.length());
        window->editor->setCursorPos(int(found) + query.length());
        window->editor->makePositionVisible(int(found));
      } else
        window->status->setText("Text not found.");
    }
    break;
  }
  case CommandRouter::Preview:
    views->setMode(ViewMode::Preview);
    break;
  case CommandRouter::Editor:
    views->setMode(ViewMode::Editor);
    break;
  case CommandRouter::Split:
    views->setMode(ViewMode::Split);
    break;
  case CommandRouter::Sidebar:
    views->toggleSidebar();
    break;
  case CommandRouter::Back:
    if (back)
      back();
    break;
  case CommandRouter::Forward:
    if (forward)
      forward();
    break;
  default:
    break;
  }
}
} // namespace xfmd
