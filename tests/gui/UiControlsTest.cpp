#include "application/commands/CommandRouter.h"
#include "application/ui/controls/UiLayout.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <thread>
using namespace FX;
using namespace xfmd;
void settle(FXApp& app) {
  app.refresh();
  for (int i = 0; i < 60; ++i) {
    app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  app.repaint();
  app.flush(true);
}
void click(FXApp& app, FXWindow* window) {
  auto* d = static_cast<Display*>(app.getDisplay());
  XEvent e{};
  e.xbutton.display = d;
  e.xbutton.window = window->id();
  e.xbutton.root = DefaultRootWindow(d);
  e.xbutton.button = Button1;
  e.xbutton.same_screen = True;
  e.xbutton.x = 10;
  e.xbutton.y = 10;
  e.type = ButtonPress;
  CHECK(XSendEvent(d, window->id(), False, ButtonPressMask, &e));
  e.type = ButtonRelease;
  CHECK(XSendEvent(d, window->id(), False, ButtonReleaseMask, &e));
  XSync(d, False);
  settle(app);
}
class AlternateButton : public UiButton {
public:
  using UiButton::UiButton;
};
void run() {
  int argc = 1;
  char name[] = "ui-controls";
  char* argv[] = {name, nullptr};
  FXApp app;
  app.init(argc, argv);
  UiContext ui(app, {});
  UiFactory factory(ui);
  CommandRouter commands;
  int calls = 0;
  bool enabled = true, selected = false;
  commands.action = [&](auto) {
    ++calls;
    selected = !selected;
  };
  commands.enabled = [&](auto) { return enabled; };
  commands.checked = [&](auto) { return selected; };
  auto* window = new FXMainWindow(&app, "Controls", nullptr, nullptr, DECOR_ALL, 0, 0, 500, 180);
  auto* tip = new FXToolTip(&app);
  auto* row = factory.row(window);
  auto* arrow = new FXArrowButton(row);
  auto* button =
      factory.button(row, "&Open\tOpen file", &commands, CommandRouter::Open, UiIcon::Open);
  factory.buttonClass = [](auto* p, auto& ui, const auto& text, auto* target, auto id, auto icon,
                           auto role, auto flags) {
    return new AlternateButton(p, ui, text, target, id, icon, role, flags);
  };
  auto* alternate = factory.button(row, "Alternate", &commands, CommandRouter::Save);
  CHECK(dynamic_cast<AlternateButton*>(alternate));
  ui.apply(app.getRootWindow());
  app.create();
  window->show();
  settle(app);
  click(app, button);
  CHECK(calls == 1);
  CHECK(button->isChecked());
  enabled = false;
  settle(app);
  click(app, button);
  CHECK(calls == 1 && !button->isEnabled());
  enabled = true;
  settle(app);
  button->setFocus();
  auto* icon = button->getIcon();
  auto previousWidth = button->getDefaultWidth();
  auto id = button->id();
  Appearance dark{"dark", true, "classic", 15};
  ui.setAppearance(dark);
  ui.apply(app.getRootWindow());
  settle(app);
  CHECK(button->id() == id && button->getIcon() == icon);
  CHECK(icon->getWidth() == ui.metrics().iconSize);
  CHECK(button->getDefaultWidth() > previousWidth);
  CHECK(button->isChecked() && button->hasFocus());
  CHECK(tip->getTextColor() == ui.palette().text && tip->getBackColor() == ui.palette().panel);
  CHECK(arrow->getArrowColor() == ui.palette().text);
  auto* d = static_cast<Display*>(app.getDisplay());
  XSetInputFocus(d, window->id(), RevertToParent, CurrentTime);
  XSync(d, False);
  settle(app);
  button->setFocus();
  XEvent e{};
  e.xkey.display = d;
  e.xkey.window = button->id();
  e.xkey.root = DefaultRootWindow(d);
  e.xkey.same_screen = True;
  e.xkey.keycode = XKeysymToKeycode(d, XK_space);
  e.type = KeyPress;
  CHECK(XSendEvent(d, button->id(), False, KeyPressMask, &e));
  XSync(d, False);
  settle(app);
  e.type = KeyRelease;
  CHECK(XSendEvent(d, button->id(), False, KeyReleaseMask, &e));
  XSync(d, False);
  settle(app);
  CHECK(calls == 2);
  ui.setAppearance({});
  ui.apply(app.getRootWindow());
  settle(app);
  CHECK(button->id() == id && button->getDefaultWidth() == previousWidth);
  delete tip;
  delete window;
}
TEST_MAIN(run)
