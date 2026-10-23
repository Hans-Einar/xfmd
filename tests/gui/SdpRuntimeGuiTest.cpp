#include "application/Application.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
using namespace xfmd;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& ready) {
  for (int i = 0; i < 1000; ++i) {
    settleNative(app.app, 5);
    if (ready())
      return;
  }
  throw std::runtime_error(app.window->workspacePanel->sdp->status->getText().text());
}
std::size_t calls(const fs::path& path, const std::string& wanted) {
  std::ifstream input(path);
  std::string line;
  std::size_t count = 0;
  while (std::getline(input, line))
    count += line == wanted;
  return count;
}
void run() {
  fs::path home = FX::FXSystem::getHomeDirectory().text();
  fs::path runtime = getenv("XDG_RUNTIME_DIR");
  auto project = home / "project", log = home / "producer-calls";
  fs::create_directories(project / "SDP");
  setenv("XFMD_RUNTIME_CALLS", log.c_str(), 1);
  setenv("XFMD_SDP_TOOL", XFMD_RUNTIME_FIXTURE, 1);
  fs::path workspace;
  {
    int argc = 1;
    char name[] = "sdp-runtime";
    char* argv[]{name, nullptr};
    Application app;
    app.initialize(argc, argv);
    XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(),
                   RevertToParent, CurrentTime);
    CHECK(app.startPath(project.string()));
    auto* panel = app.window->workspacePanel->sdp;
    until(app, [&] { return panel->tree("sdl") && panel->tree("sdl")->getFirstItem(); });
    settleNative(app.app, 1100); // Finish initial watch/discovery reconciliation before holding rows.
    CHECK(!fs::exists(runtime / "xfmd"));
    CHECK(calls(log, "select") == 0);
    auto* tabs = app.window->workspacePanel->tabs;
    nativeClick(tabs->getLast(), 15, 8);
    auto* tree = panel->tree("sdl");
    auto* parent = tree->getFirstItem();
    tree->setCurrentItem(parent);
    tree->setFocus();
    settleNative(app.app, 50);
    nativeKey(tree, XK_Return);
    CHECK(parent->isExpanded());
    int children = 0;
    for (auto* item = parent->getFirst(); item; item = item->getNext())
      ++children;
    CHECK(children == 35);
    CHECK(!fs::exists(runtime / "xfmd") && calls(log, "select") == 0);
    auto action = static_cast<NavigationItem*>(parent->getFirst())->action;
    const auto token = app.session.view().token;
    setenv("XDG_RUNTIME_DIR", (home / "missing-runtime").c_str(), 1);
    tree->activated(action);
    CHECK(app.session.view().token == token && calls(log, "select") == 0);
    CHECK(panel->status->getText().contains("runtime"));
    setenv("XDG_RUNTIME_DIR", runtime.c_str(), 1);
    fs::path first;
    for (int i = 0; i < 105; ++i) {
      const auto before = app.session.view().path;
      tree->activated(action);
      until(app, [&] { return app.session.view().path != before && app.host->interactive(); });
      const fs::path current = app.session.view().path;
      if (i == 0) {
        first = current;
        workspace = current.parent_path().parent_path();
        CHECK(workspace.parent_path() == runtime / "xfmd");
      }
      CHECK(current.parent_path().parent_path() == workspace);
    }
    CHECK(calls(log, "select") == 105);
    until(app, [&] { return !fs::exists(first.parent_path()); });
    const auto last = app.session.view().path;
    CHECK(app.navigation->goBack());
    until(app, [&] { return app.host->interactive(); });
    CHECK(fs::exists(last));
    CHECK(app.navigation->goForward() && app.session.view().path == last);
    app.window->workspacePanel->sdpRefresh();
    until(app, [&] { return panel->status->getText().contains("Navigation updated"); });
    CHECK(calls(log, "select") == 105);
    CHECK(fs::exists(last));
  }
  CHECK(!workspace.empty() && !fs::exists(workspace));
}
TEST_MAIN(run)
