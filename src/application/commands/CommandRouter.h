#pragma once
#include <functional>
#include <fx.h>
namespace xfmd {
class CommandRouter : public FX::FXObject {
  FXDECLARE(CommandRouter)
public:
  enum Command {
    Open = 1,
    Save,
    SaveAs,
    Close,
    Undo,
    Redo,
    Find,
    Preview,
    Editor,
    Split,
    Sidebar,
    Back,
    Forward,
    Preferences,
    WindowWrap,
    A4,
    FitWidth,
    ActualSize,
    ExportPdf,
    CancelExport,
    FullScreen,
    LeaveFullScreen,
    ToggleTheme,
    Last
  };
  std::function<void(Command)> action;
  std::function<bool(Command)> enabled, checked;
  long dispatch(FX::FXObject*, FX::FXSelector, void*);
  long update(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
