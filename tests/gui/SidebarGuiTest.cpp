#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void events(Application& app) {
  for (int i = 0; i < 60; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void clickItem(Application& app, FX::FXTreeItem* item, unsigned long time) {
  auto* tree = app.window->sidebar;
  tree->makeItemVisible(item);
  events(app);
  // Find the visible row, then click its label, beyond the indentation/expand box.
  int y = -1;
  for (int row = 0; row < tree->getViewportHeight(); ++row)
    if (tree->getItemAt(100, row) == item) {
      y = row + 5;
      break;
    }
  CHECK(y >= 0);
  int depth = 0;
  for (auto* parent = item->getParent(); parent; parent = parent->getParent())
    ++depth;
  XEvent event{};
  auto* display = static_cast<Display*>(app.app.getDisplay());
  event.xbutton.display = display;
  event.xbutton.window = tree->id();
  event.xbutton.root = DefaultRootWindow(display);
  event.xbutton.button = Button1;
  event.xbutton.same_screen = True;
  event.xbutton.x = 45 + depth * tree->getIndent();
  event.xbutton.y = y;
  event.xbutton.time = time;
  event.type = ButtonPress;
  CHECK(XSendEvent(display, tree->id(), False, ButtonPressMask, &event));
  event.type = ButtonRelease;
  CHECK(XSendEvent(display, tree->id(), False, ButtonReleaseMask, &event));
  XFlush(display);
  events(app);
}
void run() {
  char pattern[] = "/tmp/xfmd-sidebar-XXXXXX";
  std::filesystem::path directory = mkdtemp(pattern);
  struct Cleanup {
    std::filesystem::path directory;
    ~Cleanup() { std::filesystem::remove_all(directory); }
  } cleanup{directory};
  auto document = directory / "file.md";
  std::ofstream(document) << "# Open from tree\n";
  std::filesystem::create_directory(directory / "folder.md");
  int argc = 1;
  char name[] = "xfmd-sidebar-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.documents.error = [](const std::string& error) { throw std::runtime_error(error); };
  events(app);
  auto* tree = app.window->sidebar;
  tree->setDirectory("/");
  events(app);
  auto token = app.session.view().token;
  auto* root = tree->getPathnameItem("/");
  CHECK(root && tree->shown());
  clickItem(app, root, 1000);
  CHECK(tree->shown() && app.window->shown() && app.session.view().token == token);
  clickItem(app, root, 2000);
  clickItem(app, root, 2050);
  CHECK(tree->shown() && app.session.view().token == token);
  tree->setDirectory(directory.string().c_str());
  events(app);
  auto* folder = tree->getPathnameItem((directory / "folder.md").string().c_str());
  CHECK(folder);
  clickItem(app, folder, 3000);
  clickItem(app, folder, 3050);
  CHECK(tree->shown() && app.session.view().token == token);
  auto* file = tree->getPathnameItem(document.string().c_str());
  CHECK(file);
  clickItem(app, file, 4000);
  CHECK(tree->shown() && app.session.view().token == token);
  clickItem(app, file, 4050);
  CHECK(app.session.view().path == document.string() && tree->shown());
  CHECK(tree->getWidth() > 50);
  app.views->toggleSidebar();
  CHECK(!tree->shown());
  CHECK(app.open(document.string()) && !tree->shown());
  app.views->toggleSidebar();
  CHECK(tree->shown());
}
TEST_MAIN(run)
