#include "application/index/ReferenceWorker.h"
#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
#include <chrono>
#include <future>
using namespace xfmd;
struct Store : LocalFileStore {
  mutable std::promise<void> entered;
  std::shared_future<void> release;
  LoadedDocument read(const std::string& path) const override {
    if (path == "slow.md") {
      entered.set_value();
      release.wait();
    }
    if (path == "missing.md")
      throw Error(ErrorCode::Io, "Missing file");
    return {path, "## Main\n\n### Child\n\n## Second\n", {}, false};
  }
};
void run() {
  CmarkInterpreter parser;
  Store store;
  std::promise<void> release;
  store.release = release.get_future().share();
  ReferenceWorker worker(parser, store);
  CHECK(worker.submit("slow.md"));
  CHECK(store.entered.get_future().wait_for(std::chrono::seconds(2)) == std::future_status::ready);
  worker.cancel();
  CHECK(worker.submit("good.md"));
  CHECK(worker.submit("missing.md"));
  release.set_value();
  std::vector<ReferenceResult> results;
  for (int i = 0; i < 1000 && worker.busy(); ++i) {
    if (auto result = worker.take())
      results.push_back(std::move(*result));
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  CHECK(!worker.busy() && results.size() == 2);
  CHECK(results[0].path == "good.md" && results[0].headings.size() == 2);
  CHECK(results[0].headings[1].title == "Second");
  CHECK(results[1].path == "missing.md" && !results[1].error.empty());
}
TEST_MAIN(run)
