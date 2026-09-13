#pragma once
#include "ParserWorker.h"
#include "application/adapters/IScheduler.h"
#include "application/document/DocumentSession.h"
#include "contracts/IInterpreter.h"
#include "contracts/IRenderer.h"
#include <functional>
namespace xfmd {
class PreviewCoordinator {
  DocumentSession& session;
  IScheduler& scheduler;
  ParserWorker worker;
  IRenderer& renderer;
  ITextMetrics& metrics;
  ParseResult model;
  double width = 800;
  LayoutProfile profile;
  LayoutResult currentFrame;
  FrameKey requested;
  std::uint64_t generation = 0;

public:
  std::function<void(DocumentToken)> invalidated;
  std::function<void(LayoutResult)> present;
  std::function<void(FrameKey)> layoutRequested;
  std::function<void(const std::string&)> failed;
  PreviewCoordinator(DocumentSession& s, IInterpreter& i, IRenderer& r, ITextMetrics& m,
                     IScheduler& clock)
      : session(s), scheduler(clock), worker(i), renderer(r), metrics(m) {}
  ~PreviewCoordinator();
  void schedule();
  void poll();
  bool busy() const { return worker.busy(); }
  void refresh();
  void relayout(double);
  void setLayoutProfile(LayoutProfile);
  const LayoutProfile& layoutProfile() const { return profile; }
  bool acceptFrame(LayoutResult);
  void invalidate();
  const ParseResult& currentModel() const { return model; }
};
} // namespace xfmd
