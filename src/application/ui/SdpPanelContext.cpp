#include "SdpPanel.h"
#include "application/sdp/SdpSources.h"
using namespace FX;
namespace xfmd {
namespace {
class SourceChoice : public FXObject {
public:
  bool chosen = false;
  long handle(FXObject*, FXSelector selector, void*) override {
    if (FXSELTYPE(selector) != SEL_COMMAND)
      return 0;
    chosen = true;
    return 1;
  }
};
} // namespace
SdpPanel::~SdpPanel() { getApp()->removeTimeout(this, ID_APPLY); }
void SdpPanel::context(const std::string& source, const FXEvent& event) {
  const auto target = sdpSourceTarget(snapshot, source);
  if (target.model.empty() || inContext)
    return;
  const auto version = generation;
  inContext = true;
  SourceChoice choice;
  FXMenuPane menu(getShell());
  new FXMenuCommand(&menu, "View source", nullptr, &choice, 1);
  menu.create();
  menu.popup(nullptr, event.root_x, event.root_y);
  menu.grabKeyboard();
  getApp()->runModalWhileShown(&menu);
  if (menu.grabbedKeyboard())
    menu.ungrabKeyboard();
  // A refresh/root change during modal dispatch invalidates the copied action.
  if (choice.chosen && version == generation && sourceRequested)
    sourceRequested(target);
  else if (choice.chosen)
    message("SDP navigation changed; choose View source again");
  // Keep the dispatching tree alive until its right-button handler has returned.
  getApp()->addTimeout(this, ID_APPLY, 0);
}
long SdpPanel::handle(FXObject* sender, FXSelector selector, void* data) {
  if (FXSELTYPE(selector) == SEL_TIMEOUT && FXSELID(selector) == ID_APPLY) {
    inContext = false;
    auto next = std::move(deferred);
    deferred.reset();
    const bool reset = deferredClear;
    deferredClear = false;
    if (reset)
      clear();
    if (next)
      present(std::move(*next));
    return 1;
  }
  return FXVerticalFrame::handle(sender, selector, data);
}
} // namespace xfmd
