#pragma once
#include "DiagramModel.h"
#include "contracts/DocumentTypes.h"
#include <memory>
namespace xfmd {
struct DiagramSource {
  std::string text;
  SourceRange source;
};
struct DiagramParseResult {
  std::shared_ptr<const DiagramModel> model;
  std::string error;
};
class IDiagramInterpreter {
public:
  virtual ~IDiagramInterpreter() = default;
  virtual DiagramParseResult parse(const DiagramSource&) = 0;
};
} // namespace xfmd
