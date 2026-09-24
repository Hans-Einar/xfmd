#include "XfmdWindow.h"
#include "application/build/BuildVersion.h"
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(XfmdWindow)
windowMap[] = {FXMAPFUNC(SEL_CLOSE, 0, XfmdWindow::onClose),
               FXMAPFUNC(SEL_CONFIGURE, 0, XfmdWindow::onConfigure),
               FXMAPFUNC(SEL_KEYPRESS, 0, XfmdWindow::onKeyPress)};
FXIMPLEMENT(XfmdWindow, FXMainWindow, windowMap, ARRAYNUMBER(windowMap))
XfmdWindow::XfmdWindow(FXApp* app, CommandRouter& router, UiContext& context)
    : FXMainWindow(app, "xfmd", nullptr, nullptr, DECOR_ALL, 0, 0, 1100, 760), commands(&router),
      ui(&context) {
  buildUi();
}
XfmdWindow::~XfmdWindow() {
  delete colorPopup;
  delete fileMenu;
  delete editMenu;
  delete viewMenu;
  delete goMenu;
}
void XfmdWindow::setApplicationIcons(FXIcon* large, FXIcon* small) {
  setIcon(large);
  setMiniIcon(small);
}
void XfmdWindow::buildUi() {
  toolbar = new CompactToolbar(this);
  auto* bar = new FXMenuBar(toolbar, LAYOUT_CENTER_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  auto add = [&](FXMenuPane* menu, const char* name, CommandRouter::Command command,
                 UiIcon icon = UiIcon::NoIcon) {
    new FXMenuCommand(menu, name, ui->icons.get(icon), commands, command);
  };
  fileMenu = new FXMenuPane(this);
  add(fileMenu, "&Open...\tCtrl+O", CommandRouter::Open, UiIcon::Open);
  add(fileMenu, "&Save\tCtrl+S", CommandRouter::Save, UiIcon::Save);
  add(fileMenu, "Save &As...\tCtrl+Shift+S", CommandRouter::SaveAs);
  add(fileMenu, "Export &PDF...\tCtrl+Shift+E", CommandRouter::ExportPdf, UiIcon::Pdf);
  add(fileMenu, "Cancel PDF export", CommandRouter::CancelExport);
  add(fileMenu, "&Quit\tCtrl+Q", CommandRouter::Close);
  new FXMenuTitle(bar, "&File", nullptr, fileMenu);
  editMenu = new FXMenuPane(this);
  add(editMenu, "&Undo\tCtrl+Z", CommandRouter::Undo);
  add(editMenu, "&Redo\tCtrl+Y", CommandRouter::Redo);
  add(editMenu, "&Find...\tCtrl+F", CommandRouter::Find, UiIcon::Search);
  add(editMenu, "&Preferences...", CommandRouter::Preferences);
  new FXMenuTitle(bar, "&Edit", nullptr, editMenu);
  viewMenu = new FXMenuPane(this);
  add(viewMenu, "&Editor\tCtrl+2", CommandRouter::Editor, UiIcon::Editor);
  add(viewMenu, "&Split view\tCtrl+3", CommandRouter::Split, UiIcon::Split);
  add(viewMenu, "&Preview\tCtrl+1", CommandRouter::Preview, UiIcon::Preview);
  add(viewMenu, "&Sidebar\tF10", CommandRouter::Sidebar, UiIcon::Sidebar);
  new FXMenuSeparator(viewMenu);
  new FXMenuRadio(viewMenu, "Window &wrap", commands, CommandRouter::WindowWrap);
  new FXMenuRadio(viewMenu, "&A4 page preview", commands, CommandRouter::A4);
  new FXMenuSeparator(viewMenu);
  new FXMenuRadio(viewMenu, "Fit page &width", commands, CommandRouter::FitWidth);
  new FXMenuRadio(viewMenu, "Actual size (100%)", commands, CommandRouter::ActualSize);
  new FXMenuCheck(viewMenu, "Full &Screen\tF11", commands, CommandRouter::FullScreen);
  new FXMenuSeparator(viewMenu);
  new FXMenuCheck(viewMenu, "&Dark appearance", commands, CommandRouter::ToggleTheme);
  new FXMenuTitle(bar, "&View", nullptr, viewMenu);
  goMenu = new FXMenuPane(this);
  add(goMenu, "&Back\tAlt+Left", CommandRouter::Back, UiIcon::Back);
  add(goMenu, "&Forward\tAlt+Right", CommandRouter::Forward, UiIcon::Forward);
  new FXMenuTitle(bar, "&Go", nullptr, goMenu);
  buildToolbar();
  auto* pathRow =
      new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 6, 6, 3, 3);
  dirtyLabel = new FXLabel(pathRow, "Path", nullptr, LAYOUT_CENTER_Y);
  documentPath = new DocumentPathField(pathRow);
  new FXButton(pathRow, "×\tClear filename filter", nullptr, documentPath,
               DocumentPathField::ID_CLEAR, BUTTON_NORMAL | LAYOUT_CENTER_Y);
  auto* statusRow =
      new FXHorizontalFrame(this, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
  versionStatus = new FXLabel(statusRow, buildVersion(), nullptr, LAYOUT_RIGHT | JUSTIFY_RIGHT);
  status = new FXLabel(statusRow, "Open a local Markdown or text file.", nullptr,
                       LAYOUT_FILL_X | JUSTIFY_LEFT);
  auto* workspace =
      new FXSplitter(this, SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  workspacePanel = new WorkspacePanel(workspace, *ui);
  sidebar = workspacePanel->tree;
  split = new FXSplitter(workspace,
                         SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  navigationArea = new FXVerticalFrame(split, LAYOUT_FILL_Y, 0, 0, 330, 0, 0, 0, 0, 0);
  navigationArea->hide();
  editor = new EditorWidget(split);
  previewArea = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);

  editor->setWidth(420);
}
long XfmdWindow::onConfigure(FXObject* sender, FXSelector sel, void* data) {
  auto result = FXMainWindow::onConfigure(sender, sel, data);
  layoutToolbar();
  if (configured)
    configured();
  return result;
}
long XfmdWindow::onKeyPress(FXObject* sender, FXSelector sel, void* data) {
  auto* event = static_cast<FXEvent*>(data);
  if (event->code == KEY_F6 && !getApp()->getModalWindow()) {
    documentPath->beginEditing();
    return 1;
  }
  if (event->code == KEY_Escape && !getApp()->getModalWindow() && commands->checked &&
      commands->checked(CommandRouter::FullScreen)) {
    commands->dispatch(this, FXSEL(SEL_COMMAND, CommandRouter::LeaveFullScreen), nullptr);
    return 1;
  }
  return FXMainWindow::onKeyPress(sender, sel, data);
}
long XfmdWindow::onClose(FXObject*, FXSelector, void*) {
  commands->dispatch(this, FXSEL(SEL_COMMAND, CommandRouter::Close), nullptr);
  return 1;
}
} // namespace xfmd
