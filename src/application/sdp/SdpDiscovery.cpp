#include "SdpNavigation.h"
#include "application/Application.h"
#include <algorithm>
namespace xfmd {
namespace fs = std::filesystem;
void SdpNavigation::discover() {
  expansions.clear();
  const auto previousArea = area;
  area = sdpArea(root);
  if (previousArea != area) {
    project.clear();
    sources.clear();
  }
  auto* workspace = app.window->workspacePanel;
  workspace->showSdp(!area.empty());
  if (area.empty()) {
    workspace->sdp->clear();
    return;
  }
  message("Reading SDP project…");
  start("actions", {root, "actions"});
}
void SdpNavigation::readDiscovery() {
  std::vector<std::string> args{root, "discover"};
  if (lazySdl) args.insert(args.end(), {"--navigation", "lazy-sdl"});
  start("discover", std::move(args));
}
void SdpNavigation::expandModel(const SdpTarget& target) {
  const auto source = sources.find(target.model);
  if (!lazySdl || target.operation != "tree" || target.project != project ||
      source == sources.end() || source->second.kind != "sdl" ||
      source->second.revision != target.revision) {
    message("SDP model changed; Refresh before expanding");
    return;
  }
  if (expanding.model == target.model || std::any_of(expansions.begin(), expansions.end(),
      [&](const auto& t) { return t.model == target.model; })) return;
  expansions.push_back(target);
  nextExpansion();
}
void SdpNavigation::nextExpansion() {
  if (!pendingOperation.empty() || expansions.empty() || refreshQueued) return;
  expanding = expansions.front();
  expansions.erase(expansions.begin());
  message("Loading SDL model…");
  start("tree", {root, "tree", "--model", expanding.model, "--revision",
                 expanding.revision, "--model-only"});
}
void SdpNavigation::poll() {
  auto result = job.take();
  if (!result) {
    app.scheduler->restart(9, 40, [this] { poll(); });
    return;
  }
  try {
    if (!result->error.empty()) {
      // Old released producers explicitly reject the actions command. Never
      // retry eager discovery for transport, schema, model or limit failures.
      if (pendingOperation == "actions" && result->error.rfind("arguments:", 0) == 0) {
        lazySdl = false;
        readDiscovery();
        return;
      }
      throw std::runtime_error(result->error);
    }
    auto& reply = *result->reply;
    if (reply.operation != pendingOperation)
      throw std::runtime_error("SDP response does not match the requested operation");
    if (reply.operation == "actions") {
      lazySdl = reply.lazySdl;
      readDiscovery();
      return;
    }
    if (reply.operation == "discover") {
      if (lazySdl && reply.navigationMode != "lazy-sdl")
        throw std::runtime_error("SDP producer did not return deferred navigation");
      if (fs::canonical(reply.area) != fs::canonical(area) ||
          fs::canonical(reply.root) != fs::canonical(area).parent_path())
        throw std::runtime_error("SDP discovery returned another project root");
      expansions.clear();
      project = reply.project;
      sources = reply.sources;
      app.window->workspacePanel->sdp->present(std::move(reply));
      app.window->status->setText(app.window->workspacePanel->sdp->status->getText());
    } else if (reply.operation == "tree") {
      app.window->workspacePanel->sdp->mergeModel(reply, expanding);
      app.window->status->setText(app.window->workspacePanel->sdp->status->getText());
    } else {
      deliver(reply);
    }
  } catch (const std::exception& e) {
    message(std::string("SDP: ") + e.what());
  }
  discardPending();
  pendingOperation.clear();
  expanding = {};
  nextExpansion();
  if (!watchError.empty())
    message("SDP automatic refresh unavailable: " + watchError);
}
} // namespace xfmd
