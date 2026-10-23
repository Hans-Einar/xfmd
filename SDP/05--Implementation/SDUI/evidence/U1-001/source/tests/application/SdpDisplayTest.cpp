#include "application/sdp/SdpDisplay.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  SdpReply r;
  r.sources["m"] = {"m", "sdl", "SDP/Model.design", "r1"};
  auto add = [&](const std::string& id, const std::string& kind,
                 std::vector<std::string> children = {}) -> SdpNode& {
    auto& n = r.nodes[id];
    n.id = n.label = id;
    n.kind = kind;
    n.children = std::move(children);
    return n;
  };
  auto& source = add("sdl/m", "source", {"phase", "other", "vp"});
  add("phase", "phase", {"a0", "a1"});
  add("a0", "abstraction", {"alias0"});
  add("a1", "abstraction", {"alias1"});
  add("alias0", "reference").reference = "vp";
  add("alias1", "reference").reference = "vp";
  add("vp", "viewpoint", {"detail"});
  add("other", "future-group");
  CHECK(sdpDisplayChildren(r, source) == std::vector<std::string>({"vp", "other"}));
  // Unrelated tabs and source errors are not normalized away.
  r.sources["ui"] = {"ui", "sdui", "SDP/UI.sdui", "r1"};
  auto& ui = add("sdui/ui", "source", {"phase"});
  CHECK(sdpDisplayChildren(r, ui) == ui.children);
  r.nodes["a0"].diagnostic = "Producer warning";
  CHECK(sdpDisplayChildren(r, source) == std::vector<std::string>({"a0", "vp", "other"}));
  auto& target = add("object", "functionality");
  target.target = {"select", "p", "m", "generic-VP11", "r1"};
  auto& ref = add("detail", "reference");
  ref.reference = "object";
  ref.target = {"select", "p", "m", "focused-VP01", "r1"};
  auto row = sdpDisplayRow(r, "detail", {"vp"}, "m");
  CHECK(row && row->target.uri == "focused-VP01" && row->node == "object" && row->id == "detail");
  // Cyclic relationships stay actionable terminal links, not recursively copied.
  row = sdpDisplayRow(r, "detail", {"object"}, "m");
  CHECK(row && row->terminal && row->target.uri == "focused-VP01");
  add("cycle", "reference").reference = "cycle";
  CHECK(!sdpDisplayRow(r, "cycle", {}));
  r.nodes["cycle"].diagnostic = "Broken source";
  CHECK(sdpDisplayRow(r, "cycle", {}));
  auto& card = add("card", "card");
  card.reference = "object";
  card.target = {"open", "p", "", "", "", "/board/card.md"};
  row = sdpDisplayRow(r, "card", {});
  CHECK(row && row->node == "card" && row->target.path == "/board/card.md");
  r.depthLimit = 1;
  row = sdpDisplayRow(r, "detail", {"vp"}, "m");
  CHECK(row && row->terminal && row->truncated && row->target.uri == "focused-VP01");
  r.nodes["object"].target.model = "different-model";
  CHECK(sdpDisplayRow(r, "detail", {}, "m")->source.empty());
}
TEST_MAIN(run)
