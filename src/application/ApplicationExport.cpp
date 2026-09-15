#include "Application.h"
#include "export/ExportPipeline.h"
#include "composition/DiagramServices.h"
#include "renderer/MarkdownRenderer.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
bool Application::startExport(const std::string& path) {
  if (exporter && exporter->busy())
    return false;
  try {
    auto target = PdfTarget::inspect(path);
    if (!session.view().path.empty() &&
        std::filesystem::weakly_canonical(session.view().path) == target.path)
      throw Error(ErrorCode::Conflict, "Choose a PDF target different from the source document.");
    ExportRequest request{session.snapshot(), preview->layoutProfile().paper, metrics->fontSetId(),
                          target.path, preview->frame()};
    exporter = std::make_unique<ExportCoordinator>([target](const auto& frozen, auto& control) {
      auto parser = DiagramServices::interpreter();
      MarkdownRenderer renderer;
      SharedTextMetrics fonts;
      return ExportPipeline::run(frozen, target, control, *parser, renderer, fonts);
    });
    exporter->start(std::move(request));
    pollExport();
    return true;
  } catch (const std::exception& e) {
    window->status->setText(e.what());
    return false;
  }
}
void Application::chooseExport() {
  std::filesystem::path initial = session.view().path.empty() ? "Untitled.md" : session.view().path;
  initial.replace_extension(".pdf");
  std::string path = FXFileDialog::getSaveFilename(window, "Export current buffer to PDF",
                                                   initial.c_str(), "PDF (*.pdf)")
                         .text();
  if (path.empty())
    return;
  auto ext = FXString(std::filesystem::path(path).extension().c_str()).lower();
  if (ext != ".pdf")
    path += ".pdf";
  if (std::filesystem::exists(path) &&
      FXMessageBox::question(window, MBOX_YES_NO, "Replace PDF", "Replace %s?", path.c_str()) !=
          MBOX_CLICKED_YES)
    return;
  if (!startExport(path))
    FXMessageBox::error(window, MBOX_OK, "PDF export", "%s", window->status->getText().text());
}
void Application::pollExport() {
  if (!exporter)
    return;
  if (auto result = exporter->take()) {
    std::string message;
    if (result->success) {
      message =
          "Exported revision " + std::to_string(result->token.revision) + " — " + result->target;
      if (result->token != session.view().token)
        message += " (current buffer has changed)";
    } else
      message = result->cancelled ? "PDF export cancelled." : "PDF export failed: " + result->error;
    window->status->setText(message.c_str());
    return;
  }
  auto progress = exporter->progress();
  window->status->setText((progress.second ? "Exporting PDF: " + std::to_string(progress.first) +
                                                 "/" + std::to_string(progress.second)
                                           : "Preparing PDF…")
                              .c_str());
  scheduler->restart(3, 50, [this] { pollExport(); });
}
} // namespace xfmd
