#pragma once
#include "application/document/TextProjection.h"
#include "application/scroll/ScrollDynamics.h"
#include <functional>
#include <fx.h>
#include <memory>
namespace xfmd {
class EditorWidget : public FX::FXText {
  FXDECLARE(EditorWidget)
  std::unique_ptr<TextProjection> projection;
  bool projecting = false, scrolling = false, keyboard = false;

protected:
  EditorWidget() = default;
  void moveContents(FX::FXint x, FX::FXint y) override;

public:
  enum { ID_EDIT = FX::FXText::ID_LAST, ID_LAST };
  std::function<void(const std::string&)> edited;
  std::function<void(std::size_t)> viewportChanged;
  ScrollOrigin lastScrollOrigin = ScrollOrigin::UserDrag;
  explicit EditorWidget(FX::FXComposite*);
  void applyProjection(const SourceSnapshot&);
  void setSourceAnchor(SourceAnchor);
  std::size_t sourceAnchor() const;
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
