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
  if (Json{doc.get()}.get("schema").text(true) != "sdptool/0.2")
    throw std::runtime_error("Unsupported SDP schema; install a compatible SDPTool 2.x");
  return doc;
}
} // namespace
SdpReply decodeSdpReply(const std::string& bytes) {
  auto doc = parse(bytes);
  Json value{doc.get()};
  SdpReply result;
  result.operation = value.get("operation").text(true);
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
    value = value.get("navigation");
    if (value.get("schema").text(true) != "sdptool/0.2" ||
        value.get("operation").text(true) != "tree")
      throw std::runtime_error("Missing SDP navigation snapshot");
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
