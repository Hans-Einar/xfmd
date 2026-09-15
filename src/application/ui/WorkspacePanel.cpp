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
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_HISTORY, WorkspacePanel::onHistory),
    FXMAPFUNC(SEL_COMMAND, WorkspacePanel::ID_FILTER, WorkspacePanel::onFilter),
    FXMAPFUNC(SEL_CHANGED, WorkspacePanel::ID_FILTER, WorkspacePanel::onFilter),
    FXMAPFUNC(SEL_TIMEOUT, WorkspacePanel::ID_ACTIVATE, WorkspacePanel::onActivate),
    FXMAPFUNC(SEL_TIMEOUT, WorkspacePanel::ID_FILTER_APPLY, WorkspacePanel::onApplyFilter)};
FXIMPLEMENT(WorkspacePanel, FXVerticalFrame, panelMap, ARRAYNUMBER(panelMap))
WorkspacePanel::WorkspacePanel(FXComposite* parent, UiContext& context)
    : FXVerticalFrame(parent, LAYOUT_FILL_Y, 0, 0, 260, 0, 2, 2, 2, 2),
      history(FXSystem::getHomeDirectory().text()) {
  tabs = new FXTabBook(this, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  new FXTabItem(tabs, "Files");
  auto* filePage = new FXVerticalFrame(tabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  UiFactory ui(context);
  auto* heading = ui.row(filePage);
  rootLabel =
      new FXLabel(heading, "Files", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  ui.button(heading, "\tRefresh files", this, ID_FILTER, UiIcon::Refresh);
  auto* filterRow = ui.row(filePage);
  ui.button(filterRow, "\tApply filename filter (* and ?)", this, ID_FILTER, UiIcon::Search);
  filterInput = new FXTextField(filterRow, 14, this, ID_FILTER,
                                TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  filterInput->setTipText("Filter filenames: text contains; ? one character; * any characters");
  auto* row = ui.row(filePage);
  markdown =
      new FXToggleButton(row, ".md", ".md", nullptr, nullptr, this, ID_FILTER,
                         TOGGLEBUTTON_NORMAL | TOGGLEBUTTON_TOOLBAR | TOGGLEBUTTON_KEEPSTATE);
  text = new FXToggleButton(row, ".txt", ".txt", nullptr, nullptr, this, ID_FILTER,
                            TOGGLEBUTTON_NORMAL | TOGGLEBUTTON_TOOLBAR | TOGGLEBUTTON_KEEPSTATE);
  markdown->setTipText("Include Markdown (OR with .txt, then AND name filter)");
  text->setTipText("Include text files (OR with .md, then AND name filter)");
  auto* split = new FXSplitter(filePage, SPLITTER_VERTICAL | SPLITTER_REVERSED | SPLITTER_TRACKING |
                                             LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* upper = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 450, 0, 0, 0, 0);
  tree = new SidebarWidget(upper);
  searchStatus = new FXLabel(upper, "", nullptr, LAYOUT_FILL_X | JUSTIFY_LEFT);
  auto* lower = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 240, 0, 0, 0, 0);
  ui.header(lower, "Recent folders");
  workPaths = new WorkPathList(lower, this, ID_HISTORY);
  workPaths->setNumVisible(4);
  recentFiles = new RecentFilesPanel(lower, context);
  new FXTabItem(tabs, "Index");
  index = new IndexPanel(tabs, context);
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
    rootLabel->setText(
        history.root().filename().empty() ? "/" : history.root().filename().string().c_str());
    rootLabel->setTipText(history.root().string().c_str());
    pathError = false;
    searchStatus->setTipText("");
    tree->setRoot(history.root(), history.displayPath(history.root()));
    remember();
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
long WorkspacePanel::onFilter(FXObject*, FXSelector, void*) {
  getApp()->addTimeout(this, ID_FILTER_APPLY, 200);
  return 1;
}
long WorkspacePanel::onApplyFilter(FXObject*, FXSelector, void*) {
  pathError = false;
  tree->setFilter(
      {bool(markdown->getState()), bool(text->getState()), filterInput->getText().text()});
  return 1;
}
} // namespace xfmd
