#include "application/export/ExportCoordinator.h"
#include "support/TestSupport.h"
#include <chrono>
using namespace xfmd;
void test() {
  std::atomic<bool> entered{false}, release{false};
  ExportCoordinator jobs([&](const ExportRequest& request, ExportControl& control) {
    entered = true;
    while (!release.load()) {
      control.checkpoint();
      std::this_thread::yield();
    }
    CHECK(request.source.text == "unsaved text");
    CHECK(request.source.token == DocumentToken{3, 7});
    return 2u;
  });
  ExportRequest request;
  request.source = {{3, 7}, "unsaved text", "old.md", false};
  CHECK(jobs.start(request));
  while (!entered.load())
    std::this_thread::yield();
  CHECK(!jobs.start(request));
  request.source = {{4, 1}, "edited / different document", "new.md", false};
  release = true;
  std::optional<ExportResult> result;
  while (!(result = jobs.take()))
    std::this_thread::yield();
  CHECK(result->success && result->pages == 2 && result->token == DocumentToken{3, 7});
  entered = false;
  release = false;
  CHECK(jobs.start(request));
  while (!entered.load())
    std::this_thread::yield();
  jobs.cancel();
  while (!(result = jobs.take()))
    std::this_thread::yield();
  CHECK(result->cancelled && !result->success);
  ExportCoordinator failure(
      [](const auto&, auto&) -> unsigned { throw std::runtime_error("disk"); });
  CHECK(failure.start(request));
  while (!(result = failure.take()))
    std::this_thread::yield();
  CHECK(result->error == "disk" && !result->cancelled);
  auto before = std::chrono::steady_clock::now();
  {
    ExportCoordinator stopping([](const auto&, auto& c) -> unsigned {
      for (;;) {
        c.checkpoint();
        std::this_thread::yield();
      }
    });
    CHECK(stopping.start(request));
  }
  CHECK(std::chrono::steady_clock::now() - before < std::chrono::seconds(1));
}
TEST_MAIN(test)
