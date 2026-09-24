#include "application/Application.h"
#include "application/ui/OpenPathDialog.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
#include <fxkeys.h>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
FXButton* button(FXWindow* parent, const std::string& text) {
  for (auto* child = parent->getFirst(); child; child = child->getNext()) {
    auto* candidate = dynamic_cast<FXButton*>(child);
    if (candidate && candidate->getText().text() == text)
      return candidate;
    if (auto* found = button(child, text))
      return found;
  }
  return nullptr;
}
template <class T> T* findWidget(FXWindow* parent, std::function<bool(T*)> matches) {
  for (auto* child = parent->getFirst(); child; child = child->getNext()) {
    if (auto* candidate = dynamic_cast<T*>(child); candidate && matches(candidate))
      return candidate;
    if (auto* found = findWidget<T>(child, matches))
      return found;
  }
  return nullptr;
}
struct ChooserDriver : FXObject {
  Application& app;
  std::string selection, action, initial;
  bool visited = false;
  bool capture = false, menuPositioned = false;
  explicit ChooserDriver(Application& value) : app(value) {}
  long handle(FXObject*, FXSelector, void*) override {
    auto* dialog = dynamic_cast<OpenPathDialog*>(app.app.getModalWindow());
    if (!dialog) {
      auto* menu = app.app.getPopupWindow();
      CHECK(menu);
      auto* open = dynamic_cast<FXMenuCommand*>(menu->getFirst());
      CHECK(open && open->getSelector() == CommandRouter::Open);
      auto* display = static_cast<Display*>(app.app.getDisplay());
      if (!menuPositioned) {
        XWarpPointer(display, None, open->id(), 0, 0, 0, 0, 20, open->getHeight() / 2);
        XFlush(display);
        menuPositioned = true;
        app.app.addTimeout(this, 1, 100);
        return 1;
      }
      nativeClick(open, 20, open->getHeight() / 2, 0, Button1, false);
      app.app.addTimeout(this, 1, 80);
      return 1;
    }
    CHECK(dialog->getDirectory().text() == initial);
    if (!selection.empty())
      dialog->setFilename(selection.c_str());
    // Render a selected real path before recording the native chooser.
    dialog->recalc();
    dialog->layout();
    drainEvents(app.app);
    if (const auto* output = getenv("XFMD_UI_EVIDENCE"); output && capture) {
      captureDesktop(app.app, (fs::path(output) / "open-file-or-folder.png").c_str());
    }
    auto* accept = button(dialog, action);
    CHECK(accept);
    nativeClick(accept, accept->getWidth() / 2, accept->getHeight() / 2, 0, Button1, false);
    visited = true;
    return 1;
  }
};
void run() {
  const fs::path home = FXSystem::getHomeDirectory().text();
  const auto before = home / "before";
  const auto after = home / "project æ space";
  fs::create_directory(before);
  fs::create_directory(after);
  fs::create_directory(after / "notes.md");
  std::ofstream(before / "first.md") << "# First\n";
  std::ofstream(after / "hello ø.md") << "# Unicode\n";
  std::ofstream(after / "literal.data") << "# Text\n";
  std::ofstream(after / "invalid.txt").write("\0bad", 4);
  int argc = 1;
  char name[] = "open-dialog";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  std::string error;
  app.documents.error = [&](const auto& message) { error = message; };
  auto* workspace = app.window->workspacePanel;
  CHECK(workspace->setWorkPath(before.string()));
  CHECK(app.open((before / "first.md").string()));
  settleNative(app.app);
  auto choose = [&](const fs::path& path, const std::string& action, int entry = 0) {
    ChooserDriver driver(app);
    driver.selection = path.string();
    driver.action = action;
    driver.initial = workspace->history.root().string();
    driver.capture = entry == 1;
    app.app.addTimeout(&driver, 1, 80);
    if (entry == 1) {
      auto* display = static_cast<Display*>(app.app.getDisplay());
      XSetInputFocus(display, app.window->id(), RevertToParent, CurrentTime);
      XEvent event{};
      event.xkey.display = display;
      event.xkey.window = app.window->id();
      event.xkey.root = DefaultRootWindow(display);
      event.xkey.same_screen = True;
      event.xkey.keycode = XKeysymToKeycode(display, XK_o);
      event.xkey.state = ControlMask;
      event.type = KeyPress;
      CHECK(XSendEvent(display, app.window->id(), False, KeyPressMask, &event));
      event.type = KeyRelease;
      CHECK(XSendEvent(display, app.window->id(), False, KeyReleaseMask, &event));
      XFlush(display);
      settleNative(app.app, 300);
    } else if (entry == 2) {
      auto* open = findWidget<UiButton>(
          app.window, [](auto* b) { return b->getSelector() == CommandRouter::Open; });
      CHECK(open);
      CHECK(open->getSelector() == CommandRouter::Open);
      nativeClick(open, open->getWidth() / 2, open->getHeight() / 2);
    } else if (entry == 4) {
      auto* open = app.window->workspacePanel->openButton;
      nativeClick(open, open->getWidth() / 2, open->getHeight() / 2);
    } else if (entry == 3) {
      auto* title =
          findWidget<FXMenuTitle>(app.window, [](auto* t) { return t->getText() == "File"; });
      CHECK(title);
      auto* display = static_cast<Display*>(app.app.getDisplay());
      XWarpPointer(display, None, title->id(), 0, 0, 0, 0, title->getWidth() / 2,
                   title->getHeight() / 2);
      XFlush(display);
      nativeClick(title, title->getWidth() / 2, title->getHeight() / 2);
    } else
      app.execute(CommandRouter::Open);
    settleNative(app.app, 350);
    CHECK(driver.visited);
    settleNative(app.app);
  };
  choose(after / "hello ø.md", "OK", true);
  CHECK(app.session.view().path == (after / "hello ø.md").string());
  CHECK(workspace->history.root() == after);
  app.edits.applyEdit({0, 0, "dirty "});
  const auto dirty = app.session.snapshot();
  auto navigation = app.navigation->history.size();
  // Choosing a folder never resolves unsaved changes or creates a document visit.
  app.documents.chooseUnsaved = []() -> UnsavedChoice {
    throw std::runtime_error("Unexpected dirty prompt");
  };
  choose(after / "notes.md", "OK");
  CHECK(workspace->history.root() == after / "notes.md");
  CHECK(app.session.view().token == dirty.token && app.edits.canUndo());
  CHECK(app.navigation->history.size() == navigation);
  choose({}, "Open current folder", 2);
  CHECK(workspace->history.root() == after / "notes.md");
  auto roots = workspace->history.entries();
  choose(before / "first.md", "Cancel", 3);
  choose(before / "first.md", "Cancel", 4);
  CHECK(workspace->history.entries() == roots && app.session.view().token == dirty.token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  choose(before / "first.md", "OK");
  CHECK(workspace->history.entries() == roots && app.session.view().token == dirty.token);
  CHECK(app.session.dirty() && app.edits.canUndo());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(!app.openDialogPath((after / "invalid.txt").string()));
  CHECK(!error.empty());
  error.clear();
  CHECK(!app.openDialogPath((after / "missing.md").string()));
  CHECK(!error.empty());
  error.clear();
  CHECK(workspace->history.entries() == roots && app.session.view().token == dirty.token);
  CHECK(app.session.dirty() && app.edits.canUndo());
  choose(after / "literal.data", "OK");
  CHECK(workspace->history.root() == after && app.session.view().plainText);
  CHECK(app.views->mode() == ViewMode::Editor && !app.session.dirty());
  CHECK(error.empty());
}
TEST_MAIN(run)
