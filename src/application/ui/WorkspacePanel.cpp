#include "WorkspacePanel.h"
#include "application/adapters/FoxWheelScrollBar.h"
using namespace FX;
namespace xfmd {
namespace {
class WorkPathList : public FXList {
public:
  WorkPathList(FXComposite* parent, FXObject* target, FXSelector selector)
      : FXList(parent, target, selector, LIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
    horizontal = FoxWheelScrollBar::replace(horizontal);
    vertical = FoxWheelScrollBar::replace(vertical);
  }
};
} // namespace

FXDEFMAP(WorkspacePanel)
panelMap[] = {
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_TAB, WorkspacePanel::onTab),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_REFRESH, WorkspacePanel::onRefresh),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_UP, WorkspacePanel::onUp),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_OPEN, WorkspacePanel::onOpen),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_HISTORY, WorkspacePanel::onHistory),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_MARKDOWN, WorkspacePanel::onMarkdown),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_FILTER, WorkspacePanel::onFilter),
    FXMAPFUNC(SEL_CHANGED, WorkspacePanel::ID_FILTER, WorkspacePanel::onFilter),
    FXMAPFUNC(SEL_TIMEOUT, WorkspacePanel::ID_ACTIVATE, WorkspacePanel::onActivate),
    FXMAPFUNC(SEL_TIMEOUT, WorkspacePanel::ID_FILTER_APPLY, WorkspacePanel::onApplyFilter)};
FXIMPLEMENT(WorkspacePanel, FXVerticalFrame, panelMap, ARRAYNUMBER(panelMap))
WorkspacePanel::WorkspacePanel(FXComposite* parent, UiContext& context)
    : FXVerticalFrame(parent, LAYOUT_FILL_Y, 0, 0, 260, 0, 2, 2, 2, 2),
      history(FXSystem::getHomeDirectory().text()) {
  UiFactory ui(context);
  auto* tabRow = new FXHorizontalFrame(this, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0);
  tabs =
      new FXTabBar(tabRow, this, ID_TAB, LAYOUT_FILL_X | LAYOUT_CENTER_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  new FXTabItem(tabs, "Files");
  new FXTabItem(tabs, "Index");
  refreshButton = ui.button(tabRow, "\tRefresh Files", this, ID_REFRESH, UiIcon::Refresh);
  markdown = ui.button(tabRow, "\tShow only Markdown files (off: all file types)", this,
                       ID_MARKDOWN, UiIcon::Markdown, ButtonRole::Pill);
  upButton = ui.button(tabRow, "\tUp one folder", this, ID_UP, UiIcon::Up);
  openButton = ui.button(tabRow, "\tOpen file or folder (Ctrl+O)", this, ID_OPEN, UiIcon::Open);
  for (auto* button : {refreshButton, markdown, upButton, openButton})
    button->setCompact(true);
  pages = new FXSwitcher(this, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* filePage =
      new FXVerticalFrame(pages, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  auto* split = new FXSplitter(filePage, SPLITTER_VERTICAL | SPLITTER_REVERSED | SPLITTER_TRACKING |
                                             LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* upper = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 450, 0, 0, 0, 0);
  tree = new SidebarWidget(upper);
  searchStatus = new FXLabel(upper, "", nullptr, LAYOUT_FILL_X | JUSTIFY_LEFT);
  auto* lower = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 240, 0, 0, 0, 0);
  recentTabs = new FXTabBook(lower, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  new FXTabItem(recentTabs, "Folders\tRecent folders");
  auto* folders =
      new FXVerticalFrame(recentTabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  workPaths = new WorkPathList(folders, this, ID_HISTORY);
  workPaths->setNumVisible(4);
  new FXTabItem(recentTabs, "Files\tRecent files");
  recentFiles = new RecentFilesPanel(recentTabs, context);
  index = new IndexPanel(pages, context);
  std::vector<std::string> saved;
  for (int i = 0; i < 32; ++i) {
    auto key = "Path" + std::to_string(i);
    auto value = getApp()->reg().readStringEntry("WorkPaths", key.c_str(), "");
    if (*value)
      saved.emplace_back(value);
  }
  history.restore(saved);
  tree->workPathRequested = [this](const std::string& path) { requestWorkPath(path); };
  tree->broadenRoot = [this] { requestWorkPath(history.broaderRoot().string()); };
  tree->status = [this](const std::string& value) {
    if (!pathError)
      searchStatus->setText(value.c_str());
  };
}
WorkspacePanel::~WorkspacePanel() {
  getApp()->removeTimeout(this, ID_ACTIVATE);
  getApp()->removeTimeout(this, ID_FILTER_APPLY);
}
void WorkspacePanel::remember() {
  workPaths->clearItems();
  for (int i = 0; i < 32; ++i) {
    auto key = "Path" + std::to_string(i);
    std::string value = i < int(history.entries().size()) ? history.entries()[i] : "";
    getApp()->reg().writeStringEntry("WorkPaths", key.c_str(), value.c_str());
    if (!value.empty())
      workPaths->appendItem(history.displayPath(value).c_str());
  }
  workPaths->setCurrentItem(0);
  getApp()->reg().write();
}
bool WorkspacePanel::setWorkPath(const std::string& path) {
  try {
    history.activate(path);
    if (history.root() == history.root().root_path())
      upButton->disable();
    else
      upButton->enable();
    pathError = false;
    searchStatus->setTipText("");
    tree->setRoot(history.root(), history.displayPath(history.root()));
    remember();
    if (rootChanged)
      rootChanged(history.root().string());
    return true;
  } catch (const std::exception& e) {
    pathError = true;
    searchStatus->setText((std::string("Cannot set work path: ") + e.what()).c_str());
    searchStatus->setTipText(searchStatus->getText());
    return false;
  }
}
void WorkspacePanel::requestWorkPath(const std::string& path) {
  pendingPath = path;
  getApp()->addTimeout(this, ID_ACTIVATE, 0);
}
long WorkspacePanel::onActivate(FXObject*, FXSelector, void*) {
  setWorkPath(pendingPath);
  return 1;
}
long WorkspacePanel::onHistory(FXObject*, FXSelector, void*) {
  int index = workPaths->getCurrentItem();
  if (index >= 0 && index < int(history.entries().size()))
    requestWorkPath(history.entries()[index]);
  return 1;
}
void WorkspacePanel::setNameFilter(const std::string& pattern) {
  if (namePattern == pattern)
    return;
  namePattern = pattern;
  onFilter(nullptr, 0, nullptr);
}
long WorkspacePanel::onTab(FXObject*, FXSelector, void*) {
  pages->setCurrent(tabs->getCurrent());
  for (auto* button : {markdown, upButton, openButton}) {
    if (tabs->getCurrent() == 0)
      button->show();
    else
      button->hide();
  }
  refreshButton->getParent()->recalc();
  refreshButton->setTipText(tabs->getCurrent() == 0 ? "Refresh Files"
                                                    : "Refresh Index from current buffer");
  return 1;
}
long WorkspacePanel::onRefresh(FXObject*, FXSelector, void*) {
  if (tabs->getCurrent() == 0) {
    getApp()->removeTimeout(this, ID_FILTER_APPLY);
    if (filterPending)
      onApplyFilter(nullptr, 0, nullptr);
    else
      tree->refresh();
  } else if (indexRefresh)
    indexRefresh();
  return 1;
}
long WorkspacePanel::onUp(FXObject*, FXSelector, void*) {
  if (history.root() != history.root().root_path())
    requestWorkPath(history.root().parent_path().string());
  return 1;
}
long WorkspacePanel::onOpen(FXObject*, FXSelector, void*) {
  if (openRequested)
    openRequested();
  return 1;
}
long WorkspacePanel::onMarkdown(FXObject*, FXSelector, void*) {
  markdown->setChecked(!markdown->isChecked());
  return onFilter(nullptr, 0, nullptr);
}
long WorkspacePanel::onFilter(FXObject*, FXSelector, void*) {
  filterPending = true;
  getApp()->addTimeout(this, ID_FILTER_APPLY, 200);
  return 1;
}
long WorkspacePanel::onApplyFilter(FXObject*, FXSelector, void*) {
  filterPending = false;
  getApp()->removeTimeout(this, ID_FILTER_APPLY);
  pathError = false;
  tree->setFilter({markdown->isChecked(), false, namePattern});
  return 1;
}
} // namespace xfmd
