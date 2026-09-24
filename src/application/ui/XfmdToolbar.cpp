#include "XfmdWindow.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
void XfmdWindow::buildToolbar() {
  UiFactory factory(*ui);
  auto group = [&] {
    auto* row = factory.row(toolbar);
    row->setLayoutHints(LAYOUT_CENTER_Y);
    row->setPadLeft(0);
    row->setPadRight(0);
    row->setPadTop(0);
    row->setPadBottom(0);
    row->setHSpacing(1);
    return row;
  };
  factory.button(toolbar, "\tSidebar (F10)", commands, CommandRouter::Sidebar, UiIcon::Sidebar);
  fileTools = group();
  factory.button(fileTools, "\tOpen file or folder (Ctrl+O)", commands, CommandRouter::Open,
                 UiIcon::Open);
  factory.button(fileTools, "\tSave (Ctrl+S)", commands, CommandRouter::Save, UiIcon::Save);
  viewTools = group();
  factory.button(viewTools, "\tEditor (Ctrl+2)", commands, CommandRouter::Editor, UiIcon::Editor);
  factory.button(viewTools, "\tSplit view (Ctrl+3)", commands, CommandRouter::Split, UiIcon::Split);
  factory.button(viewTools, "\tPreview (Ctrl+1)", commands, CommandRouter::Preview,
                 UiIcon::Preview);
  viewTools->setPadLeft(8);
  viewTools->setPadRight(8);
  navTools = group();
  factory.button(navTools, "\tBack (Alt+Left)", commands, CommandRouter::Back, UiIcon::Back);
  factory.button(navTools, "\tForward (Alt+Right)", commands, CommandRouter::Forward,
                 UiIcon::Forward);
  previewControls = new PreviewControls(toolbar, *ui, *commands);
  colorPopup = new ReadingColorPopup(this, *ui);
  previewColors = colorPopup->controls;
  themeButton = new ThemeButton(toolbar, *ui, commands, CommandRouter::ToggleTheme, colorPopup);
  toolbar->rightAligned = themeButton;
}
void XfmdWindow::layoutToolbar() {
  if (!toolbar)
    return;
  toolbar->recalc();
}
void XfmdWindow::setDocumentLabel(const std::string& path, bool dirty) {
  documentPath->setDocument(path);
  dirtyLabel->setText(dirty ? "Path *" : "Path");
}
void XfmdWindow::restyle() {
  std::function<void(FXWindow*)> compact = [&](FXWindow* item) {
    if (auto* button = dynamic_cast<UiButton*>(item))
      button->setCompact(true);
    for (auto* child = item->getFirst(); child; child = child->getNext())
      compact(child);
  };
  compact(toolbar);
  themeButton->setTipText(ui->appearance().theme == "dark"
                              ? "Dark — switch to Light; right-click or Shift+F10 for colors"
                              : "Light — switch to Dark; right-click or Shift+F10 for colors");
  for (auto* group : {fileTools, navTools, viewTools}) {
    group->setPadLeft(0);
    group->setPadRight(0);
    group->setPadTop(0);
    group->setPadBottom(0);
    group->setHSpacing(1);
  }
  viewTools->setPadLeft(8);
  viewTools->setPadRight(8);
  previewControls->compact();
  layoutToolbar();
}
} // namespace xfmd
