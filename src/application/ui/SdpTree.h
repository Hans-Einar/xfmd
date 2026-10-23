#pragma once
#include "NavigationTree.h"
#include <vector>
namespace xfmd {
struct SdpItem : NavigationItem {
  std::string id, node, key, source, diagnostic;
  std::vector<std::string> ancestors;
  bool populated = false;
  SdpItem(const std::string& label, std::string id, std::string source,
          std::vector<std::string> ancestors, IndexAction action)
      : NavigationItem(label, std::move(action)), id(std::move(id)), source(std::move(source)),
        ancestors(std::move(ancestors)) {}
};
class SdpTree : public NavigationTree {
public:
  std::function<void(const std::string&, const FX::FXEvent&)> contextRequested;
  explicit SdpTree(FX::FXComposite* parent) : NavigationTree(parent) {}
  long handle(FX::FXObject*, FX::FXSelector, void*) override;
};
} // namespace xfmd
