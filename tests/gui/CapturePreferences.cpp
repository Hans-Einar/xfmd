#include "application/Application.h"
#include "application/ui/PreferencesDialog.h"
#include "support/Capture.h"
#include <chrono>
#include <thread>
int main(int argc, char** argv) {
  if (argc != 2)
    return 2;
  std::string output = argv[1];
  argc = 1;
  xfmd::Application app;
  app.initialize(argc, argv);
  auto settle = [&] {
    for (int i = 0; i < 100; ++i) {
      app.app.runWhileEvents();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  };
  settle();
  xfmd::PreferencesDialog dialog(app.window, *app.preferences, *app.ui);
  dialog.create();
  dialog.show(FX::PLACEMENT_OWNER);
  settle();
  app.app.flush(true);
  captureDesktop(app.app, output.c_str());
}
