#pragma once
#include <map>
#include <string>
#include <vector>
namespace xfmd {
struct SdpTarget {
  std::string operation, project, model, uri, revision, path, entry;
};
struct SdpNode {
  std::string id, kind, label, state, diagnostic, reference, cardType;
  std::vector<std::string> children;
  SdpTarget target;
};
struct SdpSource {
  std::string id, kind, path, revision;
};
struct SdpReply {
  std::string operation, status, root, area, project, revision, entry, directory;
  std::string source, diagnostic;
  std::map<std::string, SdpNode> nodes;
  std::vector<std::string> roots;
  std::map<std::string, SdpSource> sources;
  std::size_t depthLimit = 32;
};
// Throws a user-visible diagnostic on malformed/unsupported/oversized replies.
SdpReply decodeSdpReply(const std::string&);
std::string sdpError(const std::string&);
std::string sdpArea(const std::string& root);
bool sdpContained(const std::string& directory, const std::string& path);
} // namespace xfmd
