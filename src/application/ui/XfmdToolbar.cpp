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
  fileTools = group();
  factory.button(fileTools, "\tOpen file (Ctrl+O)", commands, CommandRouter::Open, UiIcon::Open);
  factory.button(fileTools, "\tSave (Ctrl+S)", commands, CommandRouter::Save, UiIcon::Save);
  navTools = group();
  factory.button(navTools, "\tBack (Alt+Left)", commands, CommandRouter::Back, UiIcon::Back);
  factory.button(navTools, "\tForward (Alt+Right)", commands, CommandRouter::Forward,
                 UiIcon::Forward);
  factory.button(navTools, "\tSidebar (F10)", commands, CommandRouter::Sidebar, UiIcon::Sidebar);
  documentTitle = new FXLabel(toolbar, "", nullptr,
                              LAYOUT_FIX_WIDTH | LAYOUT_CENTER_Y | JUSTIFY_LEFT, 0, 0, 130);
  viewTools = group();
  factory.button(viewTools, "\tEditor (Ctrl+2)", commands, CommandRouter::Editor, UiIcon::Editor);
  factory.button(viewTools, "\tSplit view (Ctrl+3)", commands, CommandRouter::Split, UiIcon::Split);
  factory.button(viewTools, "\tPreview (Ctrl+1)", commands, CommandRouter::Preview,
                 UiIcon::Preview);
  themeButton = factory.button(toolbar, "\tToggle Light / Dark appearance", commands,
                               CommandRouter::ToggleTheme, UiIcon::Theme);
  previewControls = new PreviewControls(toolbar, *ui, *commands);
  previewColors = new PreviewColorControls(toolbar, *ui);
}
void XfmdWindow::layoutToolbar() {
  if (!toolbar)
    return;
  int required = 4 + documentTitle->getWidth() + 4;
  for (auto* c = toolbar->getFirst(); c; c = c->getNext())
    if (c != documentTitle && c->shown())
      required +=
          ((c->getLayoutHints() & LAYOUT_FIX_WIDTH) ? c->getWidth() : c->getDefaultWidth()) + 4;
  if (getWidth() < required)
    documentTitle->hide();
  else
    documentTitle->show();
  toolbar->recalc();
}
void XfmdWindow::setDocumentLabel(const std::string& path, bool dirty) {
  std::string label = path.empty() ? "Untitled" : std::filesystem::path(path).filename().string();
  documentTitle->setText(((dirty ? "* " : "") + label).c_str());
  documentTitle->setTipText(path.c_str());
}
void XfmdWindow::restyle() {
  std::function<void(FXWindow*)> compact = [&](FXWindow* item) {
    if (auto* button = dynamic_cast<UiButton*>(item))
      button->setCompact(true);
    for (auto* child = item->getFirst(); child; child = child->getNext())
      compact(child);
  };
  compact(toolbar);
  themeButton->setTipText(ui->appearance().theme == "dark" ? "Dark — switch to Light"
                                                           : "Light — switch to Dark");
  for (auto* group : {fileTools, navTools, viewTools}) {
    group->setPadLeft(0);
    group->setPadRight(0);
    group->setPadTop(0);
    group->setPadBottom(0);
    group->setHSpacing(1);
  }
  previewControls->compact();
  layoutToolbar();
}
} // namespace xfmd
