#pragma once
#include "FavoritesPanel.h"
#include "IndexPanel.h"
#include "RecentFilesPanel.h"
#include "SidebarWidget.h"
#include "application/workspace/WorkPathHistory.h"
#include "controls/UiLayout.h"
namespace xfmd {
class WorkspacePanel : public FX::FXVerticalFrame {
  FXDECLARE(WorkspacePanel)
  std::string pendingPath;
  std::string namePattern;
  FX::FXSwitcher* pages = nullptr;
  bool pathError = false, filterPending = false;
  void remember();

protected:
  WorkspacePanel() : history("/") {}

public:
  enum {
    ID_FILTER = FX::FXVerticalFrame::ID_LAST,
    ID_MARKDOWN,
    ID_HISTORY,
    ID_ACTIVATE,
    ID_FILTER_APPLY,
    ID_TAB,
    ID_REFRESH,
    ID_UP,
    ID_OPEN,
    ID_LAST
  };
  WorkPathHistory history;
  SidebarWidget* tree = nullptr;
  FX::FXTabBar* tabs = nullptr;
  FX::FXTabBook* recentTabs = nullptr;
  UiButton *refreshButton = nullptr, *upButton = nullptr, *openButton = nullptr;
  IndexPanel* index = nullptr;
  std::function<void()> openRequested, indexRefresh;
  std::function<void(const std::string&)> rootChanged;
  UiButton* markdown = nullptr;
  FX::FXList* workPaths = nullptr;
  RecentFilesPanel* recentFiles = nullptr;
  FavoritesPanel* favorites = nullptr;
  FX::FXLabel* searchStatus = nullptr;
  WorkspacePanel(FX::FXComposite*, UiContext&);
  ~WorkspacePanel() override;
  bool setWorkPath(const std::string&);
  void setNameFilter(const std::string&);
  const std::string& nameFilter() const { return namePattern; }
  long onTab(FX::FXObject*, FX::FXSelector, void*);
  long onRefresh(FX::FXObject*, FX::FXSelector, void*);
  long onUp(FX::FXObject*, FX::FXSelector, void*);
  long onOpen(FX::FXObject*, FX::FXSelector, void*);
  void requestWorkPath(const std::string&);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
  long onHistory(FX::FXObject*, FX::FXSelector, void*);
  long onMarkdown(FX::FXObject*, FX::FXSelector, void*);
  long onFilter(FX::FXObject*, FX::FXSelector, void*);
  long onApplyFilter(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
