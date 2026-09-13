#pragma once
#include "EditorWidget.h"
#include "WorkspacePanel.h"
#include "application/commands/CommandRouter.h"
#include <fx.h>
namespace xfmd {
class XfmdWindow : public FX::FXMainWindow {
  FXDECLARE(XfmdWindow)
  CommandRouter* commands = nullptr;
  FX::FXMenuPane *fileMenu = nullptr, *editMenu = nullptr, *viewMenu = nullptr, *goMenu = nullptr;

protected:
  XfmdWindow() = default;

public:
  EditorWidget* editor = nullptr;
  SidebarWidget* sidebar = nullptr;
  WorkspacePanel* workspacePanel = nullptr;
  FX::FXSplitter* split = nullptr;
  FX::FXVerticalFrame* previewArea = nullptr;
  FX::FXLabel* status = nullptr;
  XfmdWindow(FX::FXApp*, CommandRouter&);
  ~XfmdWindow() override;
  void buildUi();
  std::function<void()> configured;
  long onConfigure(FX::FXObject*, FX::FXSelector, void*);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onClose(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
