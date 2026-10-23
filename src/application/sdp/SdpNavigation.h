#pragma once
#include "SdpToolJob.h"
#include "SdpWatch.h"
#include "contracts/DocumentTypes.h"
#include <filesystem>
namespace xfmd {
class Application;
class SdpNavigation {
  Application& app;
  SdpToolJob job;
  SdpWatch watch;
  bool refreshQueued = false, lazySdl = false;
  std::vector<SdpTarget> expansions;
  SdpTarget expanding;
  std::string watchError;
  std::string root, area, project, pendingOperation;
  std::map<std::string, SdpSource> sources;
  std::filesystem::path temporary, pendingBundle;
  std::size_t request = 0;
  DocumentToken token;
  SdpTarget selection;
  struct Context {
    std::string root;
    SdpTarget target;
    std::filesystem::path directory;
    std::vector<SdpSourceLink> sourceLinks;
  };
  std::map<std::string, Context> contexts;
  void poll();
  void pollWatch();
  void discover();
  void readDiscovery();
  void nextExpansion();
  void cancelRequest();
  void start(const std::string& operation, std::vector<std::string> args);
  void message(const std::string&);
  void discardPending();
  void prune();
  bool deliver(const SdpReply&);
  bool followSdlSource(const std::string&, const std::string&);
  bool followSduiSource(const std::string&, const std::string&);

public:
  explicit SdpNavigation(Application&);
  ~SdpNavigation();
  void stop();
  void refresh(const std::string&);
  void activate(const SdpTarget&);
  void expandModel(const SdpTarget&);
  void openSource(const SdpTarget&);
  std::optional<SdpTarget> currentSdui() const;
  bool follow(const std::string& document, const std::string& uri);
};
} // namespace xfmd
