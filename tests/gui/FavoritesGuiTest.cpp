#include "application/Application.h"
#include "application/ui/OpenPathDialog.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
FXButton* button(FXWindow* parent, const std::string& text) {
  for (auto* child = parent->getFirst(); child; child = child->getNext()) {
    if (auto* b = dynamic_cast<FXButton*>(child); b && b->getText().text() == text)
      return b;
    if (auto* b = button(child, text))
      return b;
  }
  return nullptr;
}
struct ChooserDriver : FXObject {
  Application& app;
  std::string path, action;
  bool visited = false;
  explicit ChooserDriver(Application& a) : app(a) {}
  long handle(FXObject*, FXSelector, void*) override {
    auto* dialog = dynamic_cast<OpenPathDialog*>(app.app.getModalWindow());
    CHECK(dialog && dialog->getTitle() == "Add favorite");
    CHECK(dialog->getDirectory().text() == app.window->workspacePanel->history.root().string());
    if (!path.empty())
      dialog->setFilename(path.c_str());
    auto* accept = button(dialog, action);
    CHECK(accept);
    nativeClick(accept, accept->getWidth() / 2, accept->getHeight() / 2, 0, Button1, false);
    visited = true;
    return 1;
  }
};
void clickRow(FXList* list, int row) {
  list->makeItemVisible(row);
  settleNative(*list->getApp());
  int y = list->getYPosition() + list->getItemHeight(row) / 2;
  for (int i = 0; i < row; ++i)
    y += list->getItemHeight(i);
  nativeClick(list, 20, y);
}
void run(bool restart) {
  const fs::path home = FXSystem::getHomeDirectory().text();
  const auto alpha = home / "Alpha folder";
  const auto zebra = home / "zebra";
  const auto first = home / "alpha.md";
  const auto second = home / "Zebra ø.txt";
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
  settleNative(app.app);
  auto* tab = panel->recentTabs->getLast()->getPrev();
  nativeClick(tab, tab->getWidth() / 2, 8);
  CHECK(panel->recentTabs->getCurrent() == 2);
  if (restart) {
    CHECK(favorites->entries().size() == 4);
    CHECK(favorites->entries()[0].path == alpha && favorites->entries()[0].folder);
    CHECK(favorites->entries()[1].path == zebra && favorites->entries()[1].folder);
    CHECK(favorites->entries()[2].path == first && !favorites->entries()[2].folder);
    CHECK(favorites->entries()[3].path == second && !favorites->entries()[3].folder);
    CHECK(list->getNumItems() == 5 && !list->isItemEnabled(2));
    CHECK(app.session.view().path.empty());
    // Missing favorites survive restart, remain removable, and never navigate.
    const auto root = panel->history.root();
    clickRow(list, 4);
    CHECK(app.session.view().path.empty() && panel->history.root() == root);
    CHECK(!favorites->list->getHelpText().empty());
    nativeClick(favorites->removeButton, 20, 10);
    CHECK(favorites->entries().size() == 3);
    clickRow(list, 3);
    CHECK(app.session.view().path == first);
    return;
  }
  CHECK(favorites->entries().empty() && list->getNumItems() == 0);
  CHECK(!favorites->removeButton->isEnabled());
  CHECK(panel->setWorkPath(home.string()));
  auto choose = [&](const fs::path& path, const std::string& action) {
    ChooserDriver driver(app);
    driver.path = path.string();
    driver.action = action;
    app.app.addTimeout(&driver, 1, 80);
    nativeClick(favorites->addButton, 20, 10);
    CHECK(driver.visited);
    settleNative(app.app);
  };
  choose(first, "Cancel");
  CHECK(favorites->entries().empty());
  choose(second, "OK");
  CHECK(list->getNumItems() == 1); // No separator for files alone.
  choose(zebra, "OK");
  choose(first, "OK");
  choose(alpha, "OK");
  CHECK(app.session.view().path.empty() && panel->history.root() == home);
  CHECK(favorites->entries().size() == 4);
  CHECK(favorites->entries()[0].path == alpha && favorites->entries()[1].path == zebra);
  CHECK(favorites->entries()[2].path == first && favorites->entries()[3].path == second);
  CHECK(list->getItemText(0) == "Alpha folder — ~");
  CHECK(list->getItemText(4) == "Zebra ø.txt — ~");
  CHECK(list->getNumItems() == 5 && !list->isItemEnabled(2));
  fs::create_symlink(first, home / "alias.md");
  CHECK(!favorites->add((home / "alias.md").string()));
  CHECK(!favorites->add(first.string()));
  CHECK(!favorites->add((home / "missing").string()));
  CHECK(favorites->entries().size() == 4);
  // Identical basenames remain distinct and have deterministic path ordering.
  std::ofstream(alpha / "alpha.md") << "# Nested\n";
  CHECK(favorites->add((alpha / "alpha.md").string()));
  CHECK(favorites->entries()[2].path == alpha / "alpha.md");
  list->setFocus();
  nativeKey(list, XK_Home);
  CHECK(favorites->removeButton->isEnabled());
  CHECK(app.session.view().path.empty() && panel->history.root() == home); // Selection only.

  clickRow(list, 3);
  CHECK(app.session.view().path == alpha / "alpha.md");
  nativeClick(favorites->removeButton, 20, 10);
  CHECK(fs::exists(alpha / "alpha.md") && favorites->entries().size() == 4);
  choose({}, "Add current folder");
  CHECK(favorites->entries().size() == 5);
  int homeRow = 0;
  while (favorites->entries()[homeRow].path != home)
    ++homeRow;
  clickRow(list, homeRow);
  nativeClick(favorites->removeButton, 20, 10);
  CHECK(fs::is_directory(home) && favorites->entries().size() == 4);
  clickRow(list, 3);
  CHECK(app.session.view().path == first);
  app.edits.applyEdit({0, 0, "dirty "});
  const auto dirty = app.session.snapshot();
  app.documents.chooseUnsaved = []() -> UnsavedChoice {
    throw std::runtime_error("Folder activation must not prompt for unsaved edits");
  };
  clickRow(list, 0);
  CHECK(panel->history.root() == alpha && app.session.view().token == dirty.token);
  CHECK(app.session.dirty() && app.edits.canUndo());
  auto roots = panel->history.entries();
  // Separator mouse/Enter cannot invoke either route.
  clickRow(list, 2);
  nativeKey(list, XK_Return);
  CHECK(panel->history.entries() == roots && app.session.view().token == dirty.token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  clickRow(list, 4);
  CHECK(app.session.view().token == dirty.token && app.session.dirty());
  CHECK(panel->history.entries() == roots);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  nativeKey(list, XK_Return);
  CHECK(app.session.view().path == second && app.session.view().plainText);
  CHECK(panel->history.root() == alpha);
  // A file changed into a folder must not accidentally change work root.
  fs::remove(first);
  fs::create_directory(first);
  auto token = app.session.view().token;
  clickRow(list, 3);
  CHECK(app.session.view().token == token && panel->history.root() == alpha);
  CHECK(!list->getHelpText().empty());
  fs::remove(first);
  std::ofstream(first) << "# Alpha restored\n";
  CHECK(app.open(first.string()));
  CHECK(panel->setWorkPath(home.string()));
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
  // Leave a pending activation at teardown to exercise timer ownership under ASan.
  list->setCurrentItem(0);
  favorites->onSelect(nullptr, 0, nullptr);
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
