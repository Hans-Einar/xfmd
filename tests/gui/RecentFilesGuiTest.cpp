#include "application/Application.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
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
void run(bool restart) {
  fs::path home = FX::FXSystem::getHomeDirectory().text();
  auto a = (home / "første.md").string();
  auto b = (home / "second.txt").string();
  auto c = (home / "saved.md").string();
  if (!restart) {
    std::ofstream(a) << "# First";
    std::ofstream(b) << "Second";
    int argc = 1;
    char name[] = "recent-files-test";
    char* argv[] = {name, nullptr};
    Application app;
    app.initialize(argc, argv);
    XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                   CurrentTime);
    auto* panel = app.window->workspacePanel;
    auto* recent = panel->recentFiles;
    CHECK(recent->entries().empty());
    CHECK(app.open(a));
    CHECK(app.open(b));
    CHECK(app.open(a));
    CHECK(recent->entries() == std::vector<std::string>({a, b}));
    CHECK(recent->list->getItemText(0) == "første.md — ~");
    drainEvents(app.app);
    // Actual single click activates the second row; list mutation is deferred.
    auto* list = recent->list;
    click(app, list, 20, list->getItemHeight(0) + list->getItemHeight(1) / 2, Button1, 1000);
    drainEvents(app.app);
    CHECK(app.session.view().path == b);
    CHECK(recent->entries().front() == b);
    CHECK(panel->history.root() == home && panel->shown());
    app.edits.applyEdit({0, 0, "dirty "});
    app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
    auto token = app.session.view().token;
    list->setCurrentItem(1);
    recent->onSelect(nullptr, 0, nullptr);
    drainEvents(app.app);
    CHECK(app.session.view().token == token && app.session.dirty());
    CHECK(recent->entries().front() == b);
    app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
    fs::remove(a);
    std::string error;
    app.documents.error = [&](const std::string& message) { error = message; };
    recent->onSelect(nullptr, 0, nullptr);
    drainEvents(app.app);
    CHECK(!error.empty() && app.session.view().token == token && app.session.dirty());
    CHECK(recent->entries().front() == b);
    CHECK(app.documents.save(c));
    CHECK(recent->entries().front() == c);
    // Bound persisted history using successful opens, not synthetic UI insertion.
    for (int i = 0; i < 35; ++i) {
      auto path = (home / ("file-" + std::to_string(i) + ".md")).string();
      std::ofstream(path) << "document";
      CHECK(app.open(path));
    }
    CHECK(recent->entries().size() == 32);
    CHECK(recent->entries().back() == (home / "file-3.md").string());
    CHECK(app.open(c));
    CHECK(panel->history.root() == home && panel->shown());
  } else {
    int argc = 1;
    char name[] = "recent-files-restart";
    char* argv[] = {name, nullptr};
    Application app;
    app.initialize(argc, argv);
    auto* panel = app.window->workspacePanel;
    CHECK(panel->recentFiles->entries().size() == 32);
    CHECK(panel->recentFiles->entries().front() == c);
    CHECK(panel->history.root() == home);
    CHECK(app.session.view().path.empty());
    panel->recentFiles->list->setCurrentItem(0);
    panel->recentFiles->onSelect(nullptr, 0, nullptr);
    drainEvents(app.app);
    CHECK(app.session.view().path == c);
  }
}
int main(int argc, char**) {
  try {
    run(argc > 1);
    if (argc == 1) {
      auto child = fork();
      CHECK(child >= 0);
      if (child == 0) {
        execl("/proc/self/exe", "RecentFilesGuiTest", "restart", nullptr);
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
