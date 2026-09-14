#include "Application.h"
namespace xfmd {
void Application::applyAppearance(const Appearance& value) {
  ui->setAppearance(value);
  ui->apply(app.getRootWindow());
  window->restyle();
  app.refresh();
}
} // namespace xfmd
