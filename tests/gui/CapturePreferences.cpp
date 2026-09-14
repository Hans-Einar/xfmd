#include "application/Application.h"
#include "application/ui/PreferencesDialog.h"
#include "support/Capture.h"
#include "support/DrainEvents.h"
#include <chrono>
#include <thread>
int main(int argc, char** argv) {
  if (argc < 2)
    return 2;
  std::string output = argv[1];
  std::string theme = argc > 2 ? argv[2] : "light";
  int fontSize = argc > 3 ? std::stoi(argv[3]) : 10;
  bool compact = argc > 4 && std::string(argv[4]) == "compact";
  std::string buttons = argc > 5 ? argv[5] : "flat";
  int tab = argc > 6 ? std::stoi(argv[6]) : 0;
  argc = 1;
  xfmd::Application app;
  app.initialize(argc, argv);
  auto settings = app.preferences->begin();
  settings.appearance = {theme, compact, buttons, fontSize};
  std::string error;
  if (!app.preferences->commit(settings, error))
    return 3;
  auto settle = [&] {
    for (int i = 0; i < 100; ++i) {
      app.app.runWhileEvents();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  };
  settle();
  xfmd::PreferencesDialog dialog(app.window, *app.preferences, *app.ui);
  std::function<FX::FXTabBook*(FX::FXWindow*)> findTabs =
      [&](FX::FXWindow* root) -> FX::FXTabBook* {
    if (auto* tabs = dynamic_cast<FX::FXTabBook*>(root))
      return tabs;
    for (auto* child = root->getFirst(); child; child = child->getNext())
      if (auto* tabs = findTabs(child))
        return tabs;
    return nullptr;
  };
  findTabs(&dialog)->setCurrent(tab);
  dialog.resize(std::max(640, fontSize * 48), std::max(490, fontSize * 37));
  dialog.create();
  dialog.show(FX::PLACEMENT_OWNER);
  settle();
  app.app.flush(true);
  drainEvents(app.app);
  captureDesktop(app.app, output.c_str());
}
