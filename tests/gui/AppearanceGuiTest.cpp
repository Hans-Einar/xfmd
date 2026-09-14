#include "application/Application.h"
#include "application/ui/controls/UiButton.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <thread>
using namespace FX;
using namespace xfmd;
void events(Application& app) {
  app.app.refresh();
  for (int i = 0; i < 100; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  app.app.forceRefresh();
  app.app.repaint();
  app.app.flush(true);
}
UiButton* find(FXWindow* root, CommandRouter::Command id) {
  if (auto* button = dynamic_cast<UiButton*>(root); button && button->getSelector() == id)
    return button;
  for (auto* child = root->getFirst(); child; child = child->getNext())
    if (auto* b = find(child, id))
      return b;
  return nullptr;
}
void click(Application& app, UiButton* button) {
  CHECK(button && button->shown());
  auto* d = static_cast<Display*>(app.app.getDisplay());
  XEvent e{};
  e.xbutton.display = d;
  e.xbutton.window = button->id();
  e.xbutton.root = DefaultRootWindow(d);
  e.xbutton.same_screen = True;
  e.xbutton.button = Button1;
  e.xbutton.x = 10;
  e.xbutton.y = 10;
  e.type = ButtonPress;
  CHECK(XSendEvent(d, button->id(), False, ButtonPressMask, &e));
  e.type = ButtonRelease;
  CHECK(XSendEvent(d, button->id(), False, ButtonReleaseMask, &e));
  XSync(d, False);
  events(app);
}
void run() {
  int argc = 1;
  char name[] = "appearance-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  CHECK(app.open(XFMD_FIXTURE));
  app.views->setMode(ViewMode::Split);
  for (int i = 0; i < 10 && !app.host->interactive(); ++i)
    events(app);
  CHECK(app.host->interactive());
  events(app);
  auto token = app.session.view().token;
  auto profile = app.preview->layoutProfile();
  auto* toggle = find(app.window, CommandRouter::ToggleTheme);
  auto* split = find(app.window, CommandRouter::Split);
  CHECK(split && split->isChecked());
  CHECK(toggle && !toggle->isChecked());
  auto icon = toggle->getIcon();
  auto id = toggle->id();
  click(app, toggle);
  CHECK(app.preferences->active().appearance.theme == "dark" && toggle->isChecked());
  CHECK(toggle->getText() == "Dark" && toggle->getIcon() == icon && toggle->id() == id);
  CHECK(app.session.view().token == token);
  CHECK(app.preview->layoutProfile().mode == profile.mode);
  CHECK(app.preview->layoutProfile().paper.margin == profile.paper.margin);
  FXRegistry fresh("xfmd", "xfmd");
  CHECK(fresh.read());
  FoxPreferencesStore store(fresh);
  CHECK(store.load().appearance.theme == "dark");
  click(app, toggle);
  CHECK(app.preferences->active().appearance.theme == "light" && !toggle->isChecked());
  click(app, find(app.window, CommandRouter::Preview));
  CHECK(app.views->mode() == ViewMode::Preview);
  CHECK(find(app.window, CommandRouter::Preview)->isChecked() && !split->isChecked());
  auto settings = app.preferences->begin();
  settings.appearance.fontSize = 18;
  settings.appearance.compact = true;
  std::string error;
  CHECK(app.preferences->commit(settings, error));
  app.window->resize(450, 600);
  events(app);
  auto* bar = toggle->getParent();
  CHECK(toggle->shown());
  for (auto* child = bar->getFirst(); child; child = child->getNext())
    if (child->shown())
      CHECK(child->getX() >= 0 && child->getX() + child->getWidth() <= bar->getWidth());
  CHECK(!split->getParent()->shown());
  app.execute(CommandRouter::Split);
  events(app);
  CHECK(app.views->mode() == ViewMode::Split);
  app.window->resize(1400, 850);
  settings.appearance = {};
  CHECK(app.preferences->commit(settings, error));
  events(app);
  CHECK(split->getParent()->shown() && split->isChecked());
}
TEST_MAIN(run)
