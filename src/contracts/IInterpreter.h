#pragma once
#include "SemanticDocument.h"
#include <memory>

namespace xfmd {
struct ParseOptions {
  std::size_t maxBytes = maxDocumentBytes;
};
using ParseResult = std::shared_ptr<const SemanticDocument>;
class IInterpreter {
public:
  virtual ~IInterpreter() = default;
  // Worker-safe for one caller per instance. Results own all data; errors throw Error.
  virtual ParseResult parse(const SourceSnapshot&, const ParseOptions& = {}) = 0;
};
} // namespace xfmd
