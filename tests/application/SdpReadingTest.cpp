#include "application/sdp/SdpProtocol.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  const std::string target = R"({"operation":"select","project":"p","model":"m","revision":"r1","uri":"sdl-view://p/m/VP02?projection=reading"})";
  const std::string expansion = R"({"operation":"tree","project":"p","model":"m","revision":"r1"})";
  const std::string source = R"({"id":"sdl/m","kind":"source","state":"validated","label":"System entry","childrenDeferred":true,"target":)" + target + ",\"expansionTarget\":" + expansion + "}";
  const std::string discovery = R"({"schema":"sdptool/0.2","operation":"discover","status":"valid","root":"/p","area":"/p/SDP","navigationMode":"lazy-sdl","capabilities":{"readingNavigation":"1"},"sources":[{"id":"m","kind":"sdl","source":"SDP/Model.design","revision":"r1"}],"navigation":{"schema":"sdptool/0.2","operation":"tree","project":"p","presentation":"producer-reading/1","roots":["sdl"],"nodes":[{"id":"sdl","kind":"tab","label":"SDL","children":["sdl/m"]},)" + source + "]}}";
  auto current = decodeSdpReply(discovery);
  auto requested = current.nodes.at("sdl/m").expansionTarget;
  CHECK(requested.operation == "tree" && requested.uri.empty());
  auto model = decodeSdpReply(R"({"schema":"sdptool/0.2","operation":"tree","project":"p","model":"m","revision":"r1","presentation":"producer-reading/1","roots":["sdl"],"nodes":[{"id":"sdl","kind":"tab","label":"SDL","target":)" + target + R"(,"children":["sdl/m/late","sdl/m/early"]},{"id":"sdl/m/early","kind":"viewpoint","label":"Second","target":)" + target + R"(},{"id":"sdl/m/late","kind":"collection","label":"First","target":)" + target + "}]}");
  auto merged = expandSdpModel(current, model, requested);
  CHECK(!merged.nodes.at("sdl/m").childrenDeferred);
  CHECK(merged.nodes.at("sdl/m").expansionTarget.operation.empty());
  CHECK(merged.nodes.at("sdl/m").target.uri == current.nodes.at("sdl/m").target.uri);
  CHECK(merged.nodes.at("sdl/m").children == model.nodes.at("sdl").children);
  for (int change = 0; change < 7; ++change) {
    auto invalid = model;
    if (change == 0) invalid.presentation.clear();
    if (change == 1) invalid.revision = "stale";
    if (change == 2) invalid.nodes.at("sdl").target = {};
    if (change == 3) invalid.nodes.at("sdl/m/early").target.model = "foreign";
    if (change == 4) invalid.nodes.at("sdl/m/early").target.revision = "stale";
    if (change == 5) invalid.nodes.at("sdl/m/early").target.uri = "sdl-view://foreign/m/VP02";
    if (change == 6) invalid.nodes.at("sdl").children.push_back("sdl");
    bool rejected = false;
    try { expandSdpModel(current, invalid, requested); } catch (...) { rejected = true; }
    CHECK(rejected && current.nodes.at("sdl/m").childrenDeferred);
  }
  for (const auto& change : std::vector<std::pair<std::string,std::string>>{
      {"\"expansionTarget\":" + expansion, "\"expansionTarget\":" + target},
      {"\"operation\":\"tree\",\"project\":\"p\",\"model\":\"m\",\"revision\":\"r1\"", "\"operation\":\"tree\",\"project\":\"p\",\"model\":\"foreign\",\"revision\":\"r1\""},
      {",\"expansionTarget\":" + expansion, ""}}) {
    auto invalid = discovery;
    invalid.replace(invalid.find(change.first), change.first.size(), change.second);
    bool rejected = false;
    try { decodeSdpReply(invalid); } catch (...) { rejected = true; }
    CHECK(rejected);
  }
}
TEST_MAIN(run)
