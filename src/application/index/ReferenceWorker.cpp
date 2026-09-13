#include "ReferenceWorker.h"
namespace xfmd {
ReferenceWorker::ReferenceWorker(IInterpreter& parser, const LocalFileStore& store)
    : interpreter(parser), files(store), thread([this] { run(); }) {}
ReferenceWorker::~ReferenceWorker() {
  {
    std::lock_guard<std::mutex> lock(mutex);
    stopping = true;
    pending.clear();
  }
  ready.notify_one();
  thread.join();
}
bool ReferenceWorker::submit(const std::string& path) {
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (pending.size() + completed.size() + (working ? 1 : 0) >= 32)
      return false;
    pending.push_back(path);
  }
  ready.notify_one();
  return true;
}
void ReferenceWorker::cancel() {
  std::lock_guard<std::mutex> lock(mutex);
  ++generation;
  pending.clear();
  completed.clear();
}
std::optional<ReferenceResult> ReferenceWorker::take() {
  std::lock_guard<std::mutex> lock(mutex);
  if (completed.empty())
    return {};
  auto result = std::move(completed.front());
  completed.pop_front();
  return result;
}
bool ReferenceWorker::busy() const {
  std::lock_guard<std::mutex> lock(mutex);
  return working || !pending.empty() || !completed.empty();
}
void ReferenceWorker::run() {
  for (;;) {
    std::string path;
    std::uint64_t current;
    {
      std::unique_lock<std::mutex> lock(mutex);
      ready.wait(lock, [this] { return stopping || !pending.empty(); });
      if (stopping)
        return;
      path = std::move(pending.front());
      pending.pop_front();
      current = generation;
      working = true;
    }
    ReferenceResult result{path, {}, {}};
    try {
      auto loaded = files.read(path);
      bool obsolete;
      {
        std::lock_guard<std::mutex> lock(mutex);
        obsolete = stopping || current != generation;
      }
      if (!obsolete) {
        auto model = interpreter.parse({{}, std::move(loaded.text), loaded.path, loaded.plainText});
        if (!model)
          throw Error(ErrorCode::Parse, "No document model returned.");
        result.headings = DocumentIndex::topLevel(*model);
      }
    } catch (const std::exception& e) {
      result.error = e.what();
    } catch (...) {
      result.error = "Cannot read reference headings.";
    }
    {
      std::lock_guard<std::mutex> lock(mutex);
      working = false;
      if (!stopping && current == generation)
        completed.push_back(std::move(result));
    }
  }
}
} // namespace xfmd
