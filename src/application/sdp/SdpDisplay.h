#pragma once
#include "SdpProtocol.h"
#include <optional>
namespace xfmd {
// Presentation-only projection. It never changes producer identities or parses SDL.
std::vector<std::string> sdpDisplayChildren(const SdpReply&, const SdpNode&);
struct SdpDisplayRow {
  std::string id, node, label, source;
  std::string diagnostic;
  SdpTarget target;
  std::vector<std::string> ancestors;
  bool terminal = false, truncated = false;
};
std::optional<SdpDisplayRow> sdpDisplayRow(const SdpReply&, const std::string&,
                                         std::vector<std::string>, const std::string& source = {});
bool sdpDisplayAction(const SdpTarget&);
} // namespace xfmd
