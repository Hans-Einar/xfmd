#include "SdpProtocol.h"
#include <stdexcept>
namespace xfmd {
SdpReply expandSdpModel(const SdpReply& current, const SdpReply& model, const SdpTarget& target) {
  auto fail = [] { throw std::runtime_error("Invalid or stale SDP model expansion; Refresh"); };
  const auto id = "sdl/" + target.model;
  const auto prefix = id + "/";
  auto source = current.sources.find(target.model);
  auto node = current.nodes.find(id);
  if (model.operation != "tree" || target.operation != "tree" ||
      model.project != current.project || target.project != current.project ||
      model.model != target.model || model.revision != target.revision ||
      source == current.sources.end() || source->second.kind != "sdl" ||
      source->second.revision != target.revision || node == current.nodes.end() ||
      !node->second.childrenDeferred || model.roots != std::vector<std::string>{"sdl"} ||
      !model.nodes.count("sdl") || model.nodes.at("sdl").kind != "tab") fail();
  auto validID = [&](const std::string& value) { return value.rfind(prefix, 0) == 0; };
  for (const auto& [key, n] : model.nodes) {
    if (key != "sdl" && (!validID(key) || current.nodes.count(key))) fail();
    if (n.childrenDeferred || (!n.reference.empty() &&
        (!validID(n.reference) || !model.nodes.count(n.reference)))) fail();
    for (const auto& child : n.children)
      if (!validID(child) || !model.nodes.count(child)) fail();
    for (const auto* t : {&n.target, &n.overviewTarget})
      if (!t->operation.empty() && (t->operation != "select" ||
          t->project != target.project || t->model != target.model ||
          t->revision != target.revision || !t->path.empty() || !t->entry.empty() ||
          t->uri.rfind("sdl-view://" + target.project + "/", 0) != 0)) fail();
  }
  auto merged = current; // Validate before replacing any visible snapshot.
  for (const auto& [key, n] : model.nodes)
    if (key != "sdl") merged.nodes.emplace(key, n);
  auto& parent = merged.nodes.at(id);
  parent.children = model.nodes.at("sdl").children;
  parent.childrenDeferred = false;
  return merged;
}
} // namespace xfmd
