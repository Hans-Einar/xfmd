#include "Application.h"
namespace xfmd {
void Application::applyAppearance(const Appearance& value) {
  ui->setAppearance(value);
  ui->apply(app.getRootWindow());
  window->restyle();
  bool dark = value.theme == "dark";
  const auto& colors =
      dark ? preferences->active().darkReading : preferences->active().lightReading;
  if (host)
    host->setReadingColors(colors);
  window->previewColors->sync(colors, dark);
  app.refresh();
}
void Application::changeReadingColors(const ReadingColors& colors, bool commit) {
  host->setReadingColors(colors);
  if (!commit)
    return;
  auto draft = preferences->begin();
  bool dark = ui->appearance().theme == "dark";
  (dark ? draft.darkReading : draft.lightReading) = colors;
  std::string error;
  if (!preferences->commit(draft, error)) {
    const auto& saved =
        dark ? preferences->active().darkReading : preferences->active().lightReading;
    host->setReadingColors(saved);
    window->previewColors->sync(saved, dark);
    window->previewColors->showError(error);
  }
}
} // namespace xfmd
