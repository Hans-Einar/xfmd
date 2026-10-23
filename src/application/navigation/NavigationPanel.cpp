#include "NavigationPanel.h"
#include "HeadingAnchor.h"
#include "LinkResolver.h"
#include "application/composition/DiagramServices.h"
#include "renderer/MarkdownRenderer.h"
#include <algorithm>
#include <cctype>
namespace xfmd {
NavigationPanel::NavigationPanel(FX::FXComposite* parent, FX::FXApp& app)
    : interpreter(DiagramServices::interpreter()), renderer(std::make_unique<MarkdownRenderer>()),
      scheduler(app), host(new FoxRenderHost(parent, *renderer, metrics)) {
  preview = std::make_unique<PreviewCoordinator>(session, *interpreter, *renderer, metrics,
                                                 scheduler, DiagramServices::preview());
  preview->invalidated = [this](auto token) {
    host->expect(token);
    scrolling.invalidate(token);
  };
  preview->layoutRequested = [this](auto key) {
    host->expectLayout(key);
    scrolling.expectLayout(key);
  };
  preview->present = [this](auto frame) {
    host->present(frame);
    scrolling.setFrame(frame);
  };
  preview->failed = [this](auto message) {
    if (error)
      error(message);
  };
  host->resized = [this](double width) { preview->relayout(width); };
  host->viewportChanged = [this](double y) {
    scrolling.onViewportChanged(ViewOrigin::Preview, y, session.view().token, 0, false,
                                host->lastScrollOrigin);
  };
  scrolling.setPreview = [this](double y) { host->setViewport(y); };
  documents.error = navigation.error = [this](auto message) {
    if (error)
      error(message);
  };
  documents.opened = [this] {
    navigation.commitVisit();
    preview->refresh();
    if (changed)
      changed();
  };
  preview->modelReady = [this](auto model) { applyFragment(*model); };
  host->create();
}
NavigationPanel::~NavigationPanel() {
  preview.reset();
  scheduler.cancelAll();
  delete host;
}
void NavigationPanel::applyFragment(const SemanticDocument& model) {
  if (!pendingFragment)
    return;
  const auto fragment = std::move(*pendingFragment);
  pendingFragment.reset();
  if (auto anchor = HeadingAnchor::find(model, fragment))
    scrolling.restoreAnchor(*anchor);
  else if (error)
    error("Heading anchor not found: " + fragment);
}
bool NavigationPanel::follow(const std::string& link) {
  try {
    const auto target = LinkResolver::documentLink(session.view().path, link);
    pendingFragment.reset();
    if (!target.fragment)
      return navigation.openTarget(target.path);
    if (!navigation.openAt(target.path, scrolling.captureAnchor()))
      return false;
    pendingFragment = target.fragment;
    if (auto model = preview->currentModel(); model && model->token == session.view().token)
      applyFragment(*model);
    return true;
  } catch (const std::exception& e) {
    if (error)
      error(e.what());
    return false;
  }
}
bool NavigationPanel::open(const std::string& path) {
  pendingFragment.reset();
  return navigation.openTarget(path);
}
} // namespace xfmd
