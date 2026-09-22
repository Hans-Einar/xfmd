#include "NavigationPanel.h"
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
  preview->modelReady = [this](auto model) {
    if (pendingFragment.empty())
      return;
    for (const auto& block : model->blocks) {
      if (block.kind != BlockKind::Heading)
        continue;
      std::string id;
      for (const auto& run : block.runs)
        id += run.text;
      std::transform(id.begin(), id.end(), id.begin(),
                     [](unsigned char c) { return char(std::tolower(c)); });
      if (id == pendingFragment) {
        scrolling.restoreAnchor({block.source.begin});
        pendingFragment.clear();
        return;
      }
    }
    if (error)
      error("Heading anchor not found: " + pendingFragment);
    pendingFragment.clear();
  };
  host->create();
}
NavigationPanel::~NavigationPanel() {
  preview.reset();
  scheduler.cancelAll();
  delete host;
}
bool NavigationPanel::follow(const std::string& link) {
  auto pos = link.find('#');
  if (pos == std::string::npos)
    return navigation.followLink(link);
  auto fragment = link.substr(pos + 1);
  if (fragment.empty() || fragment.find_first_not_of(
                              "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") !=
                              std::string::npos) {
    if (error)
      error("Unsupported heading anchor");
    return false;
  }
  pendingFragment = fragment;
  bool result = pos == 0 ? navigation.openTarget(session.view().path)
                         : navigation.followLink(link.substr(0, pos));
  if (!result)
    pendingFragment.clear();
  return result;
}
bool NavigationPanel::open(const std::string& path) { return navigation.openTarget(path); }
} // namespace xfmd
