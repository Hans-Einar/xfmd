#pragma once
#include "application/workspace/DirectoryScanner.h"
#include <functional>
#include <fx.h>
#include <map>
#include <set>
namespace xfmd {
class SidebarWidget : public FX::FXTreeList {
  FXDECLARE(SidebarWidget)
  DirectoryScanner scanner;
  FileNameFilter filter;
  std::filesystem::path root;
  std::map<std::string, FX::FXTreeItem*> items;
  std::set<std::string> requested;
  std::string contextPath, rootLabel, pendingOpen;
  bool pointerClick = false, pointerSystemDefault = false, pendingSystemDefault = false;
  std::size_t files = 0;
  std::set<std::string> restoreExpanded;
  std::string restoreSelected;
  int restoreX = 0, restoreY = 0;
  bool restoring = false;
  FX::FXTreeItem* add(const TreeEntry&);

protected:
  SidebarWidget() = default;

public:
  enum { ID_TREE_EVENT = FX::FXTreeList::ID_LAST, ID_POLL, ID_ACTIVATE, ID_SET_WORK_PATH, ID_LAST };
  std::function<void(const std::string&, bool)> open;
  std::function<void(const std::string&)> workPathRequested;
  std::function<void()> broadenRoot;
  std::function<void(const std::string&)> status;
  explicit SidebarWidget(FX::FXComposite*);
  ~SidebarWidget() override;
  void create() override;
  void setRoot(const std::filesystem::path&, const std::string& label);
  void setFilter(FileNameFilter);
  void refresh();
  const std::filesystem::path& workRoot() const { return root; }
  bool scanning = false;
  FX::FXTreeItem* getPathnameItem(const FX::FXString&) const;
  FX::FXString getItemPathname(const FX::FXTreeItem*) const;
  bool isItemDirectory(const FX::FXTreeItem*) const;
  bool isItemFile(const FX::FXTreeItem* item) const { return item && !isItemDirectory(item); }
  FX::FXbool expandTree(FX::FXTreeItem*, FX::FXbool notify = false) override;
  long onRelease(FX::FXObject*, FX::FXSelector, void*);
  long onKey(FX::FXObject*, FX::FXSelector, void*);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
  long onOpen(FX::FXObject*, FX::FXSelector, void*);
  long onPoll(FX::FXObject*, FX::FXSelector, void*);
  long onContext(FX::FXObject*, FX::FXSelector, void*);
  long onSetWorkPath(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
