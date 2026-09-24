#include "Application.h"
#include "adapters/FoxWheelScrollBar.h"
namespace xfmd {
void Application::wireZoom() {
  zoom = std::make_unique<DocumentZoom>(*window->editor, *host, *preview, scrolling, *views);
  auto step = [this](double value) { zoom->step(value); };
  window->editor->zoomRequested = step;
  host->zoomRequested = step;
  for (FX::FXScrollArea* area :
       {static_cast<FX::FXScrollArea*>(window->editor), static_cast<FX::FXScrollArea*>(host)})
    for (auto* bar : {area->horizontalScrollBar(), area->verticalScrollBar()})
      static_cast<FoxWheelScrollBar*>(bar)->zoomRequested = step;
  host->geometryChanged = [this] { zoom->refresh(); };
  window->editor->geometryChanged = [this] { zoom->refresh(); };
  zoom->changed = [this](double percent, ZoomMode mode) {
    window->previewControls->sync(percent, mode);
  };
  zoom->refresh();
}
} // namespace xfmd
