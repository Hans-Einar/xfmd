#pragma once
#include "Capture.h"
#include "NativeInput.h"
#include "application/Application.h"
#include <X11/keysym.h>
#include <filesystem>
struct PathMenuDriver : FX::FXObject {
  xfmd::Application& app;
  std::string action, capture;
  bool visited = false, positioned = false;
  explicit PathMenuDriver(xfmd::Application& a) : app(a) {}
  long handle(FX::FXObject*, FX::FXSelector, void*) override {
    auto* popup = app.app.getPopupWindow();
    CHECK(popup);
    if (!positioned) {
      auto* command = popup->getFirst();
      auto* display = static_cast<Display*>(app.app.getDisplay());
      XWarpPointer(display, None, command->id(), 0, 0, 0, 0, 15, command->getHeight() / 2);
      XFlush(display);
      positioned = true;
      app.app.addTimeout(this, 1, 100);
      return 1;
    }
    if (!capture.empty()) {
      if (const auto* output = getenv("XFMD_UI_EVIDENCE"))
        captureDesktop(app.app, (std::filesystem::path(output) / capture).c_str());
    }
    if (action.empty())
      nativeKey(popup, XK_Escape);
    else {
      FX::FXMenuCommand* command = nullptr;
      for (auto* child = popup->getFirst(); child; child = child->getNext()) {
        auto* candidate = dynamic_cast<FX::FXMenuCommand*>(child);
        if (candidate && candidate->getText().text() == action)
          command = candidate;
      }
      CHECK(command);
      nativeClick(command, 20, command->getHeight() / 2, 0, Button1, false);
    }
    visited = true;
    return 1;
  }
};
inline void pathMenu(xfmd::Application& app, FX::FXWindow* widget, int x, int y,
                     const std::string& action, const std::string& capture = "") {
  PathMenuDriver driver(app);
  driver.action = action;
  driver.capture = capture;
  // Queue input before scheduling the popup driver: nativeClick may wait out
  // the previous click interval, during which the popup does not exist yet.
  nativeClick(widget, x, y, 0, Button3, false);
  app.app.addTimeout(&driver, 1, 100);
  settleNative(app.app);
  CHECK(driver.visited && !app.app.getPopupWindow());
}
inline int listRowY(FX::FXList* list, int row) {
  list->makeItemVisible(row);
  settleNative(*list->getApp(), 30);
  int y = list->getYPosition() + list->getItemHeight(row) / 2;
  for (int i = 0; i < row; ++i)
    y += list->getItemHeight(i);
  return y;
}
inline void clickRow(FX::FXList* list, int row) { nativeClick(list, 20, listRowY(list, row)); }
inline void listMenu(xfmd::Application& app, FX::FXList* list, int row, const std::string& action,
                     const std::string& capture = "") {
  pathMenu(app, list, 20, listRowY(list, row), action, capture);
}
inline void treeMenu(xfmd::Application& app, const std::filesystem::path& path,
                     const std::string& action, const std::string& capture = "") {
  auto* tree = app.window->sidebar;
  settleNative(app.app);
  auto* item = tree->getPathnameItem(path.c_str());
  CHECK(item);
  tree->makeItemVisible(item);
  settleNative(app.app, 30);
  for (int y = 0; y < tree->getViewportHeight(); ++y) {
    if (tree->getItemAt(90, y) == item) {
      pathMenu(app, tree, 90, y + 5, action, capture);
      return;
    }
  }
  throw std::runtime_error("Tree item not visible");
}
