#pragma once
#include "application/document/TextProjection.h"
#include "application/preferences/ReadingColors.h"
#include "application/scroll/ScrollDynamics.h"
#include <functional>
#include <fx.h>
#include <memory>
namespace xfmd {
class EditorWidget : public FX::FXText {
  FXDECLARE(EditorWidget)
  std::unique_ptr<TextProjection> projection;
  LayoutProfile viewProfile;
  bool layingOut = false, presentationDirty = false;
  int presentationWidth = -1, presentationHeight = -1;
  int observedViewportWidth = -1, observedViewportHeight = -1;
  double viewScale = 1;
  FX::FXFont* baseFont = nullptr;
  std::unique_ptr<FX::FXFont> scaledFont;
  void applyViewProfile();
  bool projecting = false, scrolling = false, keyboard = false;

protected:
  EditorWidget() = default;
  void moveContents(FX::FXint x, FX::FXint y) override;

public:
  enum { ID_EDIT = FX::FXText::ID_LAST, ID_LAST };
  std::function<void(const std::string&)> edited;
  std::function<void(std::size_t)> viewportChanged;
  std::function<void(double)> zoomRequested;
  std::function<void()> geometryChanged;
  ScrollOrigin lastScrollOrigin = ScrollOrigin::UserDrag;
  explicit EditorWidget(FX::FXComposite*);
  void setReadingColors(const ReadingColors&);
  void setViewProfile(const LayoutProfile&, double factor);
  int contentViewportWidth() const { return viewport_w; }
  int contentViewportHeight() const { return viewport_h; }
  void layout() override;
  void applyProjection(const SourceSnapshot&);
  void setSourceAnchor(SourceAnchor);
  std::size_t sourceAnchor() const;
  long onMouseWheel(FX::FXObject*, FX::FXSelector, void*);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
