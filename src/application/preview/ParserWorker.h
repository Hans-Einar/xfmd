#pragma once
#include "contracts/IInterpreter.h"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
namespace xfmd {
struct ParseCompletion {
  DocumentToken token;
  ParseResult model;
  std::string error;
};
class ParserWorker {
  IInterpreter& interpreter;
  std::function<ParseResult(ParseResult, const SourceSnapshot&)> prepare;
  mutable std::mutex mutex;
  std::condition_variable ready;
  std::optional<SourceSnapshot> pending;
  std::optional<ParseCompletion> completed;
  bool stopping = false, working = false;
  std::uint64_t ticket = 0;
  std::thread thread;
  void run();

public:
  explicit ParserWorker(IInterpreter&,
                        std::function<ParseResult(ParseResult, const SourceSnapshot&)> = {});
  ~ParserWorker();
  void submit(SourceSnapshot);
  std::optional<ParseCompletion> take();
  bool busy() const;
};
} // namespace xfmd
