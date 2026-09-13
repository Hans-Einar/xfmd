#pragma once
#include "DocumentIndex.h"
#include "application/io/LocalFileStore.h"
#include "contracts/IInterpreter.h"
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <thread>
namespace xfmd {
struct ReferenceResult {
  std::string path, error;
  std::vector<IndexHeading> headings;
};
class ReferenceWorker {
  IInterpreter& interpreter;
  const LocalFileStore& files;
  mutable std::mutex mutex;
  std::condition_variable ready;
  std::deque<std::string> pending;
  std::deque<ReferenceResult> completed;
  bool stopping = false, working = false;
  std::uint64_t generation = 0;
  std::thread thread;
  void run();
public:
  ReferenceWorker(IInterpreter&, const LocalFileStore&);
  ~ReferenceWorker();
  bool submit(const std::string&);
  void cancel();
  std::optional<ReferenceResult> take();
  bool busy() const;
};
} // namespace xfmd
