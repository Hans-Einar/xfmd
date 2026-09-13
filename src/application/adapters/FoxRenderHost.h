#pragma once
#include "SharedTextMetrics.h"
#include "contracts/IRenderer.h"
#include <functional>
#include <fx.h>
namespace xfmd {
class FoxRenderHost : public FX::FXScrollArea {
  FXDECLARE(FoxRenderHost)
  IRenderer* renderer = nullptr;
  SharedTextMetrics* metrics = nullptr;
  LayoutResult current;
  DocumentToken expected;
  bool active = false, programmatic = false;
  int lastWidth = 0;

protected:
  FoxRenderHost() = default;
  void moveContents(FX::FXint, FX::FXint) override;

public:
  std::function<void(int)> resized;
  std::function<void(int)> viewportChanged;
  std::function<void(const std::string&)> linkActivated;
  FoxRenderHost(FX::FXComposite*, IRenderer&, SharedTextMetrics&);
  void layout() override;
  bool canFocus() const override { return true; }
  long onKeyPress(FX::FXObject*, FX::FXSelector, void*);
  FX::FXint getContentWidth() override;
  FX::FXint getContentHeight() override;
  void expect(DocumentToken token) {
    if (token.document != expected.document)
      current.reset();
    expected = token;
    active = false;
    update();
  }
  void present(LayoutResult);
  void invalidate() {
    active = false;
    update();
  }
  bool interactive() const { return active; }
  const LayoutResult& frame() const { return current; }
  void setViewport(int y);
  long onPaint(FX::FXObject*, FX::FXSelector, void*);
  long onPointer(FX::FXObject*, FX::FXSelector, void*);
  long onMotion(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
