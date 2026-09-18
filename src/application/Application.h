#pragma once
#include "adapters/DesktopFileOpener.h"
#include "adapters/ExternalBrowser.h"
#include "adapters/FoxBoxUiOverlay.h"
#include "adapters/FoxPreferencesStore.h"
#include "adapters/FoxRenderHost.h"
#include "adapters/FoxScheduler.h"
#include "adapters/FoxWindowMode.h"
#include "adapters/SharedTextMetrics.h"
#include "boxui/BoxUiSession.h"
#include "commands/CommandRouter.h"
#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include "document/DocumentCoordinator.h"
#include "document/EditController.h"
#include "export/ExportCoordinator.h"
#include "index/ReferenceWorker.h"
#include "navigation/NavigationCoordinator.h"
#include "preview/PreviewCoordinator.h"
#include "scroll/ScrollCoordinator.h"
#include "ui/IconResources.h"
#include "ui/ViewModeController.h"
#include "ui/XfmdWindow.h"
#include <fx.h>
#include <memory>
namespace xfmd {
class Application {
public:
  FX::FXApp app{"xfmd", "xfmd"};
  DocumentSession session;
  BoxUiSession boxUiSession;
  std::unique_ptr<FoxBoxUiOverlay> boxUiOverlay;
  LocalFileStore files;
  DocumentCoordinator documents{session, files};
  EditController edits{session};
  CommandRouter commands;
  std::unique_ptr<FoxPreferencesStore> preferencesStore;
  std::unique_ptr<PreferencesService> preferences;
  IconResources icons;
  std::unique_ptr<UiContext> ui;
  XfmdWindow* window = nullptr;
  std::unique_ptr<ViewModeController> views;
  std::unique_ptr<FoxWindowMode> windowMode;
  std::unique_ptr<FX::FXFont> editorFont;
  std::unique_ptr<IInterpreter> interpreter, referenceInterpreter;
  std::unique_ptr<ReferenceWorker> references;
  ExternalBrowser browser;
  DesktopFileOpener desktopFiles;
  std::unique_ptr<IRenderer> renderer;
  std::unique_ptr<SharedTextMetrics> metrics;
  FoxRenderHost* host = nullptr;
  std::unique_ptr<FoxScheduler> scheduler;
  std::unique_ptr<PreviewCoordinator> preview;
  std::unique_ptr<ExportCoordinator> exporter;
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
  bool startExport(const std::string& path);

private:
  void wireDocument();
  void wireBoxUi();
  void showLinkTarget(const std::string&);
  std::string hoverStatus, beforeHover;
  void changeReadingColors(const ReadingColors&, bool commit);
  void applyAppearance(const Appearance&);
  void wireIndex();
  void pollReferences();
  void pollBrowser();
  void openTreePath(const std::string&);
  void pollDesktopFiles();
  void openBrowser(const std::string&);
  void activateIndex(const IndexAction&);
  std::optional<IndexAction> pendingHeading;
  void chooseExport();
  void pollExport();
  std::string savePath();
};
} // namespace xfmd
