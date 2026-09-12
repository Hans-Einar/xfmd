#pragma once
#include <fx.h>
#include "document/DocumentCoordinator.h"
#include "document/EditController.h"
#include "commands/CommandRouter.h"
#include "ui/ViewModeController.h"
#include "ui/XfmdWindow.h"
#include <memory>
#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include "adapters/FoxTextMetrics.h"
#include "adapters/FoxRenderHost.h"
#include "preview/PreviewCoordinator.h"
namespace xfmd {
class Application {
public:
  FX::FXApp app{"xfmd", "xfmd"};
  DocumentSession session;
  LocalFileStore files;
  DocumentCoordinator documents{session, files};
  EditController edits{session};
  CommandRouter commands;
  XfmdWindow* window = nullptr;
  std::unique_ptr<ViewModeController> views;
  std::unique_ptr<IInterpreter> interpreter;
  std::unique_ptr<IRenderer> renderer;
  std::unique_ptr<FoxTextMetrics> metrics;
  FoxRenderHost* host = nullptr;
  std::unique_ptr<PreviewCoordinator> preview;
  std::function<void()> contentChanged, documentOpened;
  std::function<void()> back, forward;
  std::function<bool(bool)> canNavigate;
  Application() = default;
  ~Application();
  void initialize(int&, char**);
  void updateUi();
  void execute(CommandRouter::Command);
  bool open(const std::string& path) { return documents.requestOpen(path); }
private:
  void wireDocument();
  std::string savePath();
};
}
