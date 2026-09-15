#include "Application.h"
#include "navigation/LinkResolver.h"
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
  window->editor->setReadingColors(colors);
  window->previewColors->sync(colors, dark);
  app.refresh();
}
void Application::changeReadingColors(const ReadingColors& colors, bool commit) {
  host->setReadingColors(colors);
  window->editor->setReadingColors(colors);
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
    window->editor->setReadingColors(saved);
    window->status->setText(("Colors not saved: " + error).c_str());
  }
}
void Application::showLinkTarget(const std::string& target) {
  if (target.empty()) {
    if (!hoverStatus.empty() && window->status->getText() == hoverStatus.c_str())
      window->status->setText(beforeHover.c_str());
    hoverStatus.clear();
    return;
  }
  if (hoverStatus.empty() || window->status->getText() != hoverStatus.c_str())
    beforeHover = window->status->getText().text();
  try {
    hoverStatus = ExternalBrowser::accepts(target)
                      ? target
                      : LinkResolver::localPath(session.view().path, target);
  } catch (const std::exception&) {
    hoverStatus = target;
  }
  window->status->setText(hoverStatus.c_str());
}
} // namespace xfmd
