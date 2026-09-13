#pragma once
#include "adapters/FoxRenderHost.h"
#include "adapters/FoxScheduler.h"
#include "adapters/FoxTextMetrics.h"
#include "commands/CommandRouter.h"
#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include "document/DocumentCoordinator.h"
#include "document/EditController.h"
#include "navigation/NavigationCoordinator.h"
#include "preview/PreviewCoordinator.h"
#include "scroll/ScrollCoordinator.h"
#include "ui/ViewModeController.h"
#include "ui/XfmdWindow.h"
#include <fx.h>
#include <memory>
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
  std::unique_ptr<FX::FXFont> editorFont;
  std::unique_ptr<IInterpreter> interpreter;
  std::unique_ptr<IRenderer> renderer;
  std::unique_ptr<FoxTextMetrics> metrics;
  FoxRenderHost* host = nullptr;
  std::unique_ptr<FoxScheduler> scheduler;
  std::unique_ptr<PreviewCoordinator> preview;
  ScrollCoordinator scrolling;
  std::unique_ptr<NavigationCoordinator> navigation;
  std::function<void()> contentChanged, documentOpened;
  std::function<void()> back, forward;
  std::function<bool(bool)> canNavigate;
  Application() = default;
  ~Application();
  void initialize(int&, char**);
  void updateUi();
  void execute(CommandRouter::Command);
  bool startPath(const std::string& path);
  bool open(const std::string& path);

private:
  void wireDocument();
  std::string savePath();
};
} // namespace xfmd
