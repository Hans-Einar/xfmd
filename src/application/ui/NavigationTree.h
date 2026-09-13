#pragma once
#include "contracts/DocumentTypes.h"
#include <functional>
#include <fx.h>
#include <optional>
namespace xfmd {
enum class IndexActionKind { None, Heading, File, Hyperlink };
struct IndexAction {
  IndexActionKind kind = IndexActionKind::None;
  std::string path, target, error;
  SourceAnchor anchor;
  // A referenced heading is resolved again in the newly accepted model on open.
  std::string heading;
  std::size_t occurrence = 0;
};
struct NavigationItem : FX::FXTreeItem {
  IndexAction action;
  explicit NavigationItem(const std::string& label, IndexAction value = {})
      : FXTreeItem(label.c_str()), action(std::move(value)) {
    setDraggable(false);
  }
};
class NavigationTree : public FX::FXTreeList {
  FXDECLARE(NavigationTree)
  std::optional<IndexAction> pending;
  bool pointerClick = false;

protected:
  NavigationTree() = default;

public:
  enum { ID_EVENT = FX::FXTreeList::ID_LAST, ID_ACTIVATE, ID_LAST };
  std::function<void(const IndexAction&)> activated;
  std::function<void(NavigationItem*)> expanded;
  explicit NavigationTree(FX::FXComposite*);
  ~NavigationTree() override;
  void cancelActivation();
  FX::FXbool expandTree(FX::FXTreeItem*, FX::FXbool notify = false) override;
  long onRelease(FX::FXObject*, FX::FXSelector, void*);
  long onKey(FX::FXObject*, FX::FXSelector, void*);
  long onClick(FX::FXObject*, FX::FXSelector, void*);
  long onDouble(FX::FXObject*, FX::FXSelector, void*);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
