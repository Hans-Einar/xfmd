#include "application/Application.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
void run() {
  int argc = 1; char name[] = "xfmd-test"; char* argv[] = {name, nullptr};
  Application application;
  application.initialize(argc, argv);
  auto& app = application;
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
  app.edits.undo(); CHECK(app.session.view().text == raw);
  app.edits.redo(); CHECK(app.session.view().text == raw + "next");
  app.views->setMode(ViewMode::Editor);
  CHECK(!app.window->previewArea->shown() && app.edits.canUndo());
  app.views->toggleSidebar(); CHECK(!app.window->sidebar->shown());
  app.views->toggleSidebar(); CHECK(app.window->sidebar->shown());
  CHECK(FX::FXPath::match(app.window->sidebar->getPattern(), "hello.MD", app.window->sidebar->getMatchMode()));
  CHECK(FX::FXPath::match(app.window->sidebar->getPattern(), "hello.txt", app.window->sidebar->getMatchMode()));
  CHECK(!FX::FXPath::match(app.window->sidebar->getPattern(), "hello.png", app.window->sidebar->getMatchMode()));
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Cancel; };
  CHECK(!app.documents.requestClose());
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(app.documents.requestClose());
}
TEST_MAIN(run)
