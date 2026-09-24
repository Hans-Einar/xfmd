#include "application/Application.h"
#include "application/ui/PreferencesDialog.h"
#include "support/Capture.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
using namespace xfmd;
using namespace FX;
const ReadingColors savedDark{-1, 10, 45, 85}, savedLight{-1, 97, 240, 20};
void events(Application& app, int ms = 100) {
  auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
  do {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  } while (std::chrono::steady_clock::now() < end);
  app.app.forceRefresh();
  app.app.repaint();
  app.app.flush(true);
}
std::uint32_t pixel(Application& app, Point point) {
  auto* d = static_cast<Display*>(app.app.getDisplay());
  auto* image = XGetImage(d, app.host->id(), int(point.x), int(point.y), 1, 1, AllPlanes, ZPixmap);
  CHECK(image);
  auto p = XGetPixel(image, 0, 0);
  auto channel = [&](unsigned long mask) { return unsigned((p & mask) * 255 / mask); };
  auto result =
      channel(image->red_mask) << 16 | channel(image->green_mask) << 8 | channel(image->blue_mask);
  XDestroyImage(image);
  return result;
}
void pointer(Application& app, FXSlider* slider, int type, int x) {
  auto* d = static_cast<Display*>(app.app.getDisplay());
  if (type == ButtonPress) {
    XWarpPointer(d, None, slider->id(), 0, 0, 0, 0, x, slider->getHeight() / 2);
    XSync(d, False);
    events(app);
  }
  XEvent e{};
  e.type = type;
  e.xbutton.display = d;
  e.xbutton.window = slider->id();
  e.xbutton.root = DefaultRootWindow(d);
  e.xbutton.same_screen = True;
  e.xbutton.x = x;
  e.xbutton.y = slider->getHeight() / 2;
  e.xbutton.button = Button1;
  Window child;
  XTranslateCoordinates(d, slider->id(), DefaultRootWindow(d), x, slider->getHeight() / 2,
                        &e.xbutton.x_root, &e.xbutton.y_root, &child);
  if (type == MotionNotify) {
    e.xmotion.state = Button1Mask;
    e.xmotion.is_hint = NotifyNormal;
  }
  CHECK(XSendEvent(d, slider->id(), False,
                   type == MotionNotify  ? PointerMotionMask
                   : type == ButtonPress ? ButtonPressMask
                                         : ButtonReleaseMask,
                   &e));
  XFlush(d);
  events(app);
}
template <class T> T* field(FXWindow* root, FXSelector id) {
  if (auto* result = dynamic_cast<T*>(root); result && result->getSelector() == id)
    return result;
  for (auto* c = root->getFirst(); c; c = c->getNext())
    if (auto* r = field<T>(c, id))
      return r;
  return nullptr;
}
void run(bool restart) {
  int argc = 1;
  char name[] = "reading-colors";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  auto* controls = app.window->previewColors;
  if (restart) {
    CHECK(app.preferences->active().lightReading == savedLight);
    CHECK(app.preferences->active().darkReading == savedDark);
    CHECK(app.host->readingColors() == savedDark && controls->values() == savedDark);
    auto rgb = ReadingPalette::from(savedDark).text;
    CHECK(app.window->editor->getTextColor() ==
          FXRGB((rgb >> 16) & 255, (rgb >> 8) & 255, rgb & 255));
    app.execute(CommandRouter::ToggleTheme);
    CHECK(app.host->readingColors() == savedLight && controls->values() == savedLight);
    return;
  }
  app.edits.applyEdit({0, 0,
                       "# Reading colors\n\nText with **bold**, `code` and [a link](next.md).\n\n| "
                       "A | B |\n|---|---|\n| c | d |\n\n> Quote\n\n```\ncode block\n```\n"});
  events(app, 700);
  CHECK(app.host->interactive());
  auto token = app.session.view().token;
  auto frame = app.host->frame();
  app.execute(CommandRouter::ToggleTheme);
  events(app);
  CHECK(app.host->readingColors() == ReadingColors::defaults(true));
  CHECK(pixel(app, {3, 3}) == ReadingPalette::from(ReadingColors::defaults(true)).background);
  CHECK(app.host->frame() == frame);
  auto before = app.preferences->active().darkReading;
  app.window->colorPopup->showAt(app.window->previewControls);
  events(app);
  auto* slider = controls->textBrightness;
  int start = slider->getHeadSize() / 2 +
              (slider->getWidth() - slider->getHeadSize()) * slider->getValue() / 100;
  pointer(app, slider, ButtonPress, start);
  pointer(app, slider, MotionNotify, slider->getWidth() / 2);
  CHECK(slider->getValue() != before.textBrightness);
  CHECK(app.host->readingColors().textBrightness == slider->getValue());
  CHECK(app.preferences->active().darkReading == before); // Repaint before persistence.
  pointer(app, slider, ButtonRelease, slider->getWidth() / 2);
  CHECK(app.preferences->active().darkReading.textBrightness == slider->getValue());
  CHECK(app.host->frame() == frame && app.session.view().token == token && app.session.dirty());
  app.window->colorPopup->popdown();
  controls->changed(savedDark, true);
  app.execute(CommandRouter::ToggleTheme);
  controls->changed(savedLight, true);
  // Appearance preview selects stored colors, Cancel restores active theme without rewriting them.
  struct AppearanceProbe : FXObject {
    Application& app;
    explicit AppearanceProbe(Application& a) : app(a) {}
    long handle(FXObject*, FXSelector, void*) override {
      auto* dialog = dynamic_cast<PreferencesDialog*>(app.app.getModalWindow());
      CHECK(dialog);
      auto* theme = field<FXComboBox>(dialog, PreferencesDialog::ThemeChanged);
      CHECK(theme);
      theme->setCurrentItem(1, true);
      CHECK(app.host->readingColors() == savedDark);
      dialog->onCancel(nullptr, 0, nullptr);
      return 1;
    }
  } probe(app);
  app.app.addTimeout(&probe, 1, 20);
  app.execute(CommandRouter::Preferences);
  CHECK(app.host->readingColors() == savedLight);
  app.execute(CommandRouter::ToggleTheme);
  CHECK(controls->values() == savedDark);
  app.execute(CommandRouter::A4);
  events(app);
  auto paged = app.host->frame();
  CHECK(paged->key.profile.mode == LayoutMode::Paged);
  auto position = app.host->getYPosition();
  auto palette = ReadingPalette::from(savedDark);
  CHECK(pixel(app, app.host->documentToView({5, 5})) == palette.background);
  controls->changed({-1, 15, 60, 80}, false);
  events(app);
  CHECK(app.host->frame() == paged && app.host->getYPosition() == position);
  controls->changed(savedDark, true);
  events(app);
  if (const char* capture = std::getenv("XFMD_CAPTURE_FILE"))
    captureDesktop(app.app, capture);
  // Real write failure rolls back screen, controls and registry.
  auto path = std::filesystem::path(FXSystem::getHomeDirectory().text()) / ".foxrc/xfmd/xfmd";
  auto backup = path;
  backup += ".test-backup";
  std::filesystem::rename(path, backup);
  std::filesystem::create_directory(path);
  std::ofstream(path / "blocker") << "keep";
  controls->changed({0, 50, 120, 60}, true);
  CHECK(app.preferences->active().darkReading == savedDark);
  CHECK(app.host->readingColors() == savedDark && controls->values() == savedDark);
  std::filesystem::remove_all(path);
  std::filesystem::rename(backup, path);
}
int main(int argc, char**) {
  try {
    run(argc > 1);
    if (argc == 1) {
      auto child = fork();
      CHECK(child >= 0);
      if (child == 0) {
        execl("/proc/self/exe", "ReadingColorsGuiTest", "restart", nullptr);
        _exit(127);
      }
      int status;
      CHECK(waitpid(child, &status, 0) == child);
      CHECK(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    }
    std::cout << "PASS\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
