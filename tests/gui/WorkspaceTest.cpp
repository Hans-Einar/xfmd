#include "application/Application.h"
#include "support/TestSupport.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void run() {
  int argc = 1;
  char name[] = "xfmd-test";
  char* argv[] = {name, nullptr};
  Application application;
  application.initialize(argc, argv);
  for (int i = 0; i < 100 && !application.host->interactive(); ++i) {
    application.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  CHECK(application.host->interactive());
  CHECK(application.window->status->getText() == "0 bytes — saved");
  auto& app = application;
  for (const auto* key : {"Ctrl+O", "Ctrl+S", "Ctrl+Shift+S", "Ctrl+Z", "Ctrl+Y", "Ctrl+F",
                          "Ctrl+1", "Ctrl+2", "Ctrl+3", "F10", "Alt+Left", "Alt+Right"})
    CHECK(app.window->getAccelTable()->hasAccel(FX::parseAccel(key)));
  CHECK(app.views->mode() == ViewMode::Preview);
  CHECK(!app.window->editor->shown());
  app.views->setMode(ViewMode::Split);
  app.window->layout();
  CHECK(app.window->editor->shown() && app.window->previewArea->shown());
  auto raw = std::string("\xef\xbb\xbf# æøå\r\nbody\n");
  app.edits.applyEdit({0, 0, raw});
  CHECK(app.window->editor->getText() == "# æøå\nbody\n");
  CHECK(app.session.dirty());
  app.window->editor->appendText(FX::FXString("next"), true);
  CHECK(app.session.view().text == raw + "next");
  app.edits.undo();
  CHECK(app.session.view().text == raw);
  app.edits.redo();
  CHECK(app.session.view().text == raw + "next");
  app.views->setMode(ViewMode::Editor);
  CHECK(!app.window->previewArea->shown() && app.edits.canUndo());
  app.views->toggleSidebar();
  CHECK(!app.window->workspacePanel->shown());
  app.views->toggleSidebar();
  CHECK(app.window->workspacePanel->shown());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(!app.documents.requestClose());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(app.documents.requestClose());
}
TEST_MAIN(run)
