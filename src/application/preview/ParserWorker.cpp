#include "ParserWorker.h"
namespace xfmd {
ParserWorker::ParserWorker(
    IInterpreter& parser,
    std::function<ParseResult(ParseResult, const SourceSnapshot&, const std::function<bool()>&)>
        prepare)
    : interpreter(parser), prepare(std::move(prepare)), thread([this] { run(); }) {}
ParserWorker::~ParserWorker() {
  {
    std::lock_guard<std::mutex> lock(mutex);
    stopping = true;
    pending.reset();
  }
  ready.notify_one();
  thread.join();
}
void ParserWorker::submit(SourceSnapshot source) {
  {
    std::lock_guard<std::mutex> lock(mutex);
    pending = std::move(source);
    completed.reset();
    ++ticket;
  }
  ready.notify_one();
}
std::optional<ParseCompletion> ParserWorker::take() {
  std::lock_guard<std::mutex> lock(mutex);
  auto result = std::move(completed);
  completed.reset();
  return result;
}
bool ParserWorker::busy() const {
  std::lock_guard<std::mutex> lock(mutex);
  // A completion can arrive between the GUI's take() and busy() calls.
  // Keep polling until that result has actually been collected.
  return working || bool(pending) || bool(completed);
}
void ParserWorker::run() {
  for (;;) {
    SourceSnapshot source;
    std::uint64_t current;
    {
      std::unique_lock<std::mutex> lock(mutex);
      ready.wait(lock, [this] { return stopping || pending.has_value(); });
      if (stopping)
        return;
      source = std::move(*pending);
      pending.reset();
      current = ticket;
      working = true;
    }
    ParseCompletion result{source.token, {}, {}};
    try {
      result.model = interpreter.parse(source);
      if (prepare)
        result.model = prepare(result.model, source, [this, current] {
          std::lock_guard<std::mutex> lock(mutex);
          return stopping || current != ticket;
        });
    } catch (const std::exception& e) {
      result.error = e.what();
    } catch (...) {
      result.error = "Unexpected interpreter failure.";
    }
    {
      std::lock_guard<std::mutex> lock(mutex);
      working = false;
      if (!stopping && current == ticket)
        completed = std::move(result);
    }
  }
}
} // namespace xfmd
