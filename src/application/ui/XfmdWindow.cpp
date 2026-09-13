#include "XfmdWindow.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(XfmdWindow) windowMap[] = {FXMAPFUNC(SEL_CLOSE, 0, XfmdWindow::onClose)};
FXIMPLEMENT(XfmdWindow, FXMainWindow, windowMap, ARRAYNUMBER(windowMap))
XfmdWindow::XfmdWindow(FXApp* app, CommandRouter& router)
    : FXMainWindow(app, "xfmd", nullptr, nullptr, DECOR_ALL, 0, 0, 1100, 760), commands(&router) {
  buildUi();
}
XfmdWindow::~XfmdWindow() {
  delete fileMenu;
  delete editMenu;
  delete viewMenu;
  delete goMenu;
}
void XfmdWindow::buildUi() {
  auto* bar = new FXMenuBar(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X);
  auto add = [&](FXMenuPane* menu, const char* name, CommandRouter::Command command) {
    new FXMenuCommand(menu, name, nullptr, commands, command);
  };
  fileMenu = new FXMenuPane(this);
  add(fileMenu, "&Open...\tCtrl+O", CommandRouter::Open);
  add(fileMenu, "&Save\tCtrl+S", CommandRouter::Save);
  add(fileMenu, "Save &As...\tCtrl+Shift+S", CommandRouter::SaveAs);
  add(fileMenu, "&Quit\tCtrl+Q", CommandRouter::Close);
  new FXMenuTitle(bar, "&File", nullptr, fileMenu);
  editMenu = new FXMenuPane(this);
  add(editMenu, "&Undo\tCtrl+Z", CommandRouter::Undo);
  add(editMenu, "&Redo\tCtrl+Y", CommandRouter::Redo);
  add(editMenu, "&Find...\tCtrl+F", CommandRouter::Find);
  add(editMenu, "&Preferences...", CommandRouter::Preferences);
  new FXMenuTitle(bar, "&Edit", nullptr, editMenu);
  viewMenu = new FXMenuPane(this);
  add(viewMenu, "&Preview\tCtrl+1", CommandRouter::Preview);
  add(viewMenu, "&Editor\tCtrl+2", CommandRouter::Editor);
  add(viewMenu, "&Split view\tCtrl+3", CommandRouter::Split);
  add(viewMenu, "&Sidebar\tF10", CommandRouter::Sidebar);
  new FXMenuTitle(bar, "&View", nullptr, viewMenu);
  goMenu = new FXMenuPane(this);
  add(goMenu, "&Back\tAlt+Left", CommandRouter::Back);
  add(goMenu, "&Forward\tAlt+Right", CommandRouter::Forward);
  new FXMenuTitle(bar, "&Go", nullptr, goMenu);
  auto* toolbar =
      new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | PACK_UNIFORM_HEIGHT);
  for (const auto& item :
       {std::pair<const char*, CommandRouter::Command>{"Open", CommandRouter::Open},
        {"Save", CommandRouter::Save},
        {"Back", CommandRouter::Back},
        {"Forward", CommandRouter::Forward},
        {"Preview", CommandRouter::Preview},
        {"Split", CommandRouter::Split},
        {"Editor", CommandRouter::Editor}})
    new FXButton(toolbar, item.first, nullptr, commands, item.second, BUTTON_NORMAL);
  status = new FXLabel(this, "Open a local Markdown or text file.", nullptr,
                       LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | JUSTIFY_LEFT);
  auto* workspace =
      new FXSplitter(this, SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  workspacePanel = new WorkspacePanel(workspace);
  sidebar = workspacePanel->tree;
  split = new FXSplitter(workspace,
                         SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  editor = new EditorWidget(split);
  previewArea = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  editor->setWidth(420);
}
long XfmdWindow::onClose(FXObject*, FXSelector, void*) {
  commands->dispatch(this, FXSEL(SEL_COMMAND, CommandRouter::Close), nullptr);
  return 1;
}
} // namespace xfmd
