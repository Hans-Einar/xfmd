#include "application/Application.h"
#include "application/ui/PreferencesDialog.h"
#include "support/TestSupport.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
using namespace xfmd;
using namespace FX;
FXComboBox* browserField(FXWindow* window) {
  if (auto* combo = dynamic_cast<FXComboBox*>(window))
    if (combo->getSelector() == PreferencesDialog::BrowserChanged)
      return combo;
  for (auto* child = window->getFirst(); child; child = child->getNext())
    if (auto* result = browserField(child))
      return result;
  return nullptr;
}
void run() {
  int argc = 1;
  char name[] = "browser-preferences-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  const auto dir = std::filesystem::path(FXSystem::getHomeDirectory().text());
  const auto program = dir / "test browser";
  std::ofstream(program) << "#!/bin/sh\nprintf '%s\\n' \"$1\" >> \"$HOME/browser-requests\"\n";
  std::filesystem::permissions(program, std::filesystem::perms::owner_all);
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui);
    dialog.create();
    auto* field = browserField(&dialog);
    CHECK(field && field->getText() == "xdg-open");
    field->setText(program.c_str());
    dialog.onAccept(nullptr, 0, nullptr);
  }
  CHECK(app.preferences->active().browserProgram == program.string());
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui);
    dialog.create();
    browserField(&dialog)->setText("cancelled-browser");
    dialog.handle(&dialog, FXSEL(SEL_COMMAND, FXDialogBox::ID_CANCEL), nullptr);
  }
  CHECK(app.preferences->active().browserProgram == program.string());
  std::ofstream(dir / "links.md") << "# Links\n\n[Site](https://example.org/)\n";
  CHECK(app.open((dir / "links.md").string()));
  for (int i = 0; i < 100 && !app.host->interactive(); ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  CHECK(app.host->interactive());
  auto token = app.session.view().token;
  app.host->linkActivated("https://example.org/from-preview");
  IndexAction action;
  action.kind = IndexActionKind::Hyperlink;
  action.target = "https://example.org/from-index";
  app.window->workspacePanel->index->activated(action);
  for (int i = 0; i < 1000 && app.browser.poll(); ++i)
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  CHECK(!app.browser.poll() && app.session.view().token == token);
  std::ifstream stream(dir / "browser-requests");
  std::string a, b;
  std::getline(stream, a);
  std::getline(stream, b);
  CHECK((a == "https://example.org/from-preview" && b == "https://example.org/from-index") ||
        (b == "https://example.org/from-preview" && a == "https://example.org/from-index"));
  auto draft = app.preferences->begin();
  draft.browserProgram = (dir / "missing-program").string();
  std::string error;
  CHECK(app.preferences->commit(draft, error));
  app.documents.error = [&](const std::string& value) { error = value; };
  app.host->linkActivated("https://example.org/fails");
  CHECK(error.find("missing-program") != std::string::npos && app.session.view().token == token);
}
TEST_MAIN(run)
