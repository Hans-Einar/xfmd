#include "application/Application.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <chrono>
#include <thread>
using namespace xfmd;
void pump(Application& app) {
  for (int i = 0; i < 100; ++i) {
    drainEvents(app.app);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}
void pointer(Application& app, int type, Point point) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  event.type = type;
  event.xbutton.display = display;
  event.xbutton.window = app.host->id();
  event.xbutton.root = DefaultRootWindow(display);
  event.xbutton.same_screen = True;
  event.xbutton.x = point.x;
  event.xbutton.y = point.y;
  long mask;
  if (type == MotionNotify) {
    event.xmotion.state = Button1Mask;
    mask = PointerMotionMask;
  } else {
    event.xbutton.button = Button1;
    event.xbutton.state = type == ButtonRelease ? Button1Mask : 0;
    mask = type == ButtonPress ? ButtonPressMask : ButtonReleaseMask;
  }
  CHECK(XSendEvent(display, app.host->id(), False, mask, &event));
  XFlush(display);
  pump(app);
}
void key(Application& app, KeySym symbol) {
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XEvent event{};
  XSetInputFocus(display, app.window->id(), RevertToParent, CurrentTime);
  event.type = KeyPress;
  event.xkey.display = display;
  event.xkey.window = app.host->id();
  event.xkey.root = DefaultRootWindow(display);
  event.xkey.same_screen = True;
  event.xkey.state = ControlMask;
  event.xkey.keycode = XKeysymToKeycode(display, symbol);
  CHECK(XSendEvent(display, app.host->id(), False, KeyPressMask, &event));
  XFlush(display);
  pump(app);
}
void run() {
  int argc = 1;
  char name[] = "xfmd-mermaid-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.edits.applyEdit(
      {0, 0,
       "# Diagram\n\n```mermaid\nflowchart LR\nA[Blåbær] -->|Target| "
       "B{Ready}\n```\n\n```mermaid\nsequenceDiagram\nparticipant UI\nparticipant "
       "Service\nUI->>Service: Import APT\nService-->>UI: Accepted draft\n```\n\nAfter"});
  for (int i = 0; i < 8 && (!app.host->interactive() ||
                            app.host->frame()->readingText.find("After") == std::string::npos);
       ++i)
    pump(app);
  CHECK(app.host->interactive());
  auto frame = app.host->frame();
  unsigned diagrams = 0;
  for (const auto& run : frame->runs)
    diagrams += dynamic_cast<const DiagramScene*>(run.visual.get()) != nullptr;
  CHECK(diagrams == 2 && frame->readingText.find("Blåbær") == std::string::npos);
  const DrawRun* label = nullptr;
  for (const auto& run : frame->runs)
    if (run.text == "After")
      label = &run;
  CHECK(label);
  auto begin =
      app.host->documentToView({label->bounds.x, label->bounds.y + label->bounds.height / 2});
  auto end = app.host->documentToView(
      {label->bounds.x + label->bounds.width, label->bounds.y + label->bounds.height / 2});
  pointer(app, ButtonPress, begin);
  pointer(app, MotionNotify, end);
  pointer(app, ButtonRelease, end);
  CHECK(app.host->selectedText() == "After");
  auto utf8 = app.app.registerDragType("UTF8_STRING");
  FX::FXString copied;
  CHECK(app.window->editor->getDNDData(FX::FROM_SELECTION, utf8, copied));
  CHECK(copied == "After");
  key(app, XK_c);
  CHECK(app.window->editor->getDNDData(FX::FROM_CLIPBOARD, utf8, copied));
  CHECK(copied == "After");
  key(app, XK_a);
  CHECK(app.host->selectedText() == frame->readingText);
  app.execute(CommandRouter::ToggleTheme);
  pump(app);
  CHECK(app.host->frame() == frame);
  CHECK(app.host->selectedText() == frame->readingText);
  app.window->resize(640, 700);
  pump(app);
  CHECK(app.host->interactive());
  app.execute(CommandRouter::A4);
  pump(app);
  CHECK(app.host->interactive() && app.host->frame()->pages.slices.size() == 1);
  app.edits.applyEdit({0, app.session.snapshot().text.size(),
                       "```mermaid\nflowchart LR\nA-->B\nclick A bad()\n```"});
  for (int i = 0; i < 4; ++i)
    pump(app);
  CHECK(app.host->interactive() &&
        app.host->frame()->readingText.find("Mermaid:") != std::string::npos);
}
TEST_MAIN(run)
