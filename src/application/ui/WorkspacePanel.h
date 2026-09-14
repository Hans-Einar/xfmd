#pragma once
#include "IndexPanel.h"
#include "SidebarWidget.h"
#include "application/workspace/WorkPathHistory.h"
#include "controls/UiLayout.h"
namespace xfmd {
class WorkspacePanel : public FX::FXVerticalFrame {
  FXDECLARE(WorkspacePanel)
  std::string pendingPath;
  FX::FXLabel* rootLabel = nullptr;
  bool pathError = false;
  void remember();

protected:
  WorkspacePanel() : history("/") {}

public:
  enum {
    ID_FILTER = FX::FXVerticalFrame::ID_LAST,
    ID_HISTORY,
    ID_ACTIVATE,
    ID_FILTER_APPLY,
    ID_LAST
  };
  WorkPathHistory history;
  SidebarWidget* tree = nullptr;
  FX::FXTabBook* tabs = nullptr;
  IndexPanel* index = nullptr;
  FX::FXTextField* filterInput = nullptr;
  FX::FXToggleButton *markdown = nullptr, *text = nullptr;
  FX::FXList* workPaths = nullptr;
  FX::FXLabel* searchStatus = nullptr;
  WorkspacePanel(FX::FXComposite*, UiContext&);
  ~WorkspacePanel() override;
  bool setWorkPath(const std::string&);
  void requestWorkPath(const std::string&);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
  long onHistory(FX::FXObject*, FX::FXSelector, void*);
  long onFilter(FX::FXObject*, FX::FXSelector, void*);
  long onApplyFilter(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
