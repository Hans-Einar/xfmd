#include "SdpProtocol.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <set>
#include <stdexcept>
namespace xfmd {
namespace {
extern "C" {
void* xfmd_sdp_json_parse(const char*, std::size_t);
void xfmd_sdp_json_free(void*);
const void* xfmd_sdp_json_get(const void*, const char*, std::size_t);
const void* xfmd_sdp_json_at(const void*, std::size_t);
std::size_t xfmd_sdp_json_size(const void*);
const char* xfmd_sdp_json_text(const void*, std::size_t*);
bool xfmd_sdp_json_boolean(const void*, bool*);
bool xfmd_sdp_json_unsigned(const void*, std::uint64_t*);
}
struct Json {
  const void* value;
  Json get(const char* name) const { return {xfmd_sdp_json_get(value, name, std::strlen(name))}; }
  std::string text(bool required = false) const {
    if (!value && !required)
      return {};
    std::size_t size = 0;
    const auto* data = xfmd_sdp_json_text(value, &size);
    if (!data || size > 16384 || (required && !size))
      throw std::runtime_error("Invalid SDP string field");
    std::string result(data, size);
    // Text travels through native controls and argv; no embedded control characters.
    for (unsigned char c : result)
      if (c < 32 || c == 127)
        throw std::runtime_error("Control character in SDP response");
    return result;
  }
  std::vector<Json> array(std::size_t limit) const {
    auto size = xfmd_sdp_json_size(value);
    if (size > limit)
      throw std::runtime_error("Invalid or oversized SDP array");
    std::vector<Json> result;
    for (std::size_t i = 0; i < size; ++i)
      result.push_back({xfmd_sdp_json_at(value, i)});
    return result;
  }
};
using Document = std::unique_ptr<void, decltype(&xfmd_sdp_json_free)>;
Document parse(const std::string& bytes) {
  Document doc(xfmd_sdp_json_parse(bytes.data(), bytes.size()), xfmd_sdp_json_free);
  if (!doc)
    throw std::runtime_error("Invalid SDP JSON (expected sdptool/0.2 --json)");
  if (auto schema = Json{doc.get()}.get("schema").text(true);
      schema != "sdptool/0.2" && schema != "sdptool.actions/1")
    throw std::runtime_error("Unsupported SDP schema; install a compatible SDPTool 2.x");
  return doc;
}
} // namespace
SdpReply decodeSdpReply(const std::string& bytes) {
  auto doc = parse(bytes);
  Json value{doc.get()};
  SdpReply result;
  result.operation = value.get("operation").text(true);
  if (result.operation == "actions") {
    if (value.get("schema").text(true) != "sdptool.actions/1")
      throw std::runtime_error("Invalid SDP action catalogue");
    bool discovery = false, expansion = false;
    for (auto action : value.get("actions").array(4096)) {
      const auto id = action.get("id").text(true);
      if (id == "project.discover") {
        auto options = action.get("inputSchema").get("properties").get("navigation").get("enum");
        if (options.value)
          for (auto option : options.array(128)) discovery |= option.text() == "lazy-sdl";
      }
      if (id == "project.model-tree") expansion = action.get("mutation").text(true) == "none";
    }
    result.lazySdl = discovery && expansion;
    return result;
  }
  if (value.get("schema").text(true) != "sdptool/0.2")
    throw std::runtime_error("Invalid SDP operation schema");
  if (result.operation == "discover" || result.operation == "tree") {
    if (result.operation == "discover") {
    result.status = value.get("status").text(true);
    result.root = value.get("root").text(true);
    result.area = value.get("area").text(true);
    if (result.status != "valid")
      throw std::runtime_error("SDP discovery is " + result.status +
                               "; repair the project and Refresh");
    if (auto sources = value.get("sources"); sources.value) {
      for (auto item : sources.array(256)) {
        SdpSource source{item.get("id").text(true), item.get("kind").text(true),
                         item.get("source").text(true), item.get("revision").text()};
        const std::filesystem::path path(source.path);
        if (path.is_absolute() || path.filename().empty() ||
            std::any_of(path.begin(), path.end(), [](const auto& part) { return part == ".."; }))
          throw std::runtime_error("Invalid SDP source path");
        if (!result.sources.emplace(source.id, std::move(source)).second)
          throw std::runtime_error("Duplicate SDP source identity");
      }
    }
    result.navigationMode = value.get("navigationMode").text();
    if (!result.navigationMode.empty() && result.navigationMode != "lazy-sdl")
      throw std::runtime_error("Unsupported SDP navigation mode");
    value = value.get("navigation");
    if (value.get("schema").text(true) != "sdptool/0.2" ||
        value.get("operation").text(true) != "tree")
      throw std::runtime_error("Missing SDP navigation snapshot");
    } else {
      result.model = value.get("model").text(true);
      result.revision = value.get("revision").text(true);
      result.sources[result.model] = {result.model, "sdl", "", result.revision};
    }
    result.project = value.get("project").text(true);
    if (auto depth = value.get("expansionDepthLimit"); depth.value) {
      std::uint64_t limit = 0;
      if (!xfmd_sdp_json_unsigned(depth.value, &limit) || !limit)
        throw std::runtime_error("Invalid SDP expansion depth limit");
      result.depthLimit = std::min<std::uint64_t>(32, limit);
    }
    std::set<std::string> roots;
    for (auto root : value.get("roots").array(32)) {
      auto id = root.text(true);
      if (!roots.insert(id).second)
        throw std::runtime_error("Duplicate SDP navigation root");
      result.roots.push_back(std::move(id));
    }
    std::size_t edges = 0;
    for (const auto& item : value.get("nodes").array(20000)) {
      SdpNode node;
      if (auto deferred = item.get("childrenDeferred"); deferred.value &&
          !xfmd_sdp_json_boolean(deferred.value, &node.childrenDeferred))
        throw std::runtime_error("Invalid SDP deferred marker");
      node.id = item.get("id").text(true);
      node.label = item.get("label").text(true);
      node.kind = item.get("kind").text(true);
      node.state = item.get("state").text();
      node.diagnostic = item.get("diagnostic").text();
      node.reference = item.get("reference").text();
      node.cardType = item.get("externalReference").text();
      if (auto children = item.get("children"); children.value)
        for (auto child : children.array(20000)) {
          if (++edges > 100000)
            throw std::runtime_error("SDP graph edge limit exceeded");
          node.children.push_back(child.text(true));
        }
      auto target = item.get("target");
      if (target.value) {
        node.target = {target.get("operation").text(true), target.get("project").text(true),
                       target.get("model").text(),         target.get("uri").text(),
                       target.get("revision").text(),      target.get("path").text(),
                       target.get("entry").text()};
        if (node.target.project != result.project)
          throw std::runtime_error("SDP target belongs to another project");
      }
      if (auto overview = item.get("overviewTarget"); overview.value) {
        node.overviewTarget = {overview.get("operation").text(true), overview.get("project").text(true),
                              overview.get("model").text(true), overview.get("uri").text(true),
                              overview.get("revision").text(true), overview.get("path").text(),
                              overview.get("entry").text()};
        const auto& t = node.overviewTarget;
        auto source = result.sources.find(t.model);
        if (node.kind != "viewpoint" || t.operation != "select" || t.project != result.project ||
            t.uri.rfind("sdl-view://" + result.project + "/", 0) != 0 ||
            !t.path.empty() || !t.entry.empty() || source == result.sources.end() ||
            source->second.kind != "sdl" || source->second.revision != t.revision ||
            (!node.target.model.empty() && node.target.model != t.model))
          throw std::runtime_error("Invalid or stale SDP overview target; Refresh");
      }
      if (node.childrenDeferred) {
        auto source = result.sources.find(node.target.model);
        if (result.navigationMode != "lazy-sdl" || node.kind != "source" ||
            node.state != "validated" || !node.children.empty() || !node.reference.empty() ||
            node.target.operation != "tree" || source == result.sources.end() ||
            source->second.kind != "sdl" || node.target.revision.empty() ||
            node.target.revision != source->second.revision ||
            node.id != "sdl/" + node.target.model)
          throw std::runtime_error("Invalid SDP deferred source");
      }
      if (!result.nodes.emplace(node.id, std::move(node)).second)
        throw std::runtime_error("Duplicate SDP node identity");
    }
    for (const auto& pair : result.nodes) {
      const auto& node = pair.second;
      for (const auto& child : node.children)
        if (!result.nodes.count(child))
          throw std::runtime_error("Missing SDP child node");
      if (!node.reference.empty() && !result.nodes.count(node.reference))
        throw std::runtime_error("Missing SDP reference node");
    }
    for (const auto& id : result.roots)
      if (!result.nodes.count(id))
        throw std::runtime_error("Missing SDP navigation root");
  } else if (result.operation == "sdui-check") {
    result.status = value.get("status").text(true);
    result.revision = value.get("revision").text(true);
    result.entry = value.get("entry").text(true);
    result.source = value.get("source").text(true);
    result.diagnostic = value.get("diagnostic").text(true);
    if (result.status != "prototype" && result.status != "prototype-unbound")
      throw std::runtime_error("Unsupported SDUI readiness status");
  } else if (result.operation == "select" || result.operation == "sdui-preview" ||
             result.operation == "preview") {
    result.entry = value.get("entry").text(true);
    result.directory = value.get("directory").text(true);
    result.revision = value.get("revision").text(true);
    if (auto links = value.get("sourceLinks"); links.value) {
      std::set<std::string> ids, paths, snapshots;
      for (auto item : links.array(128)) {
        SdpSourceLink link{item.get("id").text(true), item.get("path").text(true),
                           item.get("snapshot").text(true), item.get("revision").text(true)};
        const std::filesystem::path path(link.path), snapshot(link.snapshot);
        auto parent = [](const std::filesystem::path& p) {
          return std::any_of(p.begin(), p.end(), [](const auto& part) { return part == ".."; });
        };
        if (result.operation == "sdui-preview" || !path.is_absolute() || parent(path) ||
            path.extension() != ".design" || snapshot.is_absolute() || parent(snapshot) ||
            snapshot.lexically_normal() != snapshot || snapshot.extension() != ".design" ||
            link.snapshot.rfind("source-inputs/", 0) != 0 ||
            link.id.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos ||
            link.revision.size() != 64 || link.revision.find_first_not_of("0123456789abcdef") != std::string::npos ||
            !ids.insert(link.id).second || !paths.insert(link.path).second ||
            !snapshots.insert(link.snapshot).second)
          throw std::runtime_error("Invalid SDL source-link metadata");
        result.sourceLinks.push_back(std::move(link));
      }
    }
  } else {
    throw std::runtime_error("Unsupported SDP response operation: " + result.operation);
  }
  return result;
}
std::string sdpError(const std::string& bytes) {
  try {
    auto doc = parse(bytes);
    auto error = Json{doc.get()}.get("error");
    return error.get("code").text(true) + ": " + error.get("message").text(true);
  } catch (...) {
    std::string result = bytes.substr(0, 1000);
    for (char& c : result)
      if (static_cast<unsigned char>(c) < 32 || c == 127)
        c = ' ';
    return result.empty() ? "SDP tool failed without a diagnostic" : result;
  }
}
std::string sdpArea(const std::string& root) {
  namespace fs = std::filesystem;
  std::error_code ec;
  fs::path path(root);
  if (path.filename() != "SDP")
    path /= "SDP";
  const auto status = fs::symlink_status(path, ec);
  if (ec || !fs::is_directory(status) || fs::is_symlink(status))
    return {};
  return fs::canonical(path, ec).string();
}
bool sdpContained(const std::string& directory, const std::string& path) {
  namespace fs = std::filesystem;
  std::error_code ec;
  if (!fs::path(path).is_absolute())
    return false;
  const auto base = fs::canonical(directory, ec);
  if (ec)
    return false;
  const auto candidate = fs::canonical(path, ec);
  if (ec)
    return false;
  auto b = base.begin(), p = candidate.begin();
  for (; b != base.end(); ++b, ++p)
    if (p == candidate.end() || *b != *p)
      return false;
  return p != candidate.end();
}
} // namespace xfmd
