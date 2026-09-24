#include "application/Application.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
void run() {
  const auto root = std::filesystem::path(FXSystem::getHomeDirectory().text());
  auto files = root / "files";
  std::filesystem::create_directory(files);
  auto bin = root / "bin";
  std::filesystem::create_directory(bin);
  for (const char* name : {"xdg-open", "chosen-browser"}) {
    auto p = bin / name;
    std::ofstream(p)
        << "#!/bin/sh\nprintf '%s\\n' '" << name
        << "' \"$#\" \"$1\" >> \"$HOME/launches\"\ncase \"$1\" in *failure*) exit 4;; esac\n";
    std::filesystem::permissions(p, std::filesystem::perms::owner_all);
  }
  auto path = bin.string() + ":" + getenv("PATH");
  setenv("PATH", path.c_str(), 1);
  std::ofstream(files / "text.data") << "# literal text\n";
  std::ofstream(files / "target.MD") << "# Markdown\n";
  std::ofstream(files / "page æ.HTML") << "<h1>HTML</h1>\n";
  std::ofstream(files / "failure.html") << "<p>error</p>";
  std::ofstream(files / "binary.data").write("\0data", 5);
  const auto source = files / "source.md";
  std::ofstream(source) << "# Links\n\n[text](text.data)\n\n[md](target.MD)\n\n"
                           "[html](page%20%C3%A6.HTML)\n\n[binary](binary.data)\n\n"
                           "[web](https://example.org/test?q=1)\n";
  int argc = 1;
  char name[] = "file-routing";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  std::string error;
  app.documents.error = [&](const auto& e) { error = e; };
  auto prefs = app.preferences->begin();
  prefs.browserProgram = "chosen-browser";
  CHECK(app.preferences->commit(prefs, error));
  CHECK(app.window->workspacePanel->setWorkPath(files.string()));
  auto link = [&](const std::string& label, bool ctrl) {
    CHECK(app.open(source.string()));
    app.views->setMode(ViewMode::Preview);
    settleNative(app.app, 300);
    CHECK(app.host->interactive());
    for (const auto& run : app.host->frame()->runs)
      if (run.text == label && !run.link.empty()) {
        auto point = app.host->documentToView({run.bounds.x + 2, run.bounds.y + 2});
        nativeClick(app.host, int(point.x) + app.host->getXPosition(),
                    int(point.y) + app.host->getYPosition(), ctrl ? ControlMask : 0);
        return;
      }
    CHECK(false);
  };
  auto tree = [&](const std::string& filename, bool ctrl) {
    auto* view = app.window->sidebar;
    auto* item = view->getPathnameItem((files / filename).c_str());
    CHECK(item);
    view->makeItemVisible(item);
    settleNative(app.app);
    int y = -1;
    for (int row = 0; row < view->getViewportHeight(); ++row)
      if (view->getItemAt(100, row) == item) {
        y = row + 5;
        break;
      }
    CHECK(y >= 0);
    nativeClick(view, 65, y, ctrl ? ControlMask : 0);
  };
  for (bool fromTree : {false, true}) {
    for (const auto& item :
         std::vector<std::pair<std::string, std::string>>{{"text.data", "text"},
                                                          {"target.MD", "md"},
                                                          {"page æ.HTML", "html"},
                                                          {"binary.data", "binary"}}) {
      for (bool ctrl : {false, true}) {
        CHECK(app.open(source.string()));
        app.views->setMode(ViewMode::Preview);
        auto history = app.navigation->history.size();
        if (fromTree)
          tree(item.first, ctrl);
        else
          link(item.second, ctrl);
        bool external = ctrl || item.second == "html" || item.second == "binary";
        if (external) {
          CHECK(app.session.view().path == source.string() && !app.session.dirty());
          CHECK(app.navigation->history.size() == history);
        } else {
          CHECK(app.session.view().path == (files / item.first).string());
          CHECK(app.session.view().plainText == (item.second == "text"));
          CHECK(app.views->mode() ==
                (item.second == "text" ? ViewMode::Editor : ViewMode::Preview));
        }
        CHECK(app.window->workspacePanel->history.root() == files);
        CHECK(error.empty());
      }
    }
  }
  link("web", false);
  link("web", true);
  // The navigator uses its own source directory while retaining its Markdown pane.
  const auto navDir = files / "navigation";
  std::filesystem::create_directory(navDir);
  std::filesystem::copy_file(source, navDir / "index.md");
  for (const char* entry : {"text.data", "target.MD", "page æ.HTML", "binary.data"})
    std::filesystem::copy_file(files / entry, navDir / entry);
  DocumentViewConfig config{(navDir / "index.md").string(), "", "", "routes", "", "routes", ""};
  app.documentViews = std::make_unique<DocumentViews>(app, config);
  auto* navigator = app.documentViews->navigator.get();
  auto navLink = [&](const std::string& label, bool ctrl) {
    CHECK(navigator->open(config.navigator));
    settleNative(app.app, 300);
    auto* host = navigator->host;
    CHECK(host->interactive());
    for (const auto& run : host->frame()->runs)
      if (run.text == label && !run.link.empty()) {
        auto point = host->documentToView({run.bounds.x + 2, run.bounds.y + 2});
        nativeClick(host, int(point.x) + host->getXPosition(), int(point.y) + host->getYPosition(),
                    ctrl ? ControlMask : 0);
        return;
      }
    CHECK(false);
  };
  navLink("md", false);
  CHECK(navigator->path() == (navDir / "target.MD").string());
  CHECK(app.session.view().path == source.string());
  navLink("text", false);
  CHECK(app.session.view().path == (navDir / "text.data").string());
  CHECK(app.session.view().plainText && app.views->mode() == ViewMode::Editor);
  auto retained = app.session.snapshot();
  for (const char* label : {"text", "md", "html", "binary", "web"}) {
    navLink(label, true);
    CHECK(app.session.view().token == retained.token);
  }
  navLink("html", false);
  navLink("web", false);
  navLink("binary", false);
  CHECK(app.session.view().token == retained.token);
  CHECK(navigator->path() == config.navigator);
  if (const auto* output = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (std::filesystem::path(output) / "text-and-navigator.png").c_str());
  settleNative(app.app, 300);
  std::ifstream calls(root / "launches");
  std::string program, count, argument;
  int chosen = 0, system = 0, html = 0, urls = 0;
  while (std::getline(calls, program)) {
    CHECK(bool(std::getline(calls, count)) && count == "1");
    CHECK(bool(std::getline(calls, argument)));
    if (program == "chosen-browser")
      ++chosen;
    else {
      CHECK(program == "xdg-open");
      ++system;
    }
    if (argument.rfind("file://", 0) == 0) {
      ++html;
      CHECK(argument.find("page%20%C3%A6.HTML") != std::string::npos);
    }
    if (argument.rfind("https://", 0) == 0)
      ++urls;
  }
  CHECK(chosen == 5 && system == 17 && html == 3 && urls == 4);
  // A dialog handoff shares policy but must not change the work root or document.
  auto roots = app.window->workspacePanel->history.entries();
  CHECK(app.openDialogPath((navDir / "page æ.HTML").string()));
  CHECK(app.window->workspacePanel->history.entries() == roots);
  CHECK(app.session.view().token == retained.token);
  app.edits.applyEdit({0, 0, "dirty "});
  auto dirty = app.session.snapshot();
  auto history = app.navigation->history.size();
  tree("text.data", true);
  CHECK(app.session.view().token == dirty.token && app.session.dirty() && app.edits.canUndo());
  CHECK(app.navigation->history.size() == history);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  tree("target.MD", false);
  CHECK(app.session.view().token == dirty.token && app.edits.canUndo());
  CHECK(app.navigation->history.size() == history);
  CHECK(app.openTarget((files / "failure.html").string()));
  settleNative(app.app, 350);
  CHECK(error.find("browser could not open") != std::string::npos);
  CHECK(app.session.view().token == dirty.token);
  error.clear();
  app.followLink(source.string(), "javascript:alert(1)");
  CHECK(!error.empty() && app.session.view().token == dirty.token);
}
TEST_MAIN(run)
