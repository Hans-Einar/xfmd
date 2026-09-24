#include "application/Application.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <filesystem>
using namespace xfmd;
using namespace FX;
ThemeButton* themeIn(FXWindow* parent) {
  for (auto* child = parent->getFirst(); child; child = child->getNext()) {
    if (auto* theme = dynamic_cast<ThemeButton*>(child))
      return theme;
    if (auto* found = themeIn(child))
      return found;
  }
  return nullptr;
}
void moveTo(FXWindow* w, int x, int y) {
  int rx, ry;
  w->translateCoordinatesTo(rx, ry, w->getRoot(), x, y);
  auto* display = static_cast<Display*>(w->getApp()->getDisplay());
  CHECK(XTestFakeMotionEvent(display, -1, rx, ry, CurrentTime));
  XSync(display, False);
  settleNative(*w->getApp(), 70);
}
void button(FXApp& app, unsigned code, bool press) {
  auto* d = static_cast<Display*>(app.getDisplay());
  CHECK(XTestFakeButtonEvent(d, code, press, CurrentTime));
  XSync(d, False);
  settleNative(app, 100);
}
void click(FXWindow* w, unsigned code = 1) {
  CHECK(w->shown());
  moveTo(w, w->getWidth() / 2, w->getHeight() / 2);
  button(*w->getApp(), code, true);
  button(*w->getApp(), code, false);
}
void run() {
  int argc = 1;
  char name[] = "theme-popup";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  app.edits.applyEdit({0, 0, "# Palette\n\nLiteral content with **bold** and `code`.\n"});
  settleNative(app.app, 500);
  auto* theme = themeIn(app.window);
  CHECK(theme);
  auto* popup = app.window->colorPopup;
  auto* controls = app.window->previewColors;
  CHECK(!popup->shown());
  const auto token = app.session.view().token;
  for (int width : {1100, 640, 450}) {
    app.window->resize(width, 760);
    settleNative(app.app);
    auto* toolbar = theme->getParent();
    CHECK(theme->getX() + theme->getWidth() == toolbar->getWidth() - 2);
    CHECK(app.window->documentPath->getParent()->getY() >= toolbar->getY() + toolbar->getHeight());
    if (width == 450)
      if (const auto* out = getenv("XFMD_UI_EVIDENCE"))
        captureDesktop(app.app, (std::filesystem::path(out) / "workspace-narrow.png").c_str());
  }
  app.window->resize(1100, 760);
  settleNative(app.app, 300);
  auto frame = app.host->frame();
  auto mode = app.preferences->active().appearance.theme;
  click(theme);
  CHECK(app.preferences->active().appearance.theme != mode);
  nativeKey(theme, XK_space);
  CHECK(app.preferences->active().appearance.theme == mode);
  click(theme, 3);
  CHECK(popup->shown() && app.preferences->active().appearance.theme == mode);
  CHECK(controls->getWidth() > 300 && controls->textBrightness->getWidth() > 80);
  if (const auto* out = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (std::filesystem::path(out) / "reading-color-popup.png").c_str());
  const auto before = app.preferences->active().lightReading;
  auto* slider = controls->textBrightness;
  int start = slider->getHeadSize() / 2 +
              (slider->getWidth() - slider->getHeadSize()) * slider->getValue() / 100;
  moveTo(slider, start, slider->getHeight() / 2);
  button(app.app, 1, true);
  moveTo(slider, slider->getWidth() * 3 / 4, slider->getHeight() / 2);
  CHECK(popup->shown());
  CHECK(app.host->readingColors().textBrightness != before.textBrightness);
  CHECK(app.preferences->active().lightReading == before);
  button(app.app, 1, false);
  CHECK(popup->shown());
  CHECK(app.preferences->active().lightReading == app.host->readingColors());
  CHECK(app.host->frame() == frame && app.session.view().token == token && app.edits.canUndo());
  nativeKey(slider, XK_Escape);
  CHECK(!popup->shown());
  nativeKey(theme, XK_F10, ShiftMask);
  CHECK(popup->shown());
  auto keyboardBefore = app.host->readingColors();
  nativeKey(controls->backgroundTone, XK_Right);
  CHECK(!(app.host->readingColors() == keyboardBefore));
  auto saved = app.preferences->active().lightReading;
  CHECK(saved == app.host->readingColors());
  moveTo(app.window->getRoot(), 10, 10);
  button(app.app, 1, true);
  button(app.app, 1, false);
  CHECK(!popup->shown());
  CHECK(app.preferences->active().appearance.theme == mode);
  click(theme);
  click(theme);
  CHECK(app.host->readingColors() == saved);
  CHECK(app.session.view().token == token && app.session.dirty());
  // Destruction must also release keyboard ownership with the popup still open.
  nativeKey(theme, XK_Menu);
  CHECK(popup->shown());
}
TEST_MAIN(run)
