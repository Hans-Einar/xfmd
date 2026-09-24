#include "application/Application.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
#include <limits>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
FXMenuButton* scaleButton(Application& app) {
  return dynamic_cast<FXMenuButton*>(app.window->previewControls->getLast());
}
void stable(Application& app) {
  settleNative(app.app, 180);
  for (int i = 0; i < 40; ++i) {
    if (app.host->interactive() && app.host->frame()->token == app.session.view().token)
      return;
    settleNative(app.app, 50);
  }
  CHECK(false);
}
void percentage(Application& app, double value) {
  CHECK(std::abs(app.zoom->percent() - value) < .001);
  CHECK(std::abs(app.host->viewScale() - value / 100) < .00001);
  CHECK(scaleButton(app)->getText() == (std::to_string(int(std::lround(value))) + "%").c_str());
}
void captureZoom(Application& app, const char* name) {
  if (const auto* path = getenv("XFMD_UI_EVIDENCE")) {
    fs::path directory(path);
    if (const auto* dpi = getenv("XFMD_ZOOM_TEST_DPI"))
      directory /= std::string("dpi-") + dpi;
    fs::create_directories(directory);
    captureDesktop(app.app, (directory / name).c_str());
  }
}
void choose(Application& app, int entry) {
  auto* button = scaleButton(app);
  nativeClick(button, 15, 10);
  auto* menu = button->getMenu();
  CHECK(menu->shown());
  CHECK(menu->numChildren() == 8);
  CHECK(dynamic_cast<FXMenuSeparator*>(
      menu->getFirst()->getNext()->getNext()->getNext()->getNext()->getNext()));
  if (entry == 6)
    captureZoom(app, "zoom-toolbar-menu.png");
  auto* item = menu->getFirst();
  for (int i = 0; i < entry; ++i)
    item = item->getNext();
  CHECK(item && item->isEnabled());
  nativeClick(item, 15, item->getHeight() / 2);
  stable(app);
  CHECK(!menu->shown());
}
void run() {
  int argc = 1;
  char name[] = "document-zoom";
  char* argv[] = {name, nullptr};
  Application app;
  if (auto* dpi = getenv("XFMD_ZOOM_TEST_DPI"))
    app.app.reg().writeRealEntry("SETTINGS", "screenres", std::stod(dpi));
  app.initialize(argc, argv);
  if (auto* dpi = getenv("XFMD_ZOOM_TEST_DPI"))
    CHECK(std::abs(app.host->screenScale() - std::stod(dpi) / 72.0) < .001);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  fs::path home = FXSystem::getHomeDirectory().text();
  auto path = home / "zoom.md";
  std::string source;
  for (int i = 0; i < 40; ++i)
    source +=
        "## Chapter " + std::to_string(i) +
        "\n\nReadable text with **bold**, æøå and enough words to wrap across several lines.\n\n";
  std::ofstream(path) << source;
  CHECK(app.open(path.string()));
  stable(app);
  percentage(app, 100);
  app.edits.applyEdit({0, 0, "<!-- dirty -->\n"});
  stable(app);
  const auto token = app.session.view().token;
  const auto text = app.session.view().text;
  auto* editor = app.window->editor;
  const auto baseFont = editor->getFont()->getSize();
  editor->setCursorPos(30);
  editor->setSelection(20, 10);
  const auto paper = app.preview->layoutProfile().paper;
  for (auto layout : {CommandRouter::WindowWrap, CommandRouter::A4}) {
    app.execute(layout);
    for (auto mode : {ViewMode::Editor, ViewMode::Preview, ViewMode::Split}) {
      app.views->setMode(mode);
      app.execute(CommandRouter::ActualSize);
      stable(app);
      FXWindow* surface = mode == ViewMode::Preview ? static_cast<FXWindow*>(app.host) : editor;
      const auto anchorByte = text.find("## Chapter 15\n");
      app.scrolling.restoreAnchor({anchorByte});
      if (mode != ViewMode::Preview)
        editor->setSourceAnchor({anchorByte});
      surface->setFocus();
      nativeKey(surface, XK_equal, ControlMask);
      percentage(app, 110);
      nativeKey(surface, XK_minus, ControlMask);
      percentage(app, 100);
      nativeKey(surface, XK_plus, ControlMask | ShiftMask);
      percentage(app, 110);
      nativeKey(surface, XK_KP_Subtract, ControlMask);
      percentage(app, 100);
      nativeKey(surface, XK_KP_Add, ControlMask);
      percentage(app, 110);
      // Real X11 wheel dispatch, exactly one ten-point zoom step per notch.
      nativeClick(surface, 25, 25, ControlMask, Button4);
      percentage(app, 120);
      nativeClick(surface, 25, 25, ControlMask, Button5);
      percentage(app, 110);
      stable(app);
      CHECK(editor->getFont()->getSize() > baseFont);
      CHECK(app.scrolling.captureAnchor().byte == anchorByte);
      auto mapped = AnchorMapper::map({anchorByte}, *app.host->frame());
      CHECK(std::abs(-app.host->getYPosition() - app.host->documentToView({0, mapped.y}).y) < 3);
      if (mode != ViewMode::Preview)
        CHECK(editor->sourceAnchor() == anchorByte);
      CHECK(editor->getCursorPos() == 30 && editor->getSelStartPos() == 20 &&
            editor->getSelEndPos() == 30);
      CHECK(app.session.view().token == token && app.session.view().text == text &&
            app.session.dirty() && app.edits.canUndo());
      CHECK(app.preview->layoutProfile().paper == paper);
    }
  }
  app.views->setMode(ViewMode::Split);
  stable(app);
  const auto fixed = app.host->frame();
  const auto pageCount = fixed->pages.slices.size();
  for (int i = 0; i < 5; ++i) {
    choose(app, i);
    percentage(app, std::vector<int>{25, 50, 100, 200, 300}[i]);
    CHECK(app.host->frame() == fixed && app.host->frame()->pages.slices.size() == pageCount);
  }
  nativeKey(editor, XK_plus, ControlMask);
  percentage(app, 300);
  choose(app, 0);
  nativeKey(editor, XK_minus, ControlMask);
  percentage(app, 25);
  app.zoom->setPercent(std::numeric_limits<double>::quiet_NaN());
  percentage(app, 25);
  choose(app, 2);
  // Synthetic sub-notch input checks accumulation; native notches are covered above.
  FXEvent fine{};
  fine.code = 1;
  fine.state = CONTROLMASK;
  for (int i = 0; i < 120; ++i)
    editor->verticalScrollBar()->handle(editor, FXSEL(SEL_MOUSEWHEEL, 0), &fine);
  stable(app);
  percentage(app, 110);
  CHECK(!app.app.hasTimeout(editor->verticalScrollBar(), FoxWheelScrollBar::ID_MOTION));
  choose(app, 6);
  CHECK(app.zoom->mode() == ZoomMode::FitWidth);
  captureZoom(app, "zoom-a4-fit-width.png");
  auto widthFit = app.zoom->percent();
  app.zoom->step(0);
  CHECK(app.zoom->mode() == ZoomMode::FitWidth);
  app.window->resize(900, 650);
  stable(app);
  CHECK(app.zoom->percent() < widthFit);
  choose(app, 7);
  CHECK(app.zoom->mode() == ZoomMode::FitHeight);
  auto expectedHeight =
      100 * (std::min(editor->contentViewportHeight(), app.host->contentViewportHeight()) - 32) /
      (paper.height * app.host->screenScale());
  percentage(app, expectedHeight);
  app.window->resize(900, 750);
  stable(app);
  CHECK(app.zoom->percent() > expectedHeight);
  expectedHeight =
      100 * (std::min(editor->contentViewportHeight(), app.host->contentViewportHeight()) - 32) /
      (paper.height * app.host->screenScale());
  percentage(app, expectedHeight);
  captureZoom(app, "zoom-fit-height.png");
  nativeKey(editor, XK_plus, ControlMask);
  CHECK(app.zoom->mode() == ZoomMode::Manual);
  percentage(app, std::clamp(expectedHeight + 10, 25.0, 300.0));
  app.execute(CommandRouter::WindowWrap);
  app.execute(CommandRouter::ActualSize);
  stable(app);
  const auto width = app.host->frame()->width;
  app.host->setFocus();
  nativeKey(app.host, XK_a, ControlMask);
  const auto selected = app.host->selectedText();
  CHECK(!selected.empty());
  choose(app, 3);
  CHECK(app.host->frame()->width < width * .55);
  CHECK(app.host->selectedText() == selected);
  CHECK(app.host->getContentWidth() <= app.host->contentViewportWidth() + 2);
  CHECK(!app.commands.enabled(CommandRouter::FitWidth) &&
        !app.commands.enabled(CommandRouter::FitHeight));
  captureZoom(app, "zoom-wrap-200.png");
  CHECK(app.session.view().token == token && app.session.view().text == text &&
        app.edits.canUndo());
  for (int i = 0; i < 5; ++i) {
    choose(app, i);
    percentage(app, std::vector<int>{25, 50, 100, 200, 300}[i]);
    CHECK(app.host->selectedText() == selected);
  }
  auto* bar = app.window->previewControls->getParent()->getFirst();
  auto* viewTitle = dynamic_cast<FXMenuTitle*>(bar->getFirst()->getNext()->getNext());
  CHECK(viewTitle);
  FXMenuCascade* cascade = nullptr;
  for (auto* child = viewTitle->getMenu()->getFirst(); child; child = child->getNext())
    if (auto* item = dynamic_cast<FXMenuCascade*>(child))
      cascade = item;
  CHECK(cascade && cascade->getMenu()->numChildren() == 7);
  std::vector<int> commands{CommandRouter::Zoom25,     CommandRouter::Zoom50,
                            CommandRouter::ActualSize, CommandRouter::Zoom200,
                            CommandRouter::Zoom300,    CommandRouter::ZoomIn,
                            CommandRouter::ZoomOut};
  int i = 0;
  for (auto* item = cascade->getMenu()->getFirst(); item; item = item->getNext())
    CHECK(item->getSelector() == unsigned(commands[i++]));
  auto* in = dynamic_cast<FXMenuCommand*>(cascade->getMenu()->getLast()->getPrev());
  auto* out = dynamic_cast<FXMenuCommand*>(cascade->getMenu()->getLast());
  CHECK(in && out && in->getAccelText() == "Ctrl++" && out->getAccelText() == "Ctrl+-");
  nativeClick(viewTitle, 12, 10);
  nativeClick(cascade, 12, 10);
  CHECK(cascade->getMenu()->shown());
  captureZoom(app, "zoom-view-menu.png");
  nativeClick(cascade->getMenu()->getFirst()->getNext(), 15, 10);
  stable(app);
  percentage(app, 50);
  for (int change = 0; change < 12; ++change)
    app.execute(change % 2 ? CommandRouter::Zoom25 : CommandRouter::Zoom300);
  stable(app);
  percentage(app, 25);
  CHECK(app.host->selectedText() == selected);
  // Fit responds to actual visible content geometry in all three view modes.
  app.execute(CommandRouter::A4);
  for (auto mode : {ViewMode::Editor, ViewMode::Preview, ViewMode::Split}) {
    app.views->setMode(mode);
    app.execute(CommandRouter::FitHeight);
    stable(app);
    int height = mode == ViewMode::Editor ? editor->contentViewportHeight()
                 : mode == ViewMode::Preview
                     ? app.host->contentViewportHeight()
                     : std::min(editor->contentViewportHeight(), app.host->contentViewportHeight());
    percentage(app, 100 * (height - 32) / (paper.height * app.host->screenScale()));
    app.execute(CommandRouter::FitWidth);
    stable(app);
    auto beforeSidebar = app.zoom->percent();
    app.views->toggleSidebar();
    stable(app);
    CHECK(app.zoom->percent() > beforeSidebar);
    app.views->toggleSidebar();
    stable(app);
  }
  CHECK(app.session.view().token == token && app.session.view().text == text &&
        app.edits.canUndo());
  std::ifstream disk(path);
  CHECK(std::string(std::istreambuf_iterator<char>(disk), {}) == source);
}
TEST_MAIN(run)
