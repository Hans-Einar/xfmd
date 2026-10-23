#include "application/sdp/SdpProtocol.h"
#include "application/sdp/SdpSources.h"
#include "support/TestSupport.h"
using namespace xfmd;
std::string discover(const std::string& navigation, const std::string& sources = "[]") {
  return R"({"schema":"sdptool/0.2","operation":"discover","status":"valid","root":"/project","area":"/project/SDP","sources":)" +
         sources + R"(,"navigation":{"schema":"sdptool/0.2","operation":"tree","project":"TEST",)" +
         navigation + "}}";
}
void reject(const std::string& bytes) {
  bool rejected = false;
  try {
    decodeSdpReply(bytes);
  } catch (const std::exception&) {
    rejected = true;
  }
  CHECK(rejected);
}
void run() {
  const std::string tabs =
      R"("roots":["files","future"],"nodes":[{"id":"files","label":"Files","kind":"directory"},{"id":"future","label":"Sessions","kind":"tab"}])";
  auto reply = decodeSdpReply(discover(tabs));
  CHECK(reply.roots == std::vector<std::string>({"files", "future"}));
  CHECK(reply.depthLimit == 32 && reply.nodes.at("future").label == "Sessions");
  CHECK(decodeSdpReply(discover(R"("roots":[],"nodes":[])")).roots.empty());
  for (auto roots : {R"(["missing"])", R"(["future","future"])", "{}", "null", "[1]"})
    reject(discover(std::string("\"roots\":") + roots +
                    R"(,"nodes":[{"id":"future","label":"Future","kind":"tab"}])"));
  reject(discover(R"("nodes":[])"));
  for (auto depth : {"0", "-1", "1.5", "\"8\"", "null", "true"})
    reject(discover(tabs + ",\"expansionDepthLimit\":" + depth));
  CHECK(decodeSdpReply(discover(tabs + ",\"expansionDepthLimit\":8")).depthLimit == 8);
  CHECK(decodeSdpReply(discover(tabs + ",\"expansionDepthLimit\":900")).depthLimit == 32);
  const std::string source =
      R"({"id":"opaque/model","kind":"sdl","source":"SDP/SDL/Entry.design","revision":"r1"})";
  reply = decodeSdpReply(discover(tabs, "[" + source + "]"));
  auto target = sdpSourceTarget(reply, "opaque/model");
  CHECK(target.path == "/project/SDP/SDL/Entry.design" && target.revision == "r1");
  SdpNode node;
  node.id = "sdl/opaque/model";
  node.kind = "source"; // Invalid models can have no action yet still expose source.
  node.label = "not a path.design";
  CHECK(sdpSourceForNode(reply, node, "") == "opaque/model");
  node.kind = "viewpoint";
  CHECK(sdpSourceForNode(reply, node, "").empty());
  CHECK(sdpSourceForNode(reply, node, "opaque/model") == "opaque/model");
  node.target.model = "foreign";
  CHECK(sdpSourceForNode(reply, node, "opaque/model").empty());
  node.target.model = "opaque/model";
  CHECK(sdpSourceForNode(reply, node, "") == "opaque/model");
  CHECK(sdpSourceTarget(reply, "unknown").path.empty());
  reply.sources["ui/opaque"] = {"ui/opaque", "sdui", "SDP/UI/My frame.sdui", "ui-r1"};
  node = {};
  node.id = "sdui/ui/opaque";
  node.kind = "source";
  node.state = "invalid";
  CHECK(sdpSourceForNode(reply, node, "") == "ui/opaque");
  CHECK(sdpSourceTarget(reply, "ui/opaque").path == "/project/SDP/UI/My frame.sdui");
  node.kind = "frame";
  CHECK(sdpSourceForNode(reply, node, "ui/opaque") == "ui/opaque");
  node.target.model = "opaque/model";
  CHECK(sdpSourceForNode(reply, node, "ui/opaque").empty());
  node.target.model = "ui/opaque";
  reply.sources["ui/opaque"].path = "SDP/UI/wrong.design";
  CHECK(sdpSourceForNode(reply, node, "").empty());
  CHECK(sdpSourceTarget(reply, "ui/opaque").operation.empty());
  reply.sources["ui/opaque"].kind = "future";
  CHECK(sdpSourceTarget(reply, "ui/opaque").operation.empty());
  reject(discover(tabs, "[" + source + "," + source + "]"));
  for (auto path : {"/outside.design", "SDP/../outside.design", "SDP/", ""})
    reject(discover(tabs, std::string(R"([{"id":"x","kind":"sdl","source":")") + path + "\"}]"));
  reject(discover(tabs, "{}"));
  reject(discover(tabs, "[{}]"));
  std::string roots = "[\"future\"";
  for (int i = 0; i < 33; ++i)
    roots += ",\"x" + std::to_string(i) + "\"";
  reject(discover("\"roots\":" + roots + "],\"nodes\":[]"));
}
TEST_MAIN(run)
