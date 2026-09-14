#include "XfmdWindow.h"
#include <filesystem>
using namespace FX;
namespace xfmd {
void XfmdWindow::buildToolbar() {
  UiFactory factory(*ui);
  toolbar = factory.row(this, LAYOUT_SIDE_TOP);
  auto group = [&] {
    auto* row = factory.row(toolbar);
    row->setLayoutHints(LAYOUT_CENTER_Y);
    row->setPadLeft(0);
    row->setPadRight(0);
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
  documentTitle = new FXLabel(toolbar, "", nullptr, LAYOUT_FILL_X | LAYOUT_CENTER_Y | JUSTIFY_LEFT);
  viewTools = group();
  factory.button(viewTools, "Preview\tPreview (Ctrl+1)", commands, CommandRouter::Preview);
  factory.button(viewTools, "Split\tSplit view (Ctrl+3)", commands, CommandRouter::Split);
  factory.button(viewTools, "Editor\tEditor (Ctrl+2)", commands, CommandRouter::Editor);
  themeButton = factory.button(toolbar, "Light\tToggle Light / Dark appearance", commands,
                               CommandRouter::ToggleTheme, UiIcon::Theme);
}
void XfmdWindow::layoutToolbar() {
  if (!toolbar)
    return;
  const auto m = ui->metrics();
  const int base =
      fileTools->getDefaultWidth() + themeButton->getDefaultWidth() + m.inset * 2 + m.gap * 4;
  const int nav = navTools->getDefaultWidth(), views = viewTools->getDefaultWidth();
  if (getWidth() < base + nav + views)
    viewTools->hide();
  else
    viewTools->show();
  if (getWidth() < base + nav)
    navTools->hide();
  else
    navTools->show();
  if (getWidth() < base + (navTools->shown() ? nav : 0) + (viewTools->shown() ? views : 0) + 120)
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
  themeButton->setText(ui->appearance().theme == "dark" ? "Dark" : "Light");
  for (auto* group : {fileTools, navTools, viewTools}) {
    group->setPadLeft(0);
    group->setPadRight(0);
  }
  layoutToolbar();
}
} // namespace xfmd
