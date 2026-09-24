#include "application/Application.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/keysym.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
using namespace FX;
using namespace xfmd;
void settle(Application& app) {
  for (int i = 0; i < 30; ++i) {
    drainEvents(app.app);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void key(Application& app, FXTreeList* tree, KeySym code) {
  drainEvents(app.app);
  app.window->setFocus();
  tree->setFocus();
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XSetInputFocus(display, app.window->id(), RevertToParent, CurrentTime);
  drainEvents(app.app);
  XEvent event{};
  event.xkey.display = display;
  event.xkey.window = tree->id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.same_screen = True;
  event.xkey.keycode = XKeysymToKeycode(display, code);
  event.type = KeyPress;
  CHECK(XSendEvent(display, tree->id(), False, KeyPressMask, &event));
  event.type = KeyRelease;
  CHECK(XSendEvent(display, tree->id(), False, KeyReleaseMask, &event));
  settle(app);
}
void run() {
  auto home = std::filesystem::path(FXSystem::getHomeDirectory().text());
  auto bin = home / "bin", root = home / "files";
  std::filesystem::create_directory(bin);
  std::filesystem::create_directory(root);
  auto launcher = bin / "xdg-open";
  std::ofstream(launcher) << "#!/bin/sh\nprintf '%s\\n' \"$#\" \"$1\" >> \"$HOME/requests\"\ncase "
                             "\"$1\" in *failure*) exit 4;; esac\n";
  std::filesystem::permissions(launcher, std::filesystem::perms::owner_all);
  std::string path = bin.string() + ":" + getenv("PATH");
  setenv("PATH", path.c_str(), 1);
  auto external = root / "-photo $(touch SENTINEL) 'quoted'.png", document = root / "guide.MD";
  std::ofstream(external).write("\0image", 6);
  std::ofstream(root / "failure.pdf").write("\0pdf", 4);
  std::ofstream(document) << "# Guide\n\n## Child\n";
  std::filesystem::create_directory(root / "empty.md");
  int argc = 1;
  char name[] = "tree-actions";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  std::string error;
  app.documents.error = [&](const auto& value) { error = value; };
  int dirtyPrompts = 0;
  app.documents.chooseUnsaved = [&] {
    ++dirtyPrompts;
    return UnsavedChoice::Cancel;
  };
  CHECK(app.window->workspacePanel->setWorkPath(root.string()));
  settle(app);
  app.edits.applyEdit({0, 0, "unsaved"});
  settle(app);
  auto token = app.session.view().token;
  auto* tree = app.window->sidebar;
  auto* item = tree->getPathnameItem(external.c_str());
  CHECK(item);
  for (auto code : {XK_Right, XK_space, XK_Return}) {
    tree->setCurrentItem(item);
    key(app, tree, code);
    CHECK(app.session.view().token == token && app.session.dirty() && app.edits.canUndo());
  }
  CHECK(dirtyPrompts == 0 && error.empty());
  std::ifstream requests(home / "requests");
  std::string count, target;
  for (int i = 0; i < 3; ++i) {
    CHECK(bool(std::getline(requests, count)));
    CHECK(count == "1");
    CHECK(bool(std::getline(requests, target)));
    CHECK(target == external.string());
  }
  CHECK(!std::getline(requests, count));
  CHECK(!std::filesystem::exists("SENTINEL"));
  tree->setCurrentItem(tree->getPathnameItem((root / "empty.md").c_str()));
  key(app, tree, XK_Right);
  CHECK(dirtyPrompts == 0 && error.empty());
  tree->setCurrentItem(tree->getPathnameItem(document.c_str()));
  key(app, tree, XK_space);
  CHECK(dirtyPrompts == 1 && app.session.view().token == token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  key(app, tree, XK_Right);
  CHECK(app.session.view().path == document.string());
  settle(app);
  tree->setCurrentItem(tree->getPathnameItem((root / "failure.pdf").c_str()));
  key(app, tree, XK_Return);
  CHECK(error.find("xdg-open could not open") != std::string::npos);
  // Both Index and References use this same tree widget, including lazy branches.
  app.window->workspacePanel->tabs->setCurrent(1, true);
  settle(app);
  for (auto* nav : {app.window->workspacePanel->index->outline,
                    app.window->workspacePanel->index->references}) {
    nav->clearItems();
    int actions = 0, expands = 0;
    IndexAction action;
    action.kind = IndexActionKind::File;
    action.path = document.string();
    auto* leaf = new NavigationItem("Leaf", action);
    nav->appendItem(nullptr, leaf);
    auto* branch = new NavigationItem("Lazy file", action);
    branch->setHasItems(true);
    nav->appendItem(nullptr, branch);
    nav->activated = [&](const auto&) { ++actions; };
    nav->expanded = [&](auto*) { ++expands; };
    nav->setCurrentItem(leaf);
    settle(app);
    for (auto code : {XK_Right, XK_space, XK_Return})
      key(app, nav, code);
    CHECK(actions == 3);
    nav->setCurrentItem(branch);
    key(app, nav, XK_Right);
    CHECK(actions == 3 && expands == 1);
    key(app, nav, XK_Left);
    CHECK(actions == 3);
    nav->activated = {};
    nav->expanded = {};
  }
}
TEST_MAIN(run)
