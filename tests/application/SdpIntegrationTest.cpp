#include "application/sdp/SdpToolJob.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
SdpReply invoke(std::vector<std::string> args) {
  auto command = sdpCommand();
  args.insert(args.begin(), command.begin(), command.end());
  args.push_back("--json");
  SdpToolJob job;
  job.start(std::move(args));
  for (int i = 0; i < 2000; ++i) {
    if (auto result = job.take()) {
      if (!result->error.empty())
        throw std::runtime_error(result->error);
      return std::move(*result->reply);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  throw std::runtime_error("Producer timed out in integration test");
}
void run() {
  auto base = fs::temp_directory_path() / ("xfmd-sdp-integration-" + std::to_string(getpid()));
  struct Cleanup {
    fs::path path;
    ~Cleanup() { fs::remove_all(path); }
  } cleanup{base};
  auto project = base / "project with spaces", area = project / "SDP";
  fs::create_directories(area / "SDL");
  fs::create_directories(area / "KanBan/backlog");
  for (auto folder :
       {"backlog", "active", "onHold", "completed", "canceled", "superseded", "irrelevant"})
    fs::create_directories(area / "KanBan" / folder);
  std::ofstream(area / "KanBan/board.json")
      << R"({"schemaVersion":"0.2","projectId":"TEST","namespaces":["TEST"],"profile":"sdp-project-management/0.2","ledger":"Ledger.ndjson"})";
  std::ofstream(area / "KanBan/Ledger.ndjson")
      << R"({"eventId":"EVT-KB-TEST-000001","eventType":"x-kanban:created","subjectId":"KB-TEST-001","payload":{"schemaVersion":"0.2","previousEventId":null,"toPath":"backlog/#001--Proposal--Example.md"}})"
      << '\n';
  const std::string source = "language design-core version 0.5.\nfunctionality Browse.\nunit "
                             "Desktop.\nDesktop owns Browse.\n";
  std::ofstream(area / "SDL/Example.design") << source;
  std::ofstream(area / "SDL/Example.sdui") << "sdui 0.2;\npage = [\"Hello from SDP\"];\n";
  std::ofstream(area / "SDL/Invalid.design") << "invalid input";
  std::ofstream(area / "KanBan/backlog/#001--Proposal--Example.md")
      << "# Example\n| Field | Value |\n| --- | --- |\n| id | KB-TEST-001 |\n| project | TEST |\n| "
         "type | Proposal |\n| CardState | backlog |\n";
  auto capabilities = invoke({project.string(), "actions"});
  CHECK(capabilities.lazySdl);
  auto compact = invoke({project.string(), "discover", "--navigation", "lazy-sdl"});
  CHECK(compact.navigationMode == "lazy-sdl");
  SdpTarget deferred;
  for (const auto& [id, node] : compact.nodes)
    if (node.childrenDeferred) deferred = node.target;
  CHECK(!deferred.model.empty());
  auto model = invoke({project.string(), "tree", "--model", deferred.model,
                       "--revision", deferred.revision, "--model-only"});
  auto merged = expandSdpModel(compact, model, deferred);
  CHECK(merged.nodes.size() > compact.nodes.size());
  CHECK(!merged.nodes.at("sdl/" + deferred.model).childrenDeferred);
  CHECK(compact.nodes.at("sdl/" + deferred.model).childrenDeferred);
  for (int change = 0; change < 7; ++change) {
    auto invalid = model;
    if (change == 0) invalid.revision = "stale";
    if (change == 1) invalid.project = "another-project";
    if (change == 2) invalid.model = "another-model";
    if (change == 3) invalid.nodes.at("sdl").children.push_back("sdl");
    if (change == 4) invalid.nodes.at("sdl").target = {"open", deferred.project, "", "", "", "/tmp/unrelated"};
    if (change == 5) invalid.nodes.emplace("outside", invalid.nodes.at("sdl"));
    if (change == 6) invalid.roots.push_back("kanban");
    bool rejected = false;
    try { expandSdpModel(compact, invalid, deferred); } catch (const std::exception&) { rejected = true; }
    CHECK(rejected && compact.nodes.at("sdl/" + deferred.model).childrenDeferred);
  }
  auto reply = invoke({project.string(), "discover"});
  CHECK(reply.status == "valid" && reply.area == area && reply.root == project);
  SdpTarget selected, frame, card;
  bool invalid = false;
  for (const auto& pair : reply.nodes) {
    const auto& node = pair.second;
    if (node.target.operation == "select" &&
        node.target.uri.find("diagram=VP02-roots") != std::string::npos)
      selected = node.target;
    if (node.target.operation == "sdui-preview" && node.target.entry == "page")
      frame = node.target;
    if (node.kind == "card")
      card = node.target;
    if (node.state == "invalid" && !node.diagnostic.empty())
      invalid = true;
  }
  CHECK(invalid && !selected.model.empty() && !frame.model.empty());
  CHECK(card.operation == "open" && sdpContained(area.string(), card.path));
  CHECK(invoke({area.string(), "discover"}).project == reply.project);
  auto output = base / "selected";
  auto view = invoke({project.string(), "select", "--model", selected.model, "--uri", selected.uri,
                      "--revision", selected.revision, "--output", output.string()});
  CHECK(view.operation == "select" && view.revision == selected.revision);
  CHECK(sdpContained(output.string(), view.entry));
  std::ifstream input(view.entry);
  std::string text((std::istreambuf_iterator<char>(input)), {});
  CHECK(text.find("```mermaid") != std::string::npos);
  auto ui = invoke({project.string(), "sdui-preview", "--model", frame.model, "--entry",
                    frame.entry, "--revision", frame.revision, "--output", (base / "ui").string()});
  CHECK(ui.operation == "sdui-preview" && sdpContained((base / "ui").string(), ui.entry));
  std::ofstream(area / "SDL/Example.design")
      << "language design-core version 0.5.\nfunctionality Browse.\nunit Desktop.\nunit "
         "Later.\nDesktop owns Browse.\n";
  bool stale = false;
  try {
    invoke({project.string(), "select", "--model", selected.model, "--uri", selected.uri,
            "--revision", selected.revision, "--output", (base / "stale").string()});
  } catch (const std::exception& e) {
    stale = std::string(e.what()).find("stale") != std::string::npos;
  }
  CHECK(stale && fs::exists(view.entry));
  // #015 producer capability is available independently of SDP recognition.
  std::ofstream(base / "standalone.design") << source;
  auto preview = invoke(
      {"preview", (base / "standalone.design").string(), "--output", (base / "preview").string()});
  CHECK(preview.operation == "preview" && fs::exists(preview.entry));
  if (const char* actual = std::getenv("XFMD_SDP_PROJECT")) {
    auto snapshot = invoke({actual, "discover"});
    CHECK(snapshot.status == "valid" && snapshot.nodes.size() > 100);
    std::cout << "Project snapshot: " << snapshot.nodes.size() << " nodes\n";
  }
}
TEST_MAIN(run)
