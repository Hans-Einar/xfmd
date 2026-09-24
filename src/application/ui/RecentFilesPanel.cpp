#include "RecentFilesPanel.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include "application/workspace/WorkPathHistory.h"
#include <algorithm>
#include <filesystem>
using namespace FX;
namespace xfmd {
namespace {
class RecentFileList : public FXList {
public:
  RecentFileList(FXComposite* parent, FXObject* target, FXSelector selector)
      : FXList(parent, target, selector, LIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
    horizontal = FoxWheelScrollBar::replace(horizontal);
    vertical = FoxWheelScrollBar::replace(vertical);
  }
};
} // namespace
FXDEFMAP(RecentFilesPanel)
recentMap[] = {FXMAPFUNC(SEL_COMMAND, RecentFilesPanel::ID_SELECT, RecentFilesPanel::onSelect),
               FXMAPFUNC(SEL_TIMEOUT, RecentFilesPanel::ID_ACTIVATE, RecentFilesPanel::onActivate)};
FXIMPLEMENT(RecentFilesPanel, FXVerticalFrame, recentMap, ARRAYNUMBER(recentMap))
RecentFilesPanel::RecentFilesPanel(FXComposite* parent, UiContext&)
    : FXVerticalFrame(parent, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 120, 0, 0, 0, 0) {

  empty = new FXLabel(this, "No recent files", nullptr, LAYOUT_FILL_X | JUSTIFY_LEFT);
  list = new RecentFileList(this, this, ID_SELECT);
  list->setNumVisible(4);
  for (int i = 0; i < 32; ++i) {
    auto key = "Path" + std::to_string(i);
    std::string path = getApp()->reg().readStringEntry("RecentFiles", key.c_str(), "");
    if (std::filesystem::path(path).is_absolute() &&
        std::find(paths.begin(), paths.end(), path) == paths.end())
      paths.push_back(path);
  }
  refresh();
}
RecentFilesPanel::~RecentFilesPanel() { getApp()->removeTimeout(this, ID_ACTIVATE); }
void RecentFilesPanel::refresh() {
  if (paths.empty())
    empty->show();
  else
    empty->hide();
  WorkPathHistory labels(FXSystem::getHomeDirectory().text());
  list->clearItems();
  for (const auto& path : paths) {
    std::filesystem::path file(path);
    auto label = file.filename().string() + " — " + labels.displayPath(file.parent_path());
    list->appendItem(label.c_str());
  }
}
void RecentFilesPanel::remember(const std::string& path) {
  if (!std::filesystem::path(path).is_absolute())
    return;
  paths.erase(std::remove(paths.begin(), paths.end(), path), paths.end());
  paths.insert(paths.begin(), path);
  if (paths.size() > 32)
    paths.resize(32);
  refresh();
  list->setCurrentItem(0);
  list->makeItemVisible(0);
  for (int i = 0; i < 32; ++i) {
    auto key = "Path" + std::to_string(i);
    getApp()->reg().writeStringEntry("RecentFiles", key.c_str(),
                                     i < int(paths.size()) ? paths[i].c_str() : "");
  }
  if (!getApp()->reg().write())
    list->setHelpText("Could not save recent files; history is available for this session only.");
  else
    list->setHelpText("");
}
long RecentFilesPanel::onSelect(FXObject*, FXSelector, void*) {
  int index = list->getCurrentItem();
  if (index >= 0 && index < int(paths.size())) {
    pending = paths[index];
    // Opening may rebuild this list or show a modal dialog: finish FOX dispatch first.
    getApp()->addTimeout(this, ID_ACTIVATE, 0);
  }
  return 1;
}
long RecentFilesPanel::onActivate(FXObject*, FXSelector, void*) {
  auto path = std::move(pending);
  pending.clear();
  if (!path.empty() && open)
    open(path);
  return 1;
}
} // namespace xfmd
