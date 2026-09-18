#pragma once
#include "EditorWidget.h"
#include "WorkspacePanel.h"
#include "application/commands/CommandRouter.h"
#include "controls/CompactToolbar.h"
#include "controls/PreviewColorControls.h"
#include "controls/PreviewControls.h"
#include "controls/UiLayout.h"
#include <fx.h>
namespace xfmd {
class XfmdWindow : public FX::FXMainWindow {
  FXDECLARE(XfmdWindow)
  CommandRouter* commands = nullptr;
  UiContext* ui = nullptr;
  CompactToolbar* toolbar = nullptr;
  FX::FXHorizontalFrame *fileTools = nullptr, *navTools = nullptr, *viewTools = nullptr;
  FX::FXLabel* documentTitle = nullptr;
  UiButton* themeButton = nullptr;
  FX::FXMenuPane *fileMenu = nullptr, *editMenu = nullptr, *viewMenu = nullptr, *goMenu = nullptr;

protected:
  XfmdWindow() = default;

public:
  PreviewControls* previewControls = nullptr;
  PreviewColorControls* previewColors = nullptr;
  EditorWidget* editor = nullptr;
  SidebarWidget* sidebar = nullptr;
  WorkspacePanel* workspacePanel = nullptr;
  FX::FXSplitter* split = nullptr;
  FX::FXVerticalFrame* previewArea = nullptr;
  FX::FXLabel* status = nullptr;
  FX::FXLabel* versionStatus = nullptr;
  XfmdWindow(FX::FXApp*, CommandRouter&, UiContext&);
  void buildToolbar();
  void setDocumentLabel(const std::string&, bool dirty);
  void restyle();
  void layoutToolbar();
  ~XfmdWindow() override;
  void buildUi();
  void setApplicationIcons(FX::FXIcon*, FX::FXIcon*);
  std::function<void()> configured;
  long onConfigure(FX::FXObject*, FX::FXSelector, void*);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onClose(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
