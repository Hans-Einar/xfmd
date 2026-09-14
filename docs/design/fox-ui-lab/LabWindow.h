#pragma once
#include <fx.h>

// Keep ownership in main; FOX's default close() deletes the window itself.
class LabWindow : public FX::FXMainWindow {
public:
  using FX::FXMainWindow::FXMainWindow;
  FX::FXbool close(FX::FXbool = false) override {
    hide();
    getApp()->stop(0);
    return true;
  }
};
