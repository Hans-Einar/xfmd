#pragma once
#include "NavigationPanel.h"
#include "WindowEndpoint.h"
#include <chrono>
namespace xfmd {
class Application;
struct DocumentViewConfig { std::string navigator, tool, source, project, renderer, window; };
class DocumentViews {
  Application& app;
  DocumentViewConfig config;
  FoxScheduler scheduler;
  WindowEndpoint endpoint;
  std::string temporary, pendingDirectory, pendingPane;
  unsigned long long sequence=0;
  int child=-1;
  std::chrono::steady_clock::time_point deadline;
  void poll();
  void cancel();
public:
  std::unique_ptr<NavigationPanel> navigator;
  DocumentViews(Application&, DocumentViewConfig);
  ~DocumentViews();
  bool follow(const std::string&);
  bool open(const std::string& pane,const std::string& path);
};
}
