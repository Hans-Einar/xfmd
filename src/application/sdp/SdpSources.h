#pragma once
#include "SdpProtocol.h"
namespace xfmd {
// Catalog kind and source suffix must agree before exposing original text.
bool sdpTextSource(const SdpSource&);
// Empty means missing/ambiguous association, never a path inferred from a label.
std::string sdpSourceForNode(const SdpReply&, const SdpNode&, const std::string& inherited);
SdpTarget sdpSourceTarget(const SdpReply&, const std::string& sourceId);
} // namespace xfmd
