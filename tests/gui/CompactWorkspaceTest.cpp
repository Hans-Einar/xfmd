#include "application/Application.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <thread>
using namespace xfmd;
using namespace FX;
void events(Application& app) {
  for (int i = 0; i < 80; ++i) {
    app.app.runWhileEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
  drainEvents(app.app);
}
void hover(Application& app, Point p, int type = MotionNotify) {
  auto* d = static_cast<Display*>(app.app.getDisplay());
  if (type == LeaveNotify)
    XWarpPointer(d, None, DefaultRootWindow(d), 0, 0, 0, 0, 0, 0);
  else
    XWarpPointer(d, None, app.host->id(), 0, 0, 0, 0, int(p.x) + app.host->getXPosition(),
                 int(p.y) + app.host->getYPosition());
  XSync(d, False);
  drainEvents(app.app);
}
void run() {
  int argc = 1;
  char name[] = "compact-workspace";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  CHECK(app.open(XFMD_FIXTURE));
  app.execute(CommandRouter::Split);
  events(app);
  auto* toolbar = app.window->previewColors->getParent();
  CHECK(toolbar == app.window->previewControls->getParent());
  for (int width : {1900, 1100, 640, 450}) {
    app.window->resize(width, 800);
    events(app);
    CHECK(toolbar->getHeight() <= (width >= 1100 ? 40 : 110));
    for (auto* a = toolbar->getFirst(); a; a = a->getNext())
      if (a->shown()) {
        CHECK(a->getX() >= 0 && a->getX() + a->getWidth() <= toolbar->getWidth());
        CHECK(a->getY() + a->getHeight() <= toolbar->getHeight());
        for (auto* b = a->getNext(); b; b = b->getNext())
          if (b->shown())
            CHECK(
                a->getX() + a->getWidth() <= b->getX() || b->getX() + b->getWidth() <= a->getX() ||
                a->getY() + a->getHeight() <= b->getY() || b->getY() + b->getHeight() <= a->getY());
      }
  }
  app.window->resize(1200, 800);
  events(app);
  auto* editor = app.window->editor;
  const auto token = app.session.view().token;
  const auto source = app.session.view().text;
  editor->setCursorPos(12);
  editor->setSelection(5, 7);
  const auto baseSize = editor->getFont()->getSize();
  app.execute(CommandRouter::A4);
  events(app);
  CHECK(editor->getTextStyle() & TEXT_FIXEDWRAP);
  CHECK(editor->getWrapColumns() > 20);
  CHECK(editor->getFont()->getSize() < baseSize);
  CHECK(editor->getContentWidth() <= editor->getViewportWidth());
  int columns = editor->getWrapColumns();
  app.execute(CommandRouter::ActualSize);
  events(app);
  CHECK(editor->getFont()->getSize() == baseSize && editor->getWrapColumns() == columns);
  CHECK(editor->getCursorPos() == 12 && editor->getSelStartPos() == 5 &&
        editor->getSelEndPos() == 12);
  app.execute(CommandRouter::WindowWrap);
  events(app);
  CHECK(!(editor->getTextStyle() & TEXT_FIXEDWRAP));
  CHECK(app.session.view().token == token && app.session.view().text == source &&
        !app.session.dirty());
  ReadingColors colors{-1, 10, 45, 85};
  app.window->previewColors->changed(colors, true);
  const auto palette = ReadingPalette::from(colors);
  auto native = [](std::uint32_t c) { return FXRGB((c >> 16) & 255, (c >> 8) & 255, c & 255); };
  CHECK(editor->getBackColor() == native(palette.background));
  CHECK(editor->getTextColor() == native(palette.text));
  CHECK(app.host->readingColors() == colors);
  app.execute(CommandRouter::ToggleTheme);
  CHECK(editor->getBackColor() ==
        native(ReadingPalette::from(ReadingColors::defaults(true)).background));
  app.execute(CommandRouter::ToggleTheme);
  CHECK(editor->getTextColor() == native(palette.text));
  for (auto command : {CommandRouter::WindowWrap, CommandRouter::A4}) {
    app.execute(command);
    if (command == CommandRouter::A4)
      app.execute(CommandRouter::FitWidth);
    events(app);
    const auto frame = app.host->frame();
    CHECK(app.host->interactive());
    bool checked = false;
    for (const auto& run : frame->runs)
      if (!run.link.empty()) {
        app.host->setViewport(std::max(0.0, run.bounds.y - 40));
        events(app);
        auto point = app.host->documentToView(
            {run.bounds.x + run.bounds.width / 2, run.bounds.y + run.bounds.height / 2});
        const auto status = app.window->status->getText();
        hover(app, point);
        CHECK(app.window->status->getText().find("/next.md") >= 0);
        CHECK(app.session.view().token == token);
        hover(app, point, LeaveNotify);
        CHECK(app.window->status->getText() == status);
        hover(app, point);
        app.host->setViewport(0);
        CHECK(app.window->status->getText() == status);
        checked = true;
        break;
      }
    CHECK(checked);
  }
  app.execute(CommandRouter::Editor);
  events(app);
  CHECK(app.window->previewColors->shown() && app.window->previewControls->shown());
  editor->appendText(FXString("\nEdited after layout changes"), true);
  CHECK(app.session.dirty() && app.edits.canUndo());
  app.edits.applyEdit(
      {0, app.session.view().text.size(),
       "[Web](https://example.org/a?b=1#part)\n\n[Absolute](/tmp/xfmd-target.md)\n"});
  app.execute(CommandRouter::Split);
  app.execute(CommandRouter::WindowWrap);
  events(app);
  for (int i = 0; i < 20 && !app.host->interactive(); ++i)
    events(app);
  CHECK(app.host->interactive());
  auto hoverToken = app.session.view().token;
  CHECK(app.host->frame()->token == hoverToken);
  int targets = 0;
  for (const auto& run : app.host->frame()->runs)
    if (run.text == "Web" || run.text == "Absolute") {
      auto point = app.host->documentToView(
          {run.bounds.x + run.bounds.width / 2, run.bounds.y + run.bounds.height / 2});
      hover(app, point);
      CHECK(app.window->status->getText() == run.link.c_str());
      CHECK(app.session.view().token == hoverToken);
      ++targets;
      hover(app, point, LeaveNotify);
    }
  CHECK(targets == 2);
}
TEST_MAIN(run)
