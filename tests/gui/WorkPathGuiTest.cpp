#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
using namespace xfmd;
namespace fs = std::filesystem;
void events(Application& app, int n = 80) {
  for (int i = 0; i < n; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void settle(Application& app) {
  events(app);
  for (int i = 0; i < 100 && app.window->sidebar->scanning; ++i)
    events(app, 2);
  CHECK(!app.window->sidebar->scanning);
}
void click(Application& app, FX::FXWindow* window, int x, int y, int button, unsigned long time) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent e{};
  e.xbutton.display = display;
  e.xbutton.window = window->id();
  e.xbutton.root = DefaultRootWindow(display);
  e.xbutton.same_screen = True;
  Window child;
  XTranslateCoordinates(display, window->id(), DefaultRootWindow(display), x, y, &e.xbutton.x_root,
                        &e.xbutton.y_root, &child);
  e.xbutton.x = x;
  e.xbutton.y = y;
  e.xbutton.button = button;
  e.xbutton.time = time;
  e.type = ButtonPress;
  CHECK(XSendEvent(display, window->id(), False, ButtonPressMask, &e));
  e.type = ButtonRelease;
  CHECK(XSendEvent(display, window->id(), False, ButtonReleaseMask, &e));
  XFlush(display);
}
int row(SidebarWidget* tree, FX::FXTreeItem* item) {
  tree->makeItemVisible(item);
  for (int y = 0; y < tree->getViewportHeight(); ++y)
    if (tree->getItemAt(100, y) == item)
      return y + 5;
  throw std::runtime_error("Tree row not visible");
}
struct MenuClick : FX::FXObject {
  Application& app;
  bool clicked = false, positioned = false;
  explicit MenuClick(Application& value) : app(value) {}
  long handle(FX::FXObject*, FX::FXSelector, void*) override {
    auto* popup = app.app.getPopupWindow();
    if (!popup)
      return 1;
    auto* command = dynamic_cast<FX::FXMenuCommand*>(popup->getFirst());
    CHECK(command && command->getText() == "Set work path");
    auto* display = static_cast<Display*>(app.app.getDisplay());
    if (!positioned) {
      XWarpPointer(display, None, command->id(), 0, 0, 0, 0, 15, command->getHeight() / 2);
      XFlush(display);
      positioned = true;
      app.app.addTimeout(this, 1, 100);
      return 1;
    }
    click(app, command, 15, command->getHeight() / 2, Button1, 2000);
    clicked = true;
    return 1;
  }
};
void run() {
  fs::path home = FX::FXSystem::getHomeDirectory().text();
  fs::create_directories(home / "project/deep/nested");
  fs::create_directory(home / "project/empty");
  std::ofstream(home / "startup.md") << "home";
  auto dir = home / "project";
  std::ofstream(dir / "hello.md") << "# Hello";
  std::ofstream(dir / "note.txt") << "text";
  std::ofstream(dir / "photo.png") << "other";
  std::ofstream(dir / "deep/nested/matchø.md") << "# Deep";
  int argc = 1;
  char name[] = "work-path-gui";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.documents.error = [](const std::string& message) { throw std::runtime_error(message); };
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  auto* panel = app.window->workspacePanel;
  auto* tree = panel->tree;
  settle(app);
  CHECK(tree->workRoot() == home);
  CHECK(tree->getFirstItem()->getText() == "~");
  CHECK(tree->getFirstItem() && tree->getFirstItem()->isExpanded());
  CHECK(tree->getPathnameItem((home / "startup.md").c_str()));
  auto cwd = fs::current_path();
  fs::current_path(dir);
  CHECK(app.startPath("."));
  fs::current_path(cwd);
  settle(app);
  CHECK(tree->workRoot() == dir && !tree->getPathnameItem(home.c_str()));
  CHECK(app.startPath((dir / "hello.md").string()));
  settle(app);
  CHECK(app.session.view().path == (dir / "hello.md").string());
  app.edits.applyEdit({0, 0, "dirty "});
  auto token = app.session.view().token;
  auto* folder = tree->getPathnameItem((dir / "deep").c_str());
  CHECK(folder);
  MenuClick menu(app);
  app.app.addTimeout(&menu, 1, 100);
  click(app, tree, 70, row(tree, folder), Button3, 1000);
  events(app);
  settle(app);
  CHECK(menu.clicked && tree->workRoot() == dir / "deep");
  CHECK(app.session.dirty() && app.session.view().token == token && panel->shown());
  CHECK(panel->workPaths->getNumItems() == 3);
  CHECK(panel->workPaths->getItemText(0) == "~/project/deep");
  // Native single click on history restores the previous project root.
  int y = panel->workPaths->getItemHeight(0) + panel->workPaths->getItemHeight(1) / 2;
  click(app, panel->workPaths, 20, y, Button1, 3000);
  settle(app);
  CHECK(tree->workRoot() == dir);
  // Native toggle buttons OR their types; name input then intersects the result.
  click(app, panel->markdown, 10, 10, Button1, 4000);
  settle(app);
  CHECK(panel->markdown->getState());
  CHECK(!tree->getPathnameItem((dir / "note.txt").c_str()));
  CHECK(!tree->getPathnameItem((dir / "empty").c_str()));
  CHECK(tree->getPathnameItem((dir / "deep/nested/matchø.md").c_str()));
  click(app, panel->text, 10, 10, Button1, 5000);
  settle(app);
  CHECK(tree->getPathnameItem((dir / "note.txt").c_str()));
  panel->setNameFilter("match?.*");
  settle(app);
  CHECK(tree->getPathnameItem((dir / "deep/nested/matchø.md").c_str()));
  CHECK(!tree->getPathnameItem((dir / "hello.md").c_str()));
  panel->setNameFilter("nothing");
  settle(app);
  CHECK(!tree->getFirstItem()->getFirst());
  CHECK(panel->searchStatus->getText().find("No matching") >= 0);
  panel->setNameFilter("");
  panel->markdown->setState(false);
  panel->text->setState(false);
  panel->onApplyFilter(nullptr, 0, nullptr);
  settle(app);
  // Native root double clicks widen project -> home -> / without stale node access.
  for (const auto& expected : {home, fs::path("/")}) {
    auto* root = tree->getFirstItem();
    int yroot = row(tree, root);
    click(app, tree, 60, yroot, Button1, 6000);
    events(app, 2);
    click(app, tree, 60, yroot, Button1, 6050);
    settle(app);
    CHECK(tree->workRoot() == expected);
  }
  auto before = panel->history.root();
  CHECK(!panel->setWorkPath((home / "gone").string()));
  events(app);
  CHECK(panel->history.root() == before);
  CHECK(panel->searchStatus->getText().find("Cannot set") >= 0);
  // A disappearing history entry leaves the active root and dirty buffer intact.
  fs::create_directory(home / "removed");
  CHECK(panel->setWorkPath((home / "removed").string()));
  CHECK(panel->setWorkPath(dir.string()));
  fs::remove(home / "removed");
  panel->workPaths->setCurrentItem(1);
  panel->onHistory(nullptr, 0, nullptr);
  settle(app);
  CHECK(tree->workRoot() == dir);
  CHECK(panel->searchStatus->getText().find("Cannot set") >= 0);
  // Pending matches from a previous root must never populate the new tree.
  panel->markdown->setState(true);
  panel->onApplyFilter(nullptr, 0, nullptr);
  CHECK(panel->setWorkPath((dir / "empty").string()));
  settle(app);
  CHECK(!tree->getFirstItem()->getFirst());
  // Registry contents survive a fresh registry reader, and duplicate visits stay unique.
  CHECK(panel->setWorkPath(dir.string()));
  settle(app);
  FX::FXRegistry saved("xfmd", "xfmd");
  CHECK(saved.read());
  CHECK(saved.readStringEntry("WorkPaths", "Path0", "") == dir.string());
  CHECK(panel->history.entries().front() == dir.string());
  // The added history list must retain the same fractional-wheel fix as the tree.
  for (int i = 0; i < 12; ++i) {
    auto path = home / ("history-" + std::to_string(i));
    fs::create_directory(path);
    CHECK(panel->setWorkPath(path.string()));
  }
  settle(app);
  auto* historyBar = panel->workPaths->verticalScrollBar();
  const int maximum = historyBar->getRange() - historyBar->getPage();
  CHECK(maximum > 20);
  for (bool bottom : {false, true}) {
    panel->workPaths->setPosition(0, -(bottom ? maximum - 7 : 7));
    for (int i = 0; i < 120; ++i) {
      FX::FXEvent event{};
      event.code = bottom ? -1 : 1;
      panel->workPaths->handle(panel->workPaths, FXSEL(FX::SEL_MOUSEWHEEL, 0), &event);
      events(app, 1);
    }
    events(app);
    CHECK(historyBar->getPosition() == (bottom ? maximum : 0));
    CHECK(panel->workPaths->getYPosition() == -(bottom ? maximum : 0));
  }
  CHECK(app.session.dirty() && app.session.view().token == token);
}
TEST_MAIN(run)
