#include "Application.h"
#include <iostream>
#include <string>
int main(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--version") {
      std::cout << "xfmd 0.1.0\n";
      return 0;
    }
    if (std::string(argv[i]) == "--help") {
      std::cout << "Usage: xfmd [directory|file.md|file.txt]\nNative FOX Markdown viewer/editor.\n";
      return 0;
    }
  }
  try {
    xfmd::Application application;
    application.initialize(argc, argv);
    if (argc > 2) {
      std::cerr << "Usage: xfmd [directory|file.md|file.txt]\n";
      return 2;
    }
    if (argc == 2 && !application.startPath(argv[1])) {
      std::cerr << "xfmd: " << application.window->status->getText().text() << '\n';
      return 1;
    }
    return application.app.run();
  } catch (const std::exception& e) {
    std::cerr << "xfmd: " << e.what() << '\n';
    return 1;
  }
}
