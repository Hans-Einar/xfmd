#pragma once
#include "FoxCairoCanvas.h"
#include "SharedTextMetrics.h"
#include "ViewTransform.h"
#include "application/scroll/ScrollDynamics.h"
#include "contracts/IRenderer.h"
#include <functional>
#include <fx.h>
#include <optional>
namespace xfmd {
class FoxRenderHost : public FX::FXScrollArea {
  FXDECLARE(FoxRenderHost)
  FoxCairoCanvas canvas;
  IRenderer* renderer = nullptr;
  SharedTextMetrics* metrics = nullptr;
  LayoutResult current;
  DocumentToken expected;
  std::optional<FrameKey> requested;
  bool active = false, programmatic = false, keyboard = false, fit = true;
  double lastWidth = 0, dpiScale = 4.0 / 3, zoom = 1;
  ViewTransform transform;
  unsigned buttons = 0;
  bool clickCancelled = false;
  Point pressPoint;
  std::optional<FrameKey> pressedFrame;
  std::string pressedLink;

protected:
  FoxRenderHost() = default;
  void moveContents(FX::FXint, FX::FXint) override;

public:
  std::function<void(double)> resized, viewportChanged;
  std::function<void(const std::string&)> linkActivated;
  ScrollOrigin lastScrollOrigin = ScrollOrigin::UserDrag;
  FoxRenderHost(FX::FXComposite*, IRenderer&, SharedTextMetrics&);
  void layout() override;
  bool canFocus() const override { return true; }
  FX::FXint getContentWidth() override;
  FX::FXint getContentHeight() override;
  void expect(DocumentToken);
  void expectLayout(FrameKey);
  void present(LayoutResult);
  void invalidate() {
    clickCancelled = true;
    active = false;
    update();
  }
  bool interactive() const { return active; }
  const LayoutResult& frame() const { return current; }
  void setViewport(double y, ScrollOrigin = ScrollOrigin::Restore);
  void setViewScale(bool fitWidth, double factor = 1);
  bool fitWidth() const { return fit; }
  Point documentToView(Point p) const { return transform.toView(p); }
  Point viewToDocument(Point p) const { return transform.toDocument(p); }
  long onPaint(FX::FXObject*, FX::FXSelector, void*);
  long onButtonPress(FX::FXObject*, FX::FXSelector, void*);
  long onUngrabbed(FX::FXObject*, FX::FXSelector, void*);
  long onPointer(FX::FXObject*, FX::FXSelector, void*);
  long onMotion(FX::FXObject*, FX::FXSelector, void*);
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
