#pragma once
#include "application/adapters/FoxRenderHost.h"
#include "application/adapters/FoxScheduler.h"
#include "application/document/DocumentCoordinator.h"
#include "application/preview/PreviewCoordinator.h"
#include "NavigationCoordinator.h"
namespace xfmd {
class NavigationPanel {
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
  NavigationPanel(FX::FXComposite*, FX::FXApp&);
  ~NavigationPanel();
  bool open(const std::string&);
  bool follow(const std::string& link) { return navigation.followLink(link); }
  const std::string& path() const { return session.view().path; }
};
}
