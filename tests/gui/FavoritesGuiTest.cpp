#include "application/Application.h"
#include "support/PathMenus.h"
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void run(bool restart) {
  const fs::path home = FXSystem::getHomeDirectory().text();
  const auto alpha = home / "Alpha folder", zebra = home / "zebra";
  const auto first = home / "alpha.md", second = home / "Zebra ø.txt";
  if (!restart) {
    fs::create_directory(alpha);
    fs::create_directory(zebra);
    std::ofstream(first) << "# Alpha\n";
    std::ofstream(second) << "Plain text\n";
  }
  int argc = 1;
  char name[] = "favorites-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  auto* panel = app.window->workspacePanel;
  auto* favorites = panel->favorites;
  auto* list = favorites->list;
  auto tab = [&](int index) {
    auto* item = panel->recentTabs->getFirst();
    for (int i = 0; i < index; ++i)
      item = item->getNext()->getNext();
    nativeClick(item, item->getWidth() / 2, 8);
    CHECK(panel->recentTabs->getCurrent() == index);
  };
  settleNative(app.app);
  tab(2);
  CHECK(favorites->getFirst() == list && favorites->getLast() == list); // No buttons/hint.
  if (restart) {
    CHECK(favorites->entries().size() == 4);
    CHECK(favorites->entries()[0].path == alpha && favorites->entries()[0].folder);
    CHECK(favorites->entries()[1].path == zebra && favorites->entries()[1].folder);
    CHECK(favorites->entries()[2].path == first && !favorites->entries()[2].folder);
    CHECK(favorites->entries()[3].path == second && !favorites->entries()[3].folder);
    CHECK(list->getNumItems() == 5 && !list->isItemEnabled(2));
    CHECK(app.session.view().path.empty());
    const auto root = panel->history.root();
    // Remove a missing, unselected favorite without trying to open it.
    list->setCurrentItem(0);
    listMenu(app, list, 4, "Remove from Favorites");
    CHECK(favorites->entries().size() == 3);
    CHECK(app.session.view().path.empty() && panel->history.root() == root);
    clickRow(list, 3);
    CHECK(app.session.view().path == first);
    return;
  }
  CHECK(favorites->entries().empty() && list->getNumItems() == 0);
  nativeClick(list, 20, 20, 0, Button3);
  CHECK(!app.app.getPopupWindow());
  // Seed both histories, then perform every context action with an unsaved buffer.
  CHECK(app.open(first.string()));
  CHECK(app.open(second.string()));
  CHECK(panel->setWorkPath(alpha.string()));
  CHECK(panel->setWorkPath(zebra.string()));
  CHECK(panel->setWorkPath(home.string()));
  app.edits.applyEdit({0, 0, "dirty "});
  const auto initial = app.session.snapshot();
  const auto roots = panel->history.entries();
  const auto recent = panel->recentFiles->entries();
  app.documents.chooseUnsaved = []() -> UnsavedChoice {
    throw std::runtime_error("Context actions must not open a dirty prompt");
  };
  auto unchanged = [&] {
    CHECK(app.session.view().token == initial.token && app.session.dirty() && app.edits.canUndo());
    CHECK(panel->history.entries() == roots && panel->recentFiles->entries() == recent);
  };
  treeMenu(app, first, ""); // Escape does not add.
  CHECK(favorites->entries().empty());
  treeMenu(app, second, "Add to Favorites", "tree-file-menu.png");
  CHECK(list->getNumItems() == 1); // No separator for files only.
  treeMenu(app, zebra, "Add to Favorites", "tree-folder-menu.png");
  unchanged();
  tab(1);
  panel->recentFiles->list->setCurrentItem(0); // second is selected; first is clicked.
  listMenu(app, panel->recentFiles->list, 1, "Add to Favorites", "file-history-menu.png");
  unchanged();
  tab(0);
  panel->workPaths->setCurrentItem(0); // home selected, alpha clicked.
  listMenu(app, panel->workPaths, 2, "Add to Favorites", "folder-history-menu.png");
  unchanged();
  tab(2);
  CHECK(favorites->entries().size() == 4);
  CHECK(favorites->entries()[0].path == alpha && favorites->entries()[1].path == zebra);
  CHECK(favorites->entries()[2].path == first && favorites->entries()[3].path == second);
  CHECK(list->getItemText(0) == "Alpha folder — ~" && list->getItemText(4) == "Zebra ø.txt — ~");
  CHECK(list->getNumItems() == 5 && !list->isItemEnabled(2));
  nativeClick(list, 20, listRowY(list, 2), 0, Button3); // separator
  CHECK(!app.app.getPopupWindow() && favorites->entries().size() == 4);
  nativeClick(list, 20, list->getViewportHeight() - 5, 0, Button3); // blank area
  CHECK(!app.app.getPopupWindow() && favorites->entries().size() == 4);
  listMenu(app, list, 0, ""); // canceled removal
  unchanged();
  CHECK(favorites->entries().size() == 4);
  treeMenu(app, first, "Add to Favorites"); // duplicate context action
  CHECK(favorites->entries().size() == 4);
  fs::create_symlink(first, home / "alias.md");
  CHECK(!favorites->add((home / "alias.md").string()));
  CHECK(!favorites->add((home / "missing").string()));
  // Same basename, deterministic full-path tie-break; remove the clicked rather
  // than selected row without opening or deleting it.
  std::ofstream(alpha / "alpha.md") << "# Nested\n";
  CHECK(favorites->add((alpha / "alpha.md").string()));
  CHECK(favorites->entries()[2].path == alpha / "alpha.md");
  list->setFocus();
  nativeKey(list, XK_Home);
  unchanged();
  listMenu(app, list, 3, "Remove from Favorites", "favorites-remove-menu.png");
  CHECK(fs::exists(alpha / "alpha.md") && favorites->entries().size() == 4);
  unchanged();
  // Root can also be favorited; removing a folder never deletes it.
  treeMenu(app, home, "Add to Favorites");
  CHECK(favorites->entries().size() == 5);
  int homeRow = 0;
  while (favorites->entries()[homeRow].path != home)
    ++homeRow;
  listMenu(app, list, homeRow, "Remove from Favorites");
  CHECK(fs::is_directory(home) && favorites->entries().size() == 4);
  unchanged();
  // Normal activation still uses the existing folder/file and dirty policies.
  clickRow(list, 0);
  CHECK(panel->history.root() == alpha && app.session.view().token == initial.token);
  const auto folderRoots = panel->history.entries();
  clickRow(list, 2);
  nativeKey(list, XK_Return);
  CHECK(panel->history.entries() == folderRoots && app.session.view().token == initial.token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  clickRow(list, 3);
  CHECK(app.session.view().token == initial.token && app.session.dirty());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  nativeKey(list, XK_Return);
  CHECK(app.session.view().path == first && panel->history.root() == alpha);
  // Changed types cannot accidentally change the work root.
  fs::remove(second);
  fs::create_directory(second);
  const auto token = app.session.view().token;
  clickRow(list, 4);
  CHECK(app.session.view().token == token && panel->history.root() == alpha);
  CHECK(!list->getHelpText().empty());
  fs::remove(second);
  std::ofstream(second) << "Plain text\n";
  CHECK(panel->setWorkPath(home.string()));
  app.views->setMode(ViewMode::Preview);
  if (const auto* output = getenv("XFMD_UI_EVIDENCE")) {
    settleNative(app.app);
    captureDesktop(app.app, (fs::path(output) / "favorites-light.png").c_str());
    auto draft = app.preferences->begin();
    draft.appearance.theme = "dark";
    std::string error;
    CHECK(app.preferences->commit(draft, error));
    settleNative(app.app);
    captureDesktop(app.app, (fs::path(output) / "favorites-dark.png").c_str());
  }
  fs::remove(second);
  clickRow(list, 4);
  CHECK(app.session.view().path == first && panel->history.root() == home);
  CHECK(favorites->entries().size() == 4 && !list->getHelpText().empty());
  list->setCurrentItem(0);
  favorites->onSelect(nullptr, 0, nullptr); // Pending timer teardown under ASan.
}
int main(int argc, char**) {
  try {
    run(argc > 1);
    if (argc == 1) {
      const auto child = fork();
      CHECK(child >= 0);
      if (child == 0) {
        execl("/proc/self/exe", "FavoritesGuiTest", "restart", nullptr);
        _exit(127);
      }
      int status;
      CHECK(waitpid(child, &status, 0) == child);
      CHECK(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    std::cout << "PASS\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
