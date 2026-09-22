#pragma once
#include "NavigationCoordinator.h"
#include "application/adapters/FoxRenderHost.h"
#include "application/adapters/FoxScheduler.h"
#include "application/document/DocumentCoordinator.h"
#include "application/preview/PreviewCoordinator.h"
namespace xfmd {
class NavigationPanel {
  std::string pendingFragment;
  DocumentSession session;
  LocalFileStore files;
  DocumentCoordinator documents{session, files};
  std::unique_ptr<IInterpreter> interpreter;
  std::unique_ptr<IRenderer> renderer;
  SharedTextMetrics metrics;
  FoxScheduler scheduler;
  std::unique_ptr<PreviewCoordinator> preview;
  ScrollCoordinator scrolling;
  NavigationCoordinator navigation{documents, session, scrolling};

public:
  FoxRenderHost* host;
  std::function<void(const std::string&)> error;
  std::function<void()> changed;
  NavigationPanel(FX::FXComposite*, FX::FXApp&);
  ~NavigationPanel();
  bool open(const std::string&);
  bool follow(const std::string& link);
  const std::string& path() const { return session.view().path; }
};
} // namespace xfmd
