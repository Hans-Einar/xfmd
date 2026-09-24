#pragma once
#include "ZoomMode.h"
#include "application/adapters/FoxRenderHost.h"
#include "application/preview/PreviewCoordinator.h"
#include "application/scroll/ScrollCoordinator.h"
#include "application/ui/EditorWidget.h"
#include "application/ui/ViewModeController.h"
namespace xfmd {
// Window-local presentation state; never changes a document or paper profile.
class DocumentZoom {
  EditorWidget& editor;
  FoxRenderHost& host;
  PreviewCoordinator& preview;
  ScrollCoordinator& scrolling;
  ViewModeController& views;
  ZoomMode selected = ZoomMode::Manual;
  double applied = 100;
  bool updating = false;

public:
  std::function<void(double, ZoomMode)> changed;
  DocumentZoom(EditorWidget& e, FoxRenderHost& h, PreviewCoordinator& p, ScrollCoordinator& s,
               ViewModeController& v)
      : editor(e), host(h), preview(p), scrolling(s), views(v) {}
  double percent() const { return applied; }
  ZoomMode mode() const { return selected; }
  void setPercent(double);
  void step(double);
  void setMode(ZoomMode);
  void refresh();
};
} // namespace xfmd
