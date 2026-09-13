#include "application/Application.h"
#include "support/TestSupport.h"
#include <chrono>
#include <filesystem>
#include <thread>
using namespace xfmd;
void test() {
  int argc = 1;
  char name[] = "export-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.edits.applyEdit({0, 0, "# Unsaved buffer\n\nUnicode æøå.\n"});
  auto token = app.session.view().token;
  auto path = (std::filesystem::path(std::getenv("HOME")) / "result.pdf").string();
  CHECK(app.startExport(path));
  CHECK(!app.startExport(path));
  app.edits.applyEdit({0, 0, "New edit\n"});
  auto stop = std::chrono::steady_clock::now() + std::chrono::seconds(10);
  while (app.exporter->busy() && std::chrono::steady_clock::now() < stop) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  CHECK(!app.exporter->busy());
  CHECK(std::filesystem::file_size(path) > 500);
  CHECK(app.session.dirty() && app.edits.canUndo() &&
        app.session.view().token.revision > token.revision);
  CHECK(app.window->status->getText().contains("current buffer has changed"));
}
TEST_MAIN(test)
