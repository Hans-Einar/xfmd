#include "application/preview/ParserWorker.h"
#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
#include <chrono>
#include <future>
using namespace xfmd;
void run() {
  CmarkInterpreter parser;
  std::promise<void> entered;
  bool observedCancellation = false;
  ParserWorker worker(parser, [&](ParseResult model, const SourceSnapshot& source,
                                  const std::function<bool()>& cancelled) {
    if (source.text == "first") {
      entered.set_value();
      for (int i = 0; i < 1000 && !cancelled(); ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      observedCancellation = cancelled();
    }
    return model;
  });
  // Same source token: worker ticket, not token alone, identifies refresh work.
  worker.submit({{1, 1}, "first", {}, false});
  entered.get_future().wait();
  worker.submit({{1, 1}, "latest", {}, false});
  std::optional<ParseCompletion> result;
  for (int i = 0; i < 3000 && !result; ++i) {
    result = worker.take();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  CHECK(result && result->model && observedCancellation);
  CHECK(result->model->blocks[0].runs[0].text == "latest");
}
TEST_MAIN(run)
