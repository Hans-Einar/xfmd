#include "application/Application.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void paste(Application& app, DocumentPathField* field, const std::string& text) {
  FXTextField clipboard(app.window, 1);
  clipboard.create();
  clipboard.hide();
  clipboard.setText(text.c_str());
  clipboard.selectAll();
  clipboard.onCmdCopySel(nullptr, 0, nullptr);
  if (!field->isEditing())
    nativeClick(field, 20, field->getHeight() / 2);
  CHECK(field->isEditing());
  nativeKey(field, XK_a, ControlMask);
  nativeKey(field, XK_v, ControlMask);
  settleNative(app.app, 300);
  CHECK(field->getText().text() == text);
}
void run() {
  fs::path home = FXSystem::getHomeDirectory().text();
  auto base = home / "project", notes = home / "notes";
  fs::create_directories(base / "deep");
  fs::create_directory(notes);
  auto original = notes / "live æ.md";
  std::ofstream(original) << "# Original\n\n[Reference](ref.md)\n";
  std::ofstream(notes / "ref.md") << "# Referenced\n";
  std::ofstream(base / "read.md") << "# Work file\n";
  std::ofstream(base / "one.md") << "# One\n";
  std::ofstream(base / "one.txt") << "Text\n";
  std::ofstream(base / "literal*.md") << "# Literal wildcard\n";
  int argc = 1;
  char name[] = "path-workspace";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  std::string error;
  app.documents.error = [&](const auto& e) { error = e; };
  auto* panel = app.window->workspacePanel;
  auto* field = app.window->documentPath;
  auto headerFits = [&] {
    auto* row = panel->refreshButton->getParent();
    CHECK(panel->markdown->getText().empty() && panel->markdown->getIcon());
    CHECK(panel->markdown->getWidth() > panel->markdown->getHeight());
    int right = 0;
    for (auto* child = row->getFirst(); child; child = child->getNext()) {
      if (!child->shown())
        continue;
      CHECK(child->getX() >= right && child->getY() >= 0);
      right = child->getX() + child->getWidth();
      CHECK(right <= row->getWidth());
    }
    for (auto* tab = panel->tabs->getFirst(); tab; tab = tab->getNext())
      CHECK(tab->getX() >= 0 && tab->getX() + tab->getWidth() <= panel->tabs->getWidth());
  };
  auto capture = [&](const char* name) {
    if (const auto* out = getenv("XFMD_UI_EVIDENCE"))
      captureDesktop(app.app, (fs::path(out) / name).c_str());
  };
  CHECK(field->getText() == "Untitled (not saved)");
  settleNative(app.app);
  nativeClick(field, 20, 10, 0, Button3);
  CHECK(app.window->status->getText().find("no saved path") >= 0);
  CHECK(panel->setWorkPath(base.string()));
  CHECK(app.open(original.string()));
  settleNative(app.app, 400);
  CHECK(field->getText() == original.c_str() && !field->isEditing());
  for (int width : {1100, 640, 450}) {
    app.window->resize(width, 760);
    settleNative(app.app);
    headerFits();
    CHECK(field->getParent()->getWidth() == app.window->getWidth());
    CHECK(field->getWidth() > width - 140);
    CHECK(field->getParent()->getY() >= app.window->previewControls->getParent()->getY() +
                                            app.window->previewControls->getParent()->getHeight());
  }
  app.window->resize(1100, 760);
  settleNative(app.app);
  nativeClick(field, 20, 10, 0, Button3);
  FXString copied;
  CHECK(app.window->getDNDData(FROM_CLIPBOARD, app.app.registerDragType("UTF8_STRING"), copied));
  CHECK(copied == original.c_str());
  paste(app, field, "read.md");
  CHECK(panel->nameFilter() == "read.md");
  nativeKey(field, XK_Return);
  CHECK(app.session.view().path == (base / "read.md").string());
  CHECK(panel->history.root() == base && !field->isEditing() && panel->nameFilter().empty());
  paste(app, field, (base / "literal*.md").string());
  nativeKey(field, XK_Return);
  CHECK(app.session.view().path == (base / "literal*.md").string());
  app.edits.applyEdit({0, 0, "dirty "});
  auto dirty = app.session.snapshot();
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  paste(app, field, original.string());
  nativeKey(field, XK_Return);
  CHECK(field->isEditing() && app.session.view().token == dirty.token && app.edits.canUndo());
  nativeClick(field, 20, 10, 0, Button3);
  CHECK(app.window->getDNDData(FROM_CLIPBOARD, app.app.registerDragType("UTF8_STRING"), copied));
  CHECK(copied == (base / "literal*.md").c_str());
  nativeKey(field, XK_Escape);
  CHECK(!field->isEditing() && panel->nameFilter().empty());
  paste(app, field, "missing.md");
  nativeKey(field, XK_Return);
  CHECK(app.session.view().token == dirty.token && field->isEditing());
  CHECK(app.window->status->getText().find("Cannot open exact path") >= 0);
  nativeKey(field, XK_Escape);
  // A folder submission and parent navigation never prompt for unsaved edits.
  app.documents.chooseUnsaved = []() -> UnsavedChoice {
    throw std::runtime_error("Unexpected prompt");
  };
  paste(app, field, "deep");
  nativeKey(field, XK_Return);
  CHECK(panel->history.root() == base / "deep" && app.session.view().token == dirty.token);
  nativeClick(panel->upButton, 10, 10);
  settleNative(app.app, 250);
  CHECK(panel->history.root() == base);
  paste(app, field, "one*");
  nativeClick(panel->markdown, 10, 10);
  settleNative(app.app, 350);
  CHECK(!field->isEditing() && field->getText() == (base / "literal*.md").c_str());
  CHECK(panel->tree->getPathnameItem((base / "one.md").c_str()));
  CHECK(!panel->tree->getPathnameItem((base / "one.txt").c_str()));
  CHECK(panel->markdown->isChecked());
  panel->markdown->setFocus();
  nativeKey(panel->markdown, XK_space);
  settleNative(app.app, 350);
  CHECK(!panel->markdown->isChecked());
  CHECK(panel->tree->getPathnameItem((base / "one.txt").c_str()));
  nativeKey(panel->markdown, XK_space);
  settleNative(app.app, 350);
  CHECK(panel->markdown->isChecked());
  CHECK(!panel->tree->getPathnameItem((base / "one.txt").c_str()));
  capture("files-header-selected.png");
  // Editing then Escape restores an already active filter.
  paste(app, field, "read*");
  nativeKey(field, XK_Escape);
  settleNative(app.app, 300);
  CHECK(panel->nameFilter() == "one*");
  std::ofstream(base / "one-new.md") << "# New\n";
  nativeClick(panel->refreshButton, 10, 10);
  settleNative(app.app, 350);
  CHECK(panel->tree->getPathnameItem((base / "one-new.md").c_str()));
  CHECK(panel->history.root() == base && panel->nameFilter() == "one*");
  // Native Index tab and Refresh rebuild the edited buffer, never disk contents.
  nativeClick(panel->tabs->getLast(), 15, 10);
  CHECK(panel->tabs->getCurrent() == 1 && panel->index->shown());
  CHECK(!panel->markdown->shown() && !panel->upButton->shown() && !panel->openButton->shown());
  CHECK(panel->refreshButton->shown() && panel->markdown->isChecked());
  capture("index-header.png");
  app.edits.applyEdit({0, app.session.view().text.size(),
                       "# Unsaved heading\n\n[Reference](" + (notes / "ref.md").string() + ")\n"});
  auto token = app.session.view().token;
  nativeClick(panel->refreshButton, 10, 10);
  settleNative(app.app, 450);
  CHECK(panel->index->outline->getFirstItem()->getText() == "Unsaved heading");
  CHECK(app.session.view().token == token && app.session.dirty());
  auto* references = panel->index->references;
  auto reference = [&] { return references->getFirstItem()->getFirst()->getFirst(); };
  references->setCurrentItem(reference());
  references->setFocus();
  nativeKey(references, XK_Right);
  settleNative(app.app, 350);
  CHECK(reference()->getFirst()->getText() == "Referenced");
  std::ofstream(notes / "ref.md") << "# Refreshed reference\n";
  nativeClick(panel->refreshButton, 10, 10);
  CHECK(reference()->getFirst()->getText().find("Expand") >= 0);
  references->setCurrentItem(reference());
  references->setFocus();
  nativeKey(references, XK_Right);
  settleNative(app.app, 350);
  CHECK(reference()->getFirst()->getText() == "Refreshed reference");
  CHECK(app.session.view().token == token && app.session.dirty());
  paste(app, field, "*.txt");
  nativeKey(field, XK_Escape);
  CHECK(panel->tabs->getCurrent() == 1);
  app.views->toggleSidebar();
  CHECK(!panel->shown());
  CHECK(field->shown());
  paste(app, field, "*.md");
  nativeKey(field, XK_Escape);
  app.views->toggleSidebar();
  settleNative(app.app);
  nativeClick(panel->tabs->getFirst(), 15, 10);
  CHECK(panel->markdown->shown() && panel->upButton->shown() && panel->openButton->shown());
  CHECK(panel->markdown->isChecked());
  headerFits();
  // Clear the name/type filters, expand a long folder and retain its context on refresh.
  nativeClick(field->getNext(), 10, 10);
  nativeClick(panel->markdown, 10, 10);
  for (int i = 0; i < 40; ++i)
    std::ofstream(base / "deep" / ("row-" + std::to_string(i) + ".md")) << "# Row\n";
  settleNative(app.app, 350);
  auto* tree = panel->tree;
  auto* deep = tree->getPathnameItem((base / "deep").c_str());
  CHECK(deep && panel->nameFilter().empty());
  tree->expandTree(deep, true);
  settleNative(app.app, 350);
  const auto selected = base / "deep/row-39.md";
  auto* selectedItem = tree->getPathnameItem(selected.c_str());
  CHECK(selectedItem);
  tree->setCurrentItem(selectedItem);
  tree->makeItemVisible(selectedItem);
  settleNative(app.app);
  int scroll = tree->getYPosition();
  CHECK(scroll < 0);
  nativeClick(panel->refreshButton, 10, 10);
  settleNative(app.app, 500);
  CHECK(tree->getItemPathname(tree->getCurrentItem()) == selected.c_str());
  CHECK(tree->getPathnameItem((base / "deep").c_str())->isExpanded());
  CHECK(tree->getYPosition() == scroll);
  nativeClick(panel->recentTabs->getFirst()->getNext()->getNext(), 15, 10);
  CHECK(panel->recentTabs->getCurrent() == 1 && panel->recentFiles->shown());
  CHECK(panel->recentFiles->entries().size() == 3);
  CHECK(panel->history.entries().front() == base);
  CHECK(panel->setWorkPath("/"));
  settleNative(app.app);
  CHECK(!panel->upButton->isEnabled());
  CHECK(panel->setWorkPath(base.string()));
  CHECK(app.session.view().token == token && app.edits.canUndo() && error.empty());
  settleNative(app.app, 350);
  capture("files-header-light.png");
  auto draft = app.preferences->begin();
  draft.appearance.theme = "dark";
  draft.appearance.buttons = "classic";
  CHECK(app.preferences->commit(draft, error));
  settleNative(app.app, 350);
  headerFits();
  capture("files-header-dark.png");
  if (const auto* out = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (fs::path(out) / "workspace-wide.png").c_str());
}
TEST_MAIN(run)
