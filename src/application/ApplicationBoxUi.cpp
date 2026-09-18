#include "Application.h"
namespace xfmd {
void Application::wireBoxUi() {
  boxUiOverlay = std::make_unique<FoxBoxUiOverlay>(*host, boxUiSession);
  boxUiOverlay->notice = [this](const std::string& message) {
    window->status->setText(message.c_str());
  };
  host->boxUiChanged = [this] { boxUiOverlay->reconcile(); };
  host->boxUiMoved = [this] { boxUiOverlay->position(); };
  boxUiOverlay->completed = [this](const BoxUiCommandResult& result) {
    if (result.status == "accepted")
      preview->refresh();
    window->status->setText(("BoxUI — " + result.status + ": " + result.message).c_str());
  };
}
} // namespace xfmd
