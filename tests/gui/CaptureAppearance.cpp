#include "application/Application.h"
#include "support/Capture.h"
#include <chrono>
#include <filesystem>
#include <thread>
int main(int argc, char** argv) {
  if (argc != 3)
    return 2;
  std::string output = argv[1], theme = argv[2];
  argc = 1;
  xfmd::Application app;
  app.initialize(argc, argv);
  app.window->resize(1200, 800);
  app.window->move(40, 40);
  app.window->workspacePanel->setWorkPath(
      std::filesystem::path(XFMD_FIXTURE).parent_path().string());
  app.open(XFMD_FIXTURE);
  app.views->setMode(xfmd::ViewMode::Split);
  auto draft = app.preferences->begin();
  draft.appearance.theme = theme;
  std::string error;
  if (!app.preferences->commit(draft, error))
    return 3;
  for (int i = 0; i < 300; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  if (!app.host->interactive())
    return 4;
  app.app.forceRefresh();
  app.app.repaint();
  app.app.flush(true);
  captureDesktop(app.app, output.c_str());
}
