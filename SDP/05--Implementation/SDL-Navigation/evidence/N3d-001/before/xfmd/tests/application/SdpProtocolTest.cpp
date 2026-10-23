#include "application/sdp/SdpToolJob.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
using namespace std::chrono_literals;
namespace fs = std::filesystem;
SdpJobResult wait(SdpToolJob& job) {
  for (int i = 0; i < 1000; ++i) {
    if (auto value = job.take())
      return std::move(*value);
    std::this_thread::sleep_for(5ms);
  }
  throw std::runtime_error("Tool did not return");
}
void run() {
  const auto readiness = decodeSdpReply(R"({"schema":"sdptool/0.2","operation":"sdui-check","status":"prototype-unbound","source":"/project/UI.sdui","entry":"page","revision":"r1","diagnostic":"Unbound SDL callbacks"})");
  CHECK(readiness.status == "prototype-unbound" && readiness.entry == "page");
  bool rejectedReadiness = false;
  try {
    decodeSdpReply(R"({"schema":"sdptool/0.2","operation":"sdui-check","status":"connected","source":"/p","entry":"page","revision":"r1","diagnostic":"claimed"})");
  } catch (const std::exception&) { rejectedReadiness = true; }
  CHECK(rejectedReadiness);

  for (const auto& bytes : {"{}", "[]", "{bad", "{\"schema\":\"sdptool/0.1\"}"}) {
    bool rejected = false;
    try {
      decodeSdpReply(bytes);
    } catch (const std::exception&) {
      rejected = true;
    }
    CHECK(rejected);
  }
  const std::string prefix =
      R"({"schema":"sdptool/0.2","operation":"discover","status":"valid","root":"/tmp/project","area":"/tmp/project/SDP","navigation":{"schema":"sdptool/0.2","operation":"tree","project":"test","roots":["kanban","sdl","sdui"],"nodes":[)";
  const std::string tabs =
      R"({"id":"kanban","label":"KanBan","kind":"tab"},{"id":"sdl","label":"SDL","kind":"tab"},{"id":"sdui","label":"SDUI","kind":"tab"})";
  CHECK(decodeSdpReply(prefix + tabs + "]}}").nodes.size() == 3);
  for (
      const auto& extra :
      {R"({"id":"sdl","label":"Duplicate","kind":"tab"})",
       R"({"id":"a","label":"Missing child","kind":"source","children":["absent"]})",
       R"({"id":"a","label":"Missing reference","kind":"reference","reference":"absent"})",
       R"({"id":"a","label":"Wrong project","kind":"source","target":{"operation":"open","project":"other"}})",
       R"({"id":"a","label":"Embedded\u0000NUL","kind":"source"})"}) {
    bool rejected = false;
    try {
      decodeSdpReply(prefix + tabs + "," + extra + "]}}");
    } catch (const std::exception&) {
      rejected = true;
    }
    CHECK(rejected);
  }
  // Reference cycles are valid producer graphs; native projection bounds their expansion.
  CHECK(decodeSdpReply(prefix + tabs +
                       R"(,{"id":"a","label":"A","kind":"reference","reference":"a"}]}})")
            .nodes.size() == 4);
  SdpToolJob job;
  const std::string fixture = XFMD_SDP_FAKE_TOOL;
  int inherited[2];
  CHECK(pipe(inherited) == 0); // Intentionally no CLOEXEC: the spawn boundary closes it.
  job.start({"python3", fixture, "fd-is-closed", std::to_string(inherited[0])});
  auto closed = wait(job);
  close(inherited[0]);
  close(inherited[1]);
  CHECK(closed.reply && closed.error.empty());
  job.start({"python3", fixture, "closed-streams"}, 100ms);
  CHECK(wait(job).error.find("timed out") != std::string::npos);
  // Exact argv, Unicode and shell metacharacters survive transport as data.
  const std::string path = "/tmp/a ø;$(false) \"quoted\".md";
  job.start({"python3", fixture, "success", path});
  auto result = wait(job);
  CHECK(result.error.empty() && result.reply && result.reply->entry == path);
  for (const auto& mode : {"malformed", "error", "overflow", "stderr-overflow"}) {
    job.start({"python3", fixture, mode});
    result = wait(job);
    CHECK(!result.reply && !result.error.empty());
    if (std::string(mode) == "error")
      CHECK(result.error.find("stale") != std::string::npos);
  }
  job.start({"/definitely/missing/sdptool"});
  CHECK(wait(job).error.find("Cannot start") != std::string::npos);
  job.start({"python3", fixture, "sleep"}, 100ms);
  CHECK(wait(job).error.find("timed out") != std::string::npos);
  auto dir = fs::temp_directory_path() / ("xfmd-sdp-test-" + std::to_string(getpid()));
  fs::create_directories(dir / "project/SDP/child");
  struct Cleanup {
    fs::path path;
    ~Cleanup() { fs::remove_all(path); }
  } cleanup{dir};
  auto marker = dir / "descendant-survived";
  job.start({"python3", fixture, "descendant", marker.string()});
  std::this_thread::sleep_for(150ms);
  auto began = std::chrono::steady_clock::now();
  job.cancel();
  CHECK(std::chrono::steady_clock::now() - began < 500ms);
  job.start({"python3", fixture, "success", path});
  CHECK(wait(job).reply->entry == path);
  std::this_thread::sleep_for(1100ms);
  CHECK(!fs::exists(marker));
  CHECK(sdpArea((dir / "project").string()) == dir / "project/SDP");
  CHECK(sdpArea((dir / "project/SDP").string()) == dir / "project/SDP");
  CHECK(sdpArea((dir / "project/SDP/child").string()).empty()); // No ancestor search.
  fs::create_directories(dir / "alias");
  fs::create_directory_symlink(dir / "project/SDP", dir / "alias/SDP");
  CHECK(sdpArea((dir / "alias").string()).empty());
  std::ofstream(dir / "outside.md") << "outside";
  fs::create_symlink(dir / "outside.md", dir / "project/SDP/escape.md");
  CHECK(!sdpContained((dir / "project/SDP").string(), (dir / "project/SDP/escape.md").string()));
  std::ofstream(dir / "project/SDP/card.md") << "card";
  CHECK(sdpContained((dir / "project/SDP").string(), (dir / "project/SDP/card.md").string()));
}
TEST_MAIN(run)
