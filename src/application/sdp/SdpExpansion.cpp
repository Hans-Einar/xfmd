#include "SdpProtocol.h"
#include <stdexcept>
namespace xfmd {
SdpReply expandSdpPage(const SdpReply& current, const SdpReply& page, const SdpTarget& target) {
  auto fail = [] { throw std::runtime_error("Invalid or stale SDP page expansion; Refresh"); };
  auto parent = current.nodes.end();
  for (auto it = current.nodes.begin(); it != current.nodes.end(); ++it) {
    const auto& t = it->second.expansionTarget;
    if (t.uri == target.uri && t.operation == target.operation && t.revision == target.revision) {
      if (parent != current.nodes.end()) fail();
      parent = it;
    }
  }
  if (parent == current.nodes.end() || !parent->second.childrenDeferred ||
      target.operation != "activity-tree" || page.operation != "activity-tree" ||
      target.project != current.project || page.project != current.project ||
      page.revision != target.revision || !page.model.empty() ||
      page.roots != std::vector<std::string>{parent->first}) fail();
  const auto replacement = page.nodes.find(parent->first);
  if (replacement == page.nodes.end() || replacement->second.childrenDeferred ||
      !replacement->second.expansionTarget.operation.empty() ||
      replacement->second.target.uri != parent->second.target.uri ||
      replacement->second.target.operation != parent->second.target.operation ||
      replacement->second.target.revision != parent->second.target.revision) fail();
  const auto prefix = parent->first + "/";
  for (const auto& [id,n] : page.nodes) {
    if (id != parent->first && (id.rfind(prefix,0) != 0 || current.nodes.count(id))) fail();
    if (!n.reference.empty() && !page.nodes.count(n.reference)) fail();
    for (const auto& child : n.children)
      if (child.rfind(prefix,0) != 0 || !page.nodes.count(child)) fail();
    for (const auto* t : {&n.target,&n.expansionTarget})
      if (!t->operation.empty() && (t->project != target.project ||
          (t->operation != "open" && t->revision != target.revision))) fail();
  }
  auto merged = current;
  merged.nodes.at(parent->first) = replacement->second;
  for (const auto& [id,n] : page.nodes)
    if (id != parent->first) merged.nodes.emplace(id,n);
  return merged;
}
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
