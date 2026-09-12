#pragma once
#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include "application/document/DocumentSession.h"
#include <functional>
namespace xfmd {
class PreviewCoordinator {
  DocumentSession& session;
  IInterpreter& interpreter;
  IRenderer& renderer;
  ITextMetrics& metrics;
  ParseResult model;
  int width = 800;
  std::uint64_t generation = 0;
public:
  std::function<void(DocumentToken)> invalidated;
  std::function<void(LayoutResult)> present;
  std::function<void(const std::string&)> failed;
  PreviewCoordinator(DocumentSession& s, IInterpreter& i, IRenderer& r, ITextMetrics& m)
      : session(s), interpreter(i), renderer(r), metrics(m) {}
  void refresh();
  void relayout(int);
  void invalidate();
  const ParseResult& currentModel() const { return model; }
};
}
