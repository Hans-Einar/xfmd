#include "application/preview/PreviewCoordinator.h"
#include "renderer/MarkdownRenderer.h"
#include "support/FakeScheduler.h"
#include "support/TestSupport.h"
#include <atomic>
#include <chrono>
#include <future>
#include <thread>
using namespace xfmd;
struct TestInterpreter : IInterpreter {
  std::atomic<int> calls{0};
  std::function<void(const SourceSnapshot&)> intercept;
  ParseResult parse(const SourceSnapshot& source, const ParseOptions&) override {
    ++calls;
    if (intercept) intercept(source);
    auto model = std::make_shared<SemanticDocument>();
    model->token = source.token; model->sourceSize = source.text.size();
    SemanticBlock block; block.source = {0, source.text.size()};
    block.runs.push_back({source.text, block.source, false, false, false, {}});
    model->blocks.push_back(std::move(block)); return model;
  }
};
struct Metrics : ITextMetrics {
  std::thread::id owner = std::this_thread::get_id();
  TextExtent measure(std::string_view text, FontSpec) override {
    CHECK(std::this_thread::get_id() == owner); return {int(text.size()) * 8, 16, 12};
  }
};
template<typename Predicate> void waitUntil(Predicate predicate) {
  auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
  while (!predicate() && std::chrono::steady_clock::now() < deadline) std::this_thread::sleep_for(std::chrono::milliseconds(1));
  CHECK(predicate());
}
void run() {
  DocumentSession session; TestInterpreter parser; MarkdownRenderer renderer; Metrics metrics; FakeScheduler clock;
  LayoutResult frame; int invalidations = 0, failures = 0;
  {
    PreviewCoordinator preview(session, parser, renderer, metrics, clock);
    preview.present = [&](LayoutResult value) { frame = std::move(value); };
    preview.invalidated = [&](DocumentToken) { ++invalidations; };
    preview.failed = [&](const std::string&) { ++failures; };
    session.applyEdit({0, 0, "one"}); preview.schedule();
    clock.advance(100); session.applyEdit({0, 3, "two"}); preview.schedule();
    clock.advance(150); session.applyEdit({0, 3, "three"}); preview.schedule();
    clock.advance(299); CHECK(parser.calls == 0 && !frame);
    clock.advance(1); waitUntil([&] { return !preview.busy(); }); preview.poll();
    CHECK(parser.calls == 1 && frame && frame->token == session.view().token);
    preview.relayout(320); CHECK(parser.calls == 1 && frame->width == 320);
    parser.intercept = [](const SourceSnapshot&) { throw Error(ErrorCode::Parse, "injected parse failure"); };
    session.applyEdit({0, 5, "bad"}); preview.refresh();
    waitUntil([&] { return !preview.busy(); }); preview.poll();
    CHECK(failures == 1 && frame->token != session.view().token);
    preview.schedule();
  }
  CHECK(clock.empty() && invalidations >= 4);
  TestInterpreter delayed;
  std::promise<void> entered, release;
  auto released = release.get_future().share();
  delayed.intercept = [&](const SourceSnapshot& source) { if (source.text == "first") { entered.set_value(); released.wait(); } };
  ParserWorker worker(delayed);
  worker.submit({{5, 1}, "first", {}, false}); entered.get_future().wait();
  worker.submit({{5, 2}, "superseded", {}, false});
  worker.submit({{6, 0}, "newest", {}, false}); release.set_value();
  waitUntil([&] { return !worker.busy(); });
  auto result = worker.take();
  CHECK(result && result->token == DocumentToken{6, 0} && delayed.calls == 2);
}
TEST_MAIN(run)
