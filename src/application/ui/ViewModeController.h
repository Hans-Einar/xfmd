#pragma once
#include <functional>
#include <fx.h>
namespace xfmd {
enum class ViewMode { Preview, Editor, Split };
class ViewModeController {
  FX::FXWindow *editor, *preview, *sidebar;
  FX::FXComposite* splitter;
  ViewMode current = ViewMode::Preview;

public:
  std::function<void()> changed;
  ViewModeController(FX::FXWindow* editor, FX::FXWindow* preview, FX::FXWindow* sidebar,
                     FX::FXComposite* splitter)
      : editor(editor), preview(preview), sidebar(sidebar), splitter(splitter) {}
  void setMode(ViewMode);
  void toggleSidebar();
  ViewMode mode() const { return current; }
};
} // namespace xfmd
