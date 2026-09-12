#pragma once
#include <fx.h>
#include "EditorWidget.h"
#include "SidebarWidget.h"
#include "application/commands/CommandRouter.h"
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
  FX::FXSplitter* split = nullptr;
  FX::FXVerticalFrame* previewArea = nullptr;
  FX::FXLabel* status = nullptr;
  XfmdWindow(FX::FXApp*, CommandRouter&);
  ~XfmdWindow() override;
  void buildUi();
  long onClose(FX::FXObject*, FX::FXSelector, void*);
};
}
