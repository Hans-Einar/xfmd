#include "application/adapters/FoxPreferencesStore.h"
#include "application/ui/PreferencesDialog.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <limits>
using namespace xfmd;
void run() {
  int argc = 1;
  char name[] = "prefs-test";
  char* argv[] = {name, nullptr};
  FX::FXApp app("xfmd", "xfmd");
  app.init(argc, argv);
  auto* window = new FX::FXMainWindow(&app, "Preferences test");
  app.reg().writeStringEntry("WorkPaths", "path0", "/tmp");
  app.reg().writeStringEntry("Future", "key", "keep");
  FoxPreferencesStore store(app.reg());
  PreferencesService service(store.load(),
                             [&](const auto& s, std::string& e) { return store.save(s, e); });
  auto draft = service.begin();
  draft.scroll.speed = 2.5;
  draft.browserProgram = "google-chrome-stable";
  draft.appearance = {"dark", true, "classic", 12};
  std::string error;
  service.cancel();
  CHECK(service.active().scroll.speed == 1.5);
  CHECK(service.active().appearance.theme == "light");
  CHECK(service.active().browserProgram == "xdg-open");
  CHECK(service.commit(draft, error));
  CHECK(service.active().scroll.speed == 2.5);
  FoxPreferencesStore reread(app.reg());
  CHECK(reread.load().scroll.speed == 2.5);
  FX::FXRegistry fresh("xfmd", "xfmd");
  CHECK(fresh.read());
  FoxPreferencesStore freshStore(fresh);
  CHECK(freshStore.load().appearance == draft.appearance);
  CHECK(freshStore.load().browserProgram == "google-chrome-stable");
  CHECK(std::string(app.reg().readStringEntry("Future", "key", "")) == "keep");
  CHECK(std::string(app.reg().readStringEntry("WorkPaths", "path0", "")) == "/tmp");
  draft.scroll.speed = std::numeric_limits<double>::quiet_NaN();
  CHECK(!service.commit(draft, error));
  CHECK(service.active().scroll.speed == 2.5);
  for (const auto& invalid :
       {std::string{}, std::string("browser\ncommand"), std::string("a\0b", 3)}) {
    draft = service.begin();
    draft.browserProgram = invalid;
    CHECK(!service.commit(draft, error));
    CHECK(service.active().browserProgram == "google-chrome-stable");
  }
  UiContext context(app, service.active().appearance);
  {
    PreferencesDialog dialog(window, service, context);
    app.create();
    dialog.create();
  }
  auto path =
      std::filesystem::path(FX::FXSystem::getHomeDirectory().text()) / ".foxrc" / "xfmd" / "xfmd";
  std::filesystem::remove(path);
  std::filesystem::create_directory(path);
  std::ofstream(path / "blocker") << "keep";
  draft = service.begin();
  draft.scroll.speed = 3;
  CHECK(!service.commit(draft, error));
  CHECK(service.active().scroll.speed == 2.5);
  CHECK(app.reg().readRealEntry("Scroll", "speed", 0) == 2.5);
  CHECK(std::string(app.reg().readStringEntry("Programs", "browser", "")) ==
        "google-chrome-stable");
  CHECK(service.active().appearance.theme == "dark");
  CHECK(std::string(app.reg().readStringEntry("Appearance", "theme", "")) == "dark");
  draft = service.begin();
  draft.appearance.theme = "missing";
  CHECK(!service.commit(draft, error));
  app.reg().writeIntEntry("Preferences", "version", 2);
  FoxPreferencesStore future(app.reg());
  future.load();
  CHECK(!future.save(draft, error));
  delete window;
}
TEST_MAIN(run)
