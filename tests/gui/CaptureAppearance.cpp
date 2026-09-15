#include "application/Application.h"
#include "support/Capture.h"
#include "support/DrainEvents.h"
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
  app.window->resize(std::getenv("XFMD_CAPTURE_NARROW") ? 640 : 1200, 800);
  app.window->move(40, 40);
  const char* fixture = std::getenv("XFMD_CAPTURE_FIXTURE");
  if (!fixture) fixture = XFMD_FIXTURE;
  app.window->workspacePanel->setWorkPath(
      std::filesystem::path(fixture).parent_path().string());
  app.open(fixture);
  app.views->setMode(xfmd::ViewMode::Split);
  auto draft = app.preferences->begin();
  draft.appearance.theme = theme;
  std::string error;
  if (!app.preferences->commit(draft, error))
    return 3;
  if (std::getenv("XFMD_CAPTURE_A4"))
    app.execute(xfmd::CommandRouter::A4);
  for (int i = 0; i < 3000; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    if (i >= 300 && app.host->interactive())
      break;
  }
  if (!app.host->interactive())
    return 4;
  app.app.forceRefresh();
  app.app.repaint();
  app.app.flush(true);
  drainEvents(app.app);
  captureDesktop(app.app, output.c_str());
}
