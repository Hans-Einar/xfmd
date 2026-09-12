#include "application/Application.h"
#include <iostream>
int main(int argc, char** argv) {
  xfmd::Application application;
  application.initialize(argc, argv);
  application.open(XFMD_FIXTURE);
  application.views->toggleSidebar();
  application.views->setMode(xfmd::ViewMode::Split);
  application.app.runWhileEvents();
  std::cout << "READY\n" << std::flush;
  application.app.addTimeout(&application.app, FX::FXApp::ID_QUIT, 3000);
  return application.app.run();
}
