#include "application/sdp/SdpDisplay.h"
#include "application/sdp/SdpProtocol.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  auto result = decodeSdpReply(
      R"({"schema":"sdptool/0.2","operation":"activity-view","profile":"activity/1","source":"/p/SDP","entry":"/bundle/entry.md","directory":"/bundle","revision":"rev"})");
  CHECK(result.profile == "activity/1" && result.source == "/p/SDP");
  CHECK(sdpDisplayAction({"activity-view", "p", "", "activity-view://p/overview", "rev"}));
  const std::string target =
      R"({"operation":"activity-view","project":"p","uri":"activity-view://p/state/active","revision":"rev"})";
  const std::string expansion =
      R"({"operation":"activity-tree","project":"p","uri":"activity-tree://p/state/active?parent=activities%2Fstate%2Factive","revision":"rev"})";
  auto current = decodeSdpReply(
      R"({"schema":"sdptool/0.2","operation":"discover","status":"valid","root":"/p","area":"/p/SDP","navigation":{"schema":"sdptool/0.2","operation":"tree","project":"p","roots":["activities"],"nodes":[{"id":"activities","kind":"tab","label":"Activities","children":["activities/state/active"]},{"id":"activities/state/active","kind":"group","label":"Active","childrenDeferred":true,"target":)" +
      target + ",\"expansionTarget\":" + expansion + "}]}}");
  const auto deferred = current.nodes.at("activities/state/active").expansionTarget;
  auto page = decodeSdpReply(
      R"({"schema":"sdptool/0.2","operation":"activity-tree","project":"p","revision":"rev","roots":["activities/state/active"],"nodes":[{"id":"activities/state/active","kind":"group","label":"Active","target":)" +
      target +
      R"(,"children":["activities/state/active/activity/1"]},{"id":"activities/state/active/activity/1","kind":"activity","label":"Useful need"}]})");
  auto merged = expandSdpPage(current, page, deferred);
  CHECK(merged.nodes.count("activities/state/active/activity/1"));
  CHECK(!merged.nodes.at("activities/state/active").childrenDeferred);
  CHECK(current.nodes.at("activities/state/active").childrenDeferred);
  for (int failure = 0; failure < 3; ++failure) {
    auto invalid = page;
    if (failure == 0)
      invalid.revision = "stale";
    if (failure == 1)
      invalid.project = "other";
    if (failure == 2)
      invalid.nodes.at("activities/state/active").target.uri = "activity-view://p/other";
    bool rejected = false;
    try {
      expandSdpPage(current, invalid, deferred);
    } catch (...) {
      rejected = true;
    }
    CHECK(rejected);
    CHECK(current.nodes.size() == 2);
  }
}
TEST_MAIN(run)
