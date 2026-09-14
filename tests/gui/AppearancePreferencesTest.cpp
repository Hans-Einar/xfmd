#include "application/Application.h"
#include "application/ui/PreferencesDialog.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
using namespace FX;
using namespace xfmd;
template <class T> T* field(FXWindow* root, FXSelector id) {
  if (auto* value = dynamic_cast<T*>(root); value && value->getSelector() == id)
    return value;
  for (auto* child = root->getFirst(); child; child = child->getNext())
    if (auto* value = field<T>(child, id))
      return value;
  return nullptr;
}
void choose(PreferencesDialog& dialog, FXSelector id, int value) {
  auto* combo = field<FXComboBox>(&dialog, id);
  CHECK(combo);
  combo->setCurrentItem(value, true);
}
void key(Application& app, PreferencesDialog& dialog, KeySym symbol) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  drainEvents(app.app);
  dialog.setFocus();
  XSetInputFocus(display, dialog.id(), RevertToParent, CurrentTime);
  drainEvents(app.app);
  CHECK(app.app.getActiveWindow() == &dialog);
  XEvent event{};
  event.xkey.display = display;
  event.xkey.window = dialog.id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.same_screen = True;
  event.xkey.keycode = XKeysymToKeycode(display, symbol);
  event.type = KeyPress;
  CHECK(XSendEvent(display, dialog.id(), False, KeyPressMask, &event));
  event.type = KeyRelease;
  CHECK(XSendEvent(display, dialog.id(), False, KeyReleaseMask, &event));
  drainEvents(app.app);
}
void run() {
  int argc = 1;
  char name[] = "appearance-preferences-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  auto preview = [&](const Appearance& value) {
    app.ui->setAppearance(value);
    app.ui->apply(app.app.getRootWindow());
    app.window->restyle();
  };
  const auto initial = app.preferences->active().appearance;
  auto token = app.session.view().token;
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui, preview);
    dialog.create();
    dialog.show();
    choose(dialog, PreferencesDialog::ThemeChanged, 1);
    choose(dialog, PreferencesDialog::DensityChanged, 1);
    choose(dialog, PreferencesDialog::ButtonsChanged, 1);
    auto* font = field<FXSpinner>(&dialog, PreferencesDialog::FontChanged);
    CHECK(font);
    font->setValue(18, true);
    CHECK(app.ui->appearance().theme == "dark" && app.ui->appearance().fontSize == 18);
    CHECK(app.preferences->active().appearance == initial);
    CHECK(app.session.view().token == token);
    key(app, dialog, XK_Escape);
    CHECK(!dialog.shown());
    CHECK(app.ui->appearance() == initial);
  }
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui, preview);
    dialog.create();
    choose(dialog, PreferencesDialog::ThemeChanged, 1);
    dialog.handle(&dialog, FXSEL(SEL_CLOSE, 0), nullptr);
    CHECK(app.ui->appearance() == initial);
  }
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui, preview);
    dialog.create();
    choose(dialog, PreferencesDialog::ThemeChanged, 1);
    choose(dialog, PreferencesDialog::DensityChanged, 1);
    choose(dialog, PreferencesDialog::ButtonsChanged, 1);
    dialog.show();
    key(app, dialog, XK_Return);
    CHECK(!dialog.shown());
  }
  auto saved = app.preferences->active().appearance;
  CHECK(saved.theme == "dark" && saved.compact && saved.buttons == "classic");
  app.execute(CommandRouter::ToggleTheme);
  CHECK(app.preferences->active().appearance.theme == "light");
  CHECK(app.preferences->active().appearance.compact &&
        app.preferences->active().appearance.buttons == "classic");
  saved = app.preferences->active().appearance;
  {
    PreferencesService failing(app.preferences->active(), [](const auto&, std::string& e) {
      e = "Simulated write failure";
      return false;
    });
    PreferencesDialog dialog(app.window, failing, *app.ui, preview);
    dialog.create();
    dialog.show();
    choose(dialog, PreferencesDialog::ThemeChanged, 1);
    dialog.onAccept(nullptr, 0, nullptr);
    CHECK(dialog.shown() && failing.active().appearance == saved && app.ui->appearance() == saved);
    CHECK(field<FXComboBox>(&dialog, PreferencesDialog::ThemeChanged)->getCurrentItem() == 1);
  }
  const auto accent = app.ui->palette().accent;
  const auto profile = std::filesystem::path(app.ui->profilePath());
  std::filesystem::create_directories(profile.parent_path());
  {
    PreferencesDialog dialog(app.window, *app.preferences, *app.ui, preview);
    dialog.create();
    std::ofstream(profile) << "[light]\naccent=#D02070\n";
    dialog.onReload(nullptr, 0, nullptr);
    CHECK(app.ui->palette().accent == FXRGB(208, 32, 112));
    dialog.onCancel(nullptr, 0, nullptr);
    CHECK(app.ui->palette().accent == accent);
  }
  CHECK(app.session.view().token == token);
}
TEST_MAIN(run)
