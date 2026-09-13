#pragma once
#include <functional>
#include <fx.h>
#include <string>
namespace xfmd {
class SidebarWidget : public FX::FXDirList {
  FXDECLARE(SidebarWidget)
protected:
  SidebarWidget() = default;

public:
  enum { ID_TREE_EVENT = FX::FXDirList::ID_LAST, ID_LAST };
  std::function<void(const std::string&)> open;
  explicit SidebarWidget(FX::FXComposite*);
  long onOpen(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
