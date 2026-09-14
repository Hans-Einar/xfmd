#include "Application.h"
#include "io/InputPolicy.h"
namespace xfmd {
void Application::openTreePath(const std::string& path) {
  if (InputPolicy::supportedPath(path)) {
    open(path);
    return;
  }
  try {
    desktopFiles.open(path);
    scheduler->restart(6, 100, [this] { pollDesktopFiles(); });
  } catch (const std::exception& e) {
    documents.error(e.what());
  }
}
void Application::pollDesktopFiles() {
  for (const auto& error : desktopFiles.poll())
    documents.error(error);
  if (desktopFiles.busy())
    scheduler->restart(6, 100, [this] { pollDesktopFiles(); });
}
} // namespace xfmd
