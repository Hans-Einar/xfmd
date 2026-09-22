#include "NavigationPanel.h"
#include "application/composition/DiagramServices.h"
#include "renderer/MarkdownRenderer.h"
namespace xfmd {
NavigationPanel::NavigationPanel(FX::FXComposite* parent, FX::FXApp& app)
    : interpreter(DiagramServices::interpreter()), renderer(std::make_unique<MarkdownRenderer>()),
      scheduler(app), host(new FoxRenderHost(parent, *renderer, metrics)) {
  preview = std::make_unique<PreviewCoordinator>(session, *interpreter, *renderer, metrics,
                                                scheduler, DiagramServices::preview());
  preview->invalidated = [this](auto token) { host->expect(token); scrolling.invalidate(token); };
  preview->layoutRequested = [this](auto key) { host->expectLayout(key); scrolling.expectLayout(key); };
  preview->present = [this](auto frame) { host->present(frame); scrolling.setFrame(frame); };
  preview->failed = [this](auto message) { if (error) error(message); };
  host->resized = [this](double width) { preview->relayout(width); };
  host->viewportChanged = [this](double y) {
    scrolling.onViewportChanged(ViewOrigin::Preview, y, session.view().token, 0, false, host->lastScrollOrigin);
  };
  scrolling.setPreview = [this](double y) { host->setViewport(y); };
  documents.error = navigation.error = [this](auto message) { if (error) error(message); };
  documents.opened = [this] { navigation.commitVisit(); preview->refresh(); };
  host->create();
}
NavigationPanel::~NavigationPanel() { preview.reset(); scheduler.cancelAll(); delete host; }
bool NavigationPanel::open(const std::string& path) { return navigation.openTarget(path); }
}
