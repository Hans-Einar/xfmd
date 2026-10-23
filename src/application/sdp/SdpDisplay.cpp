#include "SdpDisplay.h"
#include "SdpSources.h"
#include <algorithm>
#include <set>
namespace xfmd {
namespace {
bool healthy(const SdpNode& n) {
  return n.diagnostic.empty() &&
         (n.state.empty() || n.state == "available" || n.state == "validated");
}
const SdpNode* resolve(const SdpReply& r, const std::string& id) {
  std::set<std::string> visited;
  auto it = r.nodes.find(id);
  while (it != r.nodes.end() && visited.insert(it->first).second) {
    if (it->second.reference.empty() || it->second.target.operation == "open")
      return &it->second;
    it = r.nodes.find(it->second.reference);
  }
  return nullptr;
}
} // namespace
bool sdpDisplayAction(const SdpTarget& t) {
  return t.operation == "open" || t.operation == "select" ||
         (t.operation == "sdui-preview" && !t.entry.empty());
}
std::vector<std::string> sdpDisplayChildren(const SdpReply& r, const SdpNode& parent) {
  // Only SDL source/System presentation wrappers are transparent. Other tabs and
  // unfamiliar producer groups retain their own topology and diagnostics.
  const auto source = r.sources.find(sdpSourceForNode(r, parent, {}));
  if ((parent.kind != "source" && parent.kind != "system") ||
      source == r.sources.end() || source->second.kind != "sdl")
    return parent.children;
  std::vector<std::string> output;
  std::set<std::string> wrappers, viewpoints;
  std::vector<std::string> pending(parent.children.rbegin(), parent.children.rend());
  while (!pending.empty()) {
    auto id = std::move(pending.back());
    pending.pop_back();
    const auto& n = r.nodes.at(id);
    if ((n.kind == "phase" || n.kind == "abstraction") && healthy(n)) {
      if (wrappers.insert(id).second)
        pending.insert(pending.end(), n.children.rbegin(), n.children.rend());
      continue;
    }
    const auto* canonical = resolve(r, id);
    if (canonical && canonical->kind == "viewpoint" && healthy(n) &&
        n.target.operation.empty()) {
      if (viewpoints.insert(canonical->id).second)
        output.push_back(canonical->id);
    } else if (n.kind != "viewpoint" || viewpoints.insert(id).second) {
      output.push_back(id);
    }
  }
  return output;
}
std::optional<SdpDisplayRow> sdpDisplayRow(const SdpReply& r, const std::string& id,
                                         std::vector<std::string> ancestors,
                                         const std::string& inherited) {
  auto found = r.nodes.find(id);
  if (found == r.nodes.end())
    return {};
  const auto* node = &found->second;
  SdpDisplayRow row{id, id, node->label, sdpSourceForNode(r, *node, inherited),
                    node->diagnostic, node->target, std::move(ancestors)};
  bool reference = false, sourceConflict = false;
  std::set<std::string> chain;
  for (;;) {
    if (!chain.insert(node->id).second) {
      row.terminal = true;
      break;
    }
    if (std::find(row.ancestors.begin(), row.ancestors.end(), node->id) != row.ancestors.end())
      row.terminal = true;
    if (row.ancestors.size() >= r.depthLimit) {
      row.terminal = true;
      row.truncated = true;
    }
    if (!row.terminal)
      row.ancestors.push_back(node->id);
    row.node = node->id;
    if (row.target.operation.empty())
      row.target = node->target;
    if (node->reference.empty() || node->target.operation == "open")
      break;
    reference = true;
    node = &r.nodes.at(node->reference);
    if (!row.source.empty() && !node->target.model.empty() && row.source != node->target.model)
      sourceConflict = true;
    row.source = sourceConflict ? "" : sdpSourceForNode(r, *node, row.source);
  }
  if (row.terminal && !sdpDisplayAction(row.target) && healthy(*node) && healthy(found->second))
    return {};
  if (node->id != id && !node->diagnostic.empty())
    row.diagnostic += (row.diagnostic.empty() ? "" : "\n") + node->diagnostic;
  if (found->second.state == "invalid" || found->second.state == "unsupported" ||
      found->second.state == "context-required")
    row.label += " [" + found->second.state + "]";
  if (reference) {
    if (node->kind == "viewpoint")
      row.label = node->label;
    else
      row.label += " [reference]";
  } else if (!node->reference.empty() && node->target.operation == "open") {
    row.label += " [Ref: " + node->reference + "]";
  }
  if (node->id != id && (node->state == "invalid" || node->state == "context-required" || node->state == "unsupported"))
    row.label += " [" + node->state + "]";
  if (!node->cardType.empty())
    row.label += " [Ref: " + node->cardType + "]";
  return row;
}
} // namespace xfmd
