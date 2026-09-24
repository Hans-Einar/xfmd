#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <thread>
using namespace xfmd;
void pump(Application& app) {
  for (int i = 0; i < 15; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}
void button(Application& app, unsigned code, bool press, unsigned state = 0, int x = 30,
            int y = 30) {
  auto* d = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  event.xbutton.display = d;
  event.xbutton.window = app.host->id();
  event.xbutton.root = DefaultRootWindow(d);
  event.xbutton.button = code;
  event.xbutton.state = state;
  event.xbutton.same_screen = True;
  event.xbutton.x = x;
  event.xbutton.y = y;
  event.type = press ? ButtonPress : ButtonRelease;
  CHECK(XSendEvent(d, app.host->id(), False, press ? ButtonPressMask : ButtonReleaseMask, &event));
  XFlush(d);
  pump(app);
}
FX::FXButton* editorButton(FX::FXWindow* root) {
  if (auto* b = dynamic_cast<FX::FXButton*>(root); b && b->getSelector() == CommandRouter::Editor)
    return b;
  for (auto* child = root->getFirst(); child; child = child->getNext())
    if (auto* b = editorButton(child))
      return b;
  return nullptr;
}
void run() {
  int argc = 1;
  char name[] = "xfmd-pointer-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  pump(app);
  button(app, Button1, true);
  button(app, Button1, false, Button1Mask);
  CHECK(!app.host->grabbed()); // Regression: overridden release bypassed FXWindow::ungrab.
  for (unsigned b : {Button1, Button2, Button3}) {
    for (int count = 1; count <= 3; ++count) {
      for (int i = 0; i < count; ++i) {
        button(app, b, true);
        button(app, b, false);
        CHECK(!app.host->grabbed());
      }
    }
  }
  // Chords may arrive when touchpad fingers change during a tap/drag.
  for (unsigned first : {Button1, Button2, Button3})
    for (unsigned second : {Button1, Button2, Button3}) {
      if (first == second)
        continue;
      button(app, first, true);
      button(app, second, true);
      button(app, first, false);
      button(app, second, false, 0, -10, -10);
      CHECK(!app.host->grabbed());
    }
  // Pending layout must not bypass release cleanup.
  button(app, Button1, true);
  app.host->invalidate();
  button(app, Button1, false);
  CHECK(!app.host->grabbed());
  CHECK(app.session.snapshot().text.empty());
  app.edits.applyEdit({0, 0, "[link](next.md)"});
  for (int i = 0; i < 20; ++i)
    pump(app);
  CHECK(app.host->interactive());
  auto frame = app.host->frame();
  const DrawRun* link = nullptr;
  for (const auto& run : frame->runs)
    if (run.text == "link")
      link = &run;
  CHECK(link);
  auto point = app.host->documentToView({link->bounds.x + 1, link->bounds.y + 1});
  int x = int(point.x), y = int(point.y), activations = 0;
  app.host->linkActivated = [&](const std::string&, bool) {
    CHECK(!app.host->grabbed());
    ++activations;
  };
  button(app, Button1, true, 0, x, y);
  button(app, Button1, false, Button1Mask, x, y);
  CHECK(activations == 1);
  button(app, Button1, false, 0, x, y); // Orphan release cannot follow a link.
  button(app, Button1, true, 0, x, y);
  button(app, Button2, true, Button1Mask, x, y);
  button(app, Button2, false, Button1Mask | Button2Mask, x, y);
  button(app, Button1, false, Button1Mask, x, y);
  CHECK(!app.host->grabbed() && activations == 1);
  button(app, Button1, true, 0, x, y);
  button(app, Button1, false, Button1Mask, x + 100, y);
  CHECK(!app.host->grabbed() && activations == 1);
  button(app, Button1, true, 0, x, y);
  app.host->invalidate();
  button(app, Button1, false, Button1Mask, x, y);
  CHECK(!app.host->grabbed() && activations == 1);
  CHECK(app.session.snapshot().text == "[link](next.md)");
  auto* editor = editorButton(app.window);
  CHECK(editor);
  auto* d = static_cast<Display*>(app.app.getDisplay());
  XEvent e{};
  e.xbutton.display = d;
  e.xbutton.window = editor->id();
  e.xbutton.root = DefaultRootWindow(d);
  e.xbutton.button = Button1;
  e.xbutton.same_screen = True;
  e.xbutton.x = 5;
  e.xbutton.y = 5;
  e.type = ButtonPress;
  CHECK(XSendEvent(d, editor->id(), False, ButtonPressMask, &e));
  e.type = ButtonRelease;
  e.xbutton.state = Button1Mask;
  CHECK(XSendEvent(d, editor->id(), False, ButtonReleaseMask, &e));
  XFlush(d);
  pump(app);
  CHECK(app.views->mode() == ViewMode::Editor);
  CHECK(!editor->grabbed());
}
TEST_MAIN(run)
