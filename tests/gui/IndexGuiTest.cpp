#include "application/Application.h"
#include "support/Capture.h"
#include "support/TestSupport.h"
#include <X11/keysym.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
using namespace xfmd;
using namespace FX;
void events(Application& app, int count = 30) {
  for (int i = 0; i < count; ++i) {
    app.app.runWhileEvents();
    auto* display = static_cast<Display*>(app.app.getDisplay());
    for (int j = 0; j < 1024 && XPending(display); ++j)
      app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void settle(Application& app) {
  for (int i = 0; i < 200; ++i) {
    events(app, 2);
    if (app.host->interactive() && app.preview->currentModel() &&
        app.preview->currentModel()->token == app.session.view().token && !app.references->busy())
      return;
  }
  CHECK(false);
}
void click(Application& app, FXWindow* window, int x, int y, unsigned long time) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent e{};
  e.xbutton.display = display;
  e.xbutton.window = window->id();
  e.xbutton.root = DefaultRootWindow(display);
  e.xbutton.button = Button1;
  e.xbutton.same_screen = True;
  e.xbutton.x = x;
  e.xbutton.y = y;
  e.xbutton.time = time;
  e.type = ButtonPress;
  CHECK(XSendEvent(display, window->id(), False, ButtonPressMask, &e));
  e.type = ButtonRelease;
  CHECK(XSendEvent(display, window->id(), False, ButtonReleaseMask, &e));
  XSync(display, False);
  events(app);
}
void clickItem(Application& app, FXTreeList* tree, FXTreeItem* item, unsigned long time) {
  CHECK(item);
  tree->makeItemVisible(item);
  events(app, 2);
  int y = -1, depth = 0;
  for (int row = 0; row < tree->getViewportHeight(); ++row)
    if (tree->getItemAt(100, row) == item) {
      y = row + 5;
      break;
    }
  for (auto* p = item->getParent(); p; p = p->getParent())
    ++depth;
  CHECK(y >= 0);
  click(app, tree, 45 + depth * tree->getIndent(), y, time);
}
void key(Application& app, FXWindow* window, KeySym symbol) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent e{};
  e.xkey.display = display;
  e.xkey.window = window->id();
  e.xkey.root = DefaultRootWindow(display);
  e.xkey.same_screen = True;
  e.xkey.keycode = XKeysymToKeycode(display, symbol);
  e.type = KeyPress;
  CHECK(XSendEvent(display, window->id(), False, KeyPressMask, &e));
  e.type = KeyRelease;
  CHECK(XSendEvent(display, window->id(), False, KeyReleaseMask, &e));
  XSync(display, False);
  events(app);
}
void run() {
  auto dir = std::filesystem::path(FXSystem::getHomeDirectory().text());
  auto main = dir / "guide.md", other = dir / "architecture.md";
  std::string source = "# XFMD guide\n\n## Workspace\n\n";
  for (int i = 0; i < 30; ++i)
    source += "Native Markdown navigation, editing and preview.\n\n";
  source += "### Sidebar\n\n## References\n\n"
            "[Architecture](architecture.md) [Missing](missing.md)\n\n"
            "[FOX toolkit](https://www.fox-toolkit.org/) [Local anchor](#workspace)\n";
  std::ofstream(main) << source;
  std::ofstream(other) << "## Architecture\n\n### Internals\n\n## Components\n\nDetails\n";
  int argc = 1;
  char name[] = "xfmd-index-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  int errors = 0;
  app.documents.error = [&](const std::string& message) {
    std::cerr << message << "\n";
    ++errors;
  };
  CHECK(app.startPath(main.string()));
  app.views->setMode(ViewMode::Split);
  settle(app);
  auto* panel = app.window->workspacePanel;
  auto* index = panel->index;
  auto* tab = panel->tabs->getLast()->getPrev();
  CHECK(dynamic_cast<FXTabItem*>(tab));
  auto token = app.session.view().token;
  click(app, tab, 15, 10, 1000);
  CHECK(panel->tabs->getCurrent() == 1 && app.session.view().token == token);
  auto* title = index->outline->getFirstItem();
  CHECK(title && title->getText() == "XFMD guide");
  auto* workspace = title->getFirst();
  CHECK(workspace && workspace->getFirst()->getText() == "Sidebar");
  auto* chapter = workspace->getNext();
  CHECK(chapter && chapter->getText() == "References");
  clickItem(app, index->outline, chapter, 2000);
  CHECK(app.scrolling.captureAnchor().byte == source.find("## References"));
  CHECK(app.window->editor->sourceAnchor() > 0 && app.host->getYPosition() < 0);
  // Arrow navigation must not activate; Enter does.
  const auto anchor = app.scrolling.captureAnchor().byte;
  key(app, index->outline, XK_Up);
  CHECK(app.scrolling.captureAnchor().byte == anchor);
  key(app, index->outline, XK_Return);
  CHECK(app.scrolling.captureAnchor().byte == source.find("### Sidebar"));
  auto* root = index->references->getFirstItem();
  CHECK(root->getText() == "References" && root->getFirst()->getText() == "Markdown");
  CHECK(root->getLast()->getText() == "Hyperlinks");
  auto* file = root->getFirst()->getFirst();
  index->references->expandTree(file, true);
  events(app);
  settle(app);
  CHECK(file->getFirst()->getText() == "Architecture");
  CHECK(file->getFirst()->getNext()->getText() == "Components");
  CHECK(!file->getFirst()->getFirst());
  auto* missing = file->getNext();
  index->references->expandTree(missing, true);
  events(app);
  CHECK(missing->getFirst()->getText().find("Expand") < 0);
  // A changed file is navigated by heading title, not yesterday's byte offset.
  std::ofstream(other) << std::string(80, '\n') << "## Architecture\n\n## Components\n\nDetails\n";
  clickItem(app, index->references, file->getFirst()->getNext(), 3000);
  settle(app);
  CHECK(app.session.view().path == other.string());
  CHECK(app.scrolling.captureAnchor().byte == app.session.view().text.find("## Components"));
  CHECK(app.navigation->history.size() == 2);
  CHECK(app.navigation->goBack());
  settle(app);
  CHECK(app.session.view().path == main.string());
  app.edits.applyEdit({0, 0, "<!-- edited -->\n"});
  CHECK(!index->outline->isEnabled());
  settle(app);
  CHECK(index->outline->isEnabled() && app.session.dirty());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  root = index->references->getFirstItem();
  file = root->getFirst()->getFirst();
  token = app.session.view().token;
  clickItem(app, index->references, file, 4000);
  CHECK(app.session.view().token == token && app.session.dirty());
  CHECK(app.navigation->history.position() == 0);
  // Same-document heading clicks preserve dirty text, without a save prompt.
  clickItem(app, index->outline, index->outline->getFirstItem()->getFirst(), 5000);
  CHECK(app.session.view().token == token && app.session.dirty());
  CHECK(errors == 0);
  // Tab/F10 keep the same buffer and document index.
  app.views->toggleSidebar();
  CHECK(!panel->shown());
  app.views->toggleSidebar();
  CHECK(panel->shown() && panel->tabs->getCurrent() == 1);
  index->references->expandTree(file, true);
  events(app);
  settle(app);
  // Save As changes link bases without changing the document token.
  std::filesystem::create_directory(dir / "copy");
  token = app.session.view().token;
  CHECK(app.documents.save((dir / "copy/guide.md").string()));
  CHECK(app.session.view().token == token && !index->references->isEnabled());
  settle(app);
  auto* rebased =
      static_cast<NavigationItem*>(index->references->getFirstItem()->getFirst()->getFirst());
  CHECK(rebased->action.path == (dir / "copy/architecture.md").string());
  CHECK(app.open(main.string()));
  settle(app);
  file = index->references->getFirstItem()->getFirst()->getFirst();
  index->references->expandTree(file, true);
  events(app);
  settle(app);
  if (const auto* path = std::getenv("XFMD_INDEX_CAPTURE"))
    captureDesktop(app.app, path);
}
TEST_MAIN(run)
