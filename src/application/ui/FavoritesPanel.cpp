#include "FavoritesPanel.h"
#include "OpenPathDialog.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include "application/workspace/WorkPathHistory.h"
#include <algorithm>
#include <filesystem>
#include <fxkeys.h>
#include <stdexcept>
using namespace FX;
namespace fs = std::filesystem;
namespace xfmd {
namespace {
constexpr int maxFavorites = 512;
class FavoriteList : public FXList {
  bool navigating = false;

public:
  bool keyboardNavigation() const { return navigating; }
  long handle(FXObject* sender, FXSelector selector, void* data) override {
    if (FXSELTYPE(selector) != SEL_KEYPRESS)
      return FXList::handle(sender, selector, data);
    const auto* event = static_cast<FXEvent*>(data);
    if (!(event->state & (CONTROLMASK | SHIFTMASK | ALTMASK)) &&
        (event->code == KEY_Return || event->code == KEY_KP_Enter || event->code == KEY_space))
      return getTarget()->tryHandle(this, FXSEL(SEL_COMMAND, getSelector()), nullptr);
    // FOX emits SEL_COMMAND for arrow/Home/End selection too. Let those keys
    // select for Remove without opening the target; Enter/Space activate above.
    navigating = true;
    auto result = FXList::handle(sender, selector, data);
    navigating = false;
    return result;
  }
  FavoriteList(FXComposite* parent, FXObject* target, FXSelector selector)
      : FXList(parent, target, selector, LIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y) {
    horizontal = FoxWheelScrollBar::replace(horizontal);
    vertical = FoxWheelScrollBar::replace(vertical);
  }
};
class FavoriteSeparator : public FXListItem {
public:
  FavoriteSeparator() : FXListItem("") { setEnabled(false); }
  FXint getHeight(const FXList*) const override { return 9; }
  void draw(const FXList* list, FXDC& dc, FXint x, FXint y, FXint w, FXint h) override {
    dc.setForeground(list->getBackColor());
    dc.fillRectangle(x, y, w, h);
    dc.setForeground(list->getTextColor());
    dc.drawLine(x + 6, y + h / 2, x + w - 6, y + h / 2);
  }
};
std::string basename(const std::string& path) {
  auto name = fs::path(path).filename().string();
  return name.empty() ? path : name;
}
} // namespace
FXDEFMAP(FavoritesPanel)
favoritesMap[] = {FXMAPFUNC(SEL_COMMAND, FavoritesPanel::ID_ADD, FavoritesPanel::onAdd),
                  FXMAPFUNC(SEL_COMMAND, FavoritesPanel::ID_REMOVE, FavoritesPanel::onRemove),
                  FXMAPFUNC(SEL_COMMAND, FavoritesPanel::ID_SELECT, FavoritesPanel::onSelect),
                  FXMAPFUNC(SEL_CHANGED, FavoritesPanel::ID_SELECT, FavoritesPanel::onChanged),
                  FXMAPFUNC(SEL_TIMEOUT, FavoritesPanel::ID_ACTIVATE, FavoritesPanel::onActivate)};
FXIMPLEMENT(FavoritesPanel, FXVerticalFrame, favoritesMap, ARRAYNUMBER(favoritesMap))
FavoritesPanel::FavoritesPanel(FXComposite* parent, UiContext& context)
    : FXVerticalFrame(parent, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 120, 0, 0, 0, 0) {
  UiFactory ui(context);
  auto* actions = new FXHorizontalFrame(this, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
  addButton = ui.button(actions, "Add…\tAdd a favorite file or folder", this, ID_ADD);
  removeButton =
      ui.button(actions, "Remove\tRemove favorite (keep the file or folder)", this, ID_REMOVE);
  empty = new FXLabel(this, "No favorites — use Add…", nullptr, LAYOUT_FILL_X | JUSTIFY_LEFT);
  list = new FavoriteList(this, this, ID_SELECT);
  list->setNumVisible(4);
  auto& reg = getApp()->reg();
  const int count = std::clamp(reg.readIntEntry("Favorites", "Count", 0), 0, maxFavorites);
  for (int i = 0; i < count; ++i) {
    const auto suffix = std::to_string(i);
    std::string path = reg.readStringEntry("Favorites", ("Path" + suffix).c_str(), "");
    std::string kind = reg.readStringEntry("Favorites", ("Kind" + suffix).c_str(), "");
    if (!fs::path(path).is_absolute() || (kind != "folder" && kind != "file"))
      continue;
    path = fs::path(path).lexically_normal().string();
    if (std::none_of(paths.begin(), paths.end(), [&](const auto& e) { return e.path == path; }))
      paths.push_back({path, kind == "folder"});
  }
  refresh();
}
FavoritesPanel::~FavoritesPanel() { getApp()->removeTimeout(this, ID_ACTIVATE); }
void FavoritesPanel::report(const std::string& message) {
  list->setHelpText(message.c_str());
  if (feedback)
    feedback(message);
}
void FavoritesPanel::refresh() {
  std::sort(paths.begin(), paths.end(), [](const auto& a, const auto& b) {
    if (a.folder != b.folder)
      return a.folder;
    const auto order = comparecase(basename(a.path).c_str(), basename(b.path).c_str());
    return order == 0 ? a.path < b.path : order < 0;
  });
  list->clearItems();
  rows.clear();
  WorkPathHistory labels(FXSystem::getHomeDirectory().text());
  for (int i = 0; i < int(paths.size()); ++i) {
    if (i && paths[i - 1].folder && !paths[i].folder) {
      list->appendItem(new FavoriteSeparator);
      rows.push_back(-1);
    }
    const auto& entry = paths[i];
    auto label =
        basename(entry.path) + " — " + labels.displayPath(fs::path(entry.path).parent_path());
    list->appendItem(label.c_str());
    rows.push_back(i);
  }
  list->setCurrentItem(-1);
  removeButton->disable();
  if (paths.empty())
    empty->show();
  else
    empty->hide();
  recalc();
}
void FavoritesPanel::save() {
  auto& reg = getApp()->reg();
  reg.deleteSection("Favorites");
  reg.writeIntEntry("Favorites", "Count", int(paths.size()));
  for (int i = 0; i < int(paths.size()); ++i) {
    const auto suffix = std::to_string(i);
    reg.writeStringEntry("Favorites", ("Path" + suffix).c_str(), paths[i].path.c_str());
    reg.writeStringEntry("Favorites", ("Kind" + suffix).c_str(),
                         paths[i].folder ? "folder" : "file");
  }
  report(reg.write() ? ""
                     : "Could not save favorites; changes are available for this session only.");
}
bool FavoritesPanel::add(const std::string& path) {
  try {
    const auto target = fs::canonical(path);
    const auto status = fs::status(target);
    if (!fs::is_directory(status) && !fs::is_regular_file(status))
      throw std::runtime_error("Choose a regular file or folder.");
    if (std::any_of(paths.begin(), paths.end(), [&](const auto& e) { return e.path == target; })) {
      report("Already in Favorites.");
      return false;
    }
    if (paths.size() >= maxFavorites)
      throw std::runtime_error("Favorites is full (512 entries). Remove an entry first.");
    paths.push_back({target.string(), fs::is_directory(status)});
    refresh();
    save();
    return true;
  } catch (const std::exception& e) {
    report(std::string("Cannot add favorite: ") + e.what());
    return false;
  }
}
int FavoritesPanel::selected() const {
  const auto row = list->getCurrentItem();
  return row >= 0 && row < int(rows.size()) ? rows[row] : -1;
}
long FavoritesPanel::onAdd(FXObject*, FXSelector, void*) {
  const auto directory =
      currentDirectory ? currentDirectory() : FXSystem::getHomeDirectory().text();
  OpenPathDialog dialog(this, directory, "Add favorite", "Add current &folder");
  if (dialog.execute(PLACEMENT_OWNER))
    add(dialog.selectedPath().text());
  return 1;
}
long FavoritesPanel::onRemove(FXObject*, FXSelector, void*) {
  const auto index = selected();
  if (index >= 0) {
    getApp()->removeTimeout(this, ID_ACTIVATE);
    pending = {};
    paths.erase(paths.begin() + index);
    refresh();
    save();
  }
  return 1;
}
long FavoritesPanel::onChanged(FXObject*, FXSelector, void*) {
  if (selected() >= 0)
    removeButton->enable();
  else
    removeButton->disable();
  return 1;
}
long FavoritesPanel::onSelect(FXObject*, FXSelector, void*) {
  if (static_cast<FavoriteList*>(list)->keyboardNavigation())
    return 1;
  getApp()->removeTimeout(this, ID_ACTIVATE);
  pending = {};
  const auto index = selected();
  if (index >= 0) {
    removeButton->enable();
    pending = paths[index];
    // Navigation or a dirty prompt must run after native list dispatch finishes.
    getApp()->addTimeout(this, ID_ACTIVATE, 0);
  } else
    removeButton->disable();
  return 1;
}
long FavoritesPanel::onActivate(FXObject*, FXSelector, void*) {
  const auto entry = std::move(pending);
  pending = {};
  if (entry.path.empty())
    return 1;
  std::error_code error;
  const auto status = fs::status(entry.path, error);
  if (error || (entry.folder ? !fs::is_directory(status) : !fs::is_regular_file(status)))
    report("Cannot open favorite: target is missing, inaccessible or has changed type. " +
           entry.path);
  else if (open) {
    report("");
    open(entry);
  }
  return 1;
}
} // namespace xfmd
