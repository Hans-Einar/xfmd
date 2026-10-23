#include "SdpSources.h"
#include <filesystem>
namespace xfmd {
bool sdpTextSource(const SdpSource& source) {
  const auto suffix = std::filesystem::path(source.path).extension();
  return (source.kind == "sdl" && suffix == ".design") ||
         (source.kind == "sdui" && suffix == ".sdui");
}
std::string sdpSourceForNode(const SdpReply& reply, const SdpNode& node,
                             const std::string& inherited) {
  std::string id = node.target.model;
  if (node.kind == "source") {
    // SDPTool 2.x source-node convention also covers invalid rows with no target.
    // Compare complete returned identities, never split a path/label into a model ID.
    for (const auto& pair : reply.sources) {
      if (sdpTextSource(pair.second) && node.id == pair.second.kind + "/" + pair.first) {
        if (!id.empty() && id != pair.first)
          return {};
        id = pair.first;
        break;
      }
    }
  }
  if (!id.empty() && !inherited.empty() && id != inherited)
    return {};
  if (id.empty())
    id = inherited;
  auto found = reply.sources.find(id);
  if (found == reply.sources.end() || !sdpTextSource(found->second))
    return {};
  return id;
}
SdpTarget sdpSourceTarget(const SdpReply& reply, const std::string& id) {
  auto found = reply.sources.find(id);
  if (found == reply.sources.end() || !sdpTextSource(found->second))
    return {};
  return {"open-source",
          reply.project,
          id,
          {},
          found->second.revision,
          (std::filesystem::path(reply.root) / found->second.path).lexically_normal().string(),
          {}};
}
} // namespace xfmd
