#pragma once
#include "NavigationTree.h"
#include "application/sdp/SdpProtocol.h"
#include <optional>
namespace xfmd {
class SdpPanel : public FX::FXVerticalFrame {
  SdpReply snapshot;
  std::size_t rows = 0;
  bool truncated = false;
  std::vector<std::string> tabIds;
  std::size_t generation = 0;
  bool inContext = false, deferredClear = false;
  std::optional<SdpReply> deferred;
  enum { ID_APPLY = FX::FXVerticalFrame::ID_LAST };
  void add(NavigationTree*, NavigationItem*, const std::string&, std::vector<std::string>,
           const std::string& source = {});
  void expand(NavigationTree*, NavigationItem*);
  void buildTabs(const std::vector<std::string>&);
  void context(const std::string&, const FX::FXEvent&);

public:
  FX::FXTabBook* tabs;
  FX::FXLabel* status;
  std::vector<NavigationTree*> trees;
  std::function<void(const SdpTarget&)> activated, sourceRequested, modelRequested;
  explicit SdpPanel(FX::FXComposite*);
  ~SdpPanel() override;
  long handle(FX::FXObject*, FX::FXSelector, void*) override;
  NavigationTree* tree(const std::string&) const;
  std::string selectedTab() const;
  void clear();
  void present(SdpReply);
  void mergeModel(const SdpReply&, const SdpTarget&);
  void message(const std::string&);
};
} // namespace xfmd
