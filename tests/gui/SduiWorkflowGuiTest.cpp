#include "application/Application.h"
#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "application/ui/SdpTree.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/NativeInput.h"
#include "support/PathMenus.h"
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <fstream>
#include <regex>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& done) {
  for (int i = 0; i < 2400; ++i) {
    app.app.runWhileEvents();
    if (done())
      return;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  throw std::runtime_error("SDUI timeout: " + std::string(app.window->status->getText().text()));
}
Window fyneWindow(Display* display) {
  Window root, parent, *children = nullptr;
  unsigned count = 0;
  Window found = 0;
  if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &count)) {
    for (unsigned i = 0; i < count; ++i) {
      char* name = nullptr;
      if (XFetchName(display, children[i], &name) && name) {
        if (std::string(name).find("native prototype") != std::string::npos)
          found = children[i];
        XFree(name);
      }
    }
    if (children)
      XFree(children);
  }
  return found;
}
void run() {
  auto home = fs::path(FXSystem::getHomeDirectory().text());
  auto project = home / "SDUI project", area = project / "SDP";
  fs::create_directories(area / "SDL");
  auto source = area / "SDL/Prototype.sdui";
  std::ofstream(source) << R"(sdui 0.2;
Buttons=<ok=button("Save"), field=input("Name",value="Åse")>;
page=[header="## Prototype"; left=Buttons; hidden=Buttons {visible=false}];
)";
  std::ofstream(home / "other.md") << "# Other document\n";
  int argc = 1;
  char name[] = "sdui-workflow-test";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(10, 10, 1250, 850);
  auto* display = static_cast<Display*>(app.app.getDisplay());
  XSetInputFocus(display, app.window->id(), RevertToParent, CurrentTime);
  auto* workspace = app.window->workspacePanel;
  auto* panel = workspace->sdp;
  CHECK(app.startPath(project.string()));
  until(app, [&] { return panel->tree("sdui") && panel->tree("sdui")->getFirstItem(); });
  settleNative(app.app, 1100);
  nativeClick(workspace->tabs->getLast(), 15, 8);
  nativeClick(panel->tabs->getLast()->getPrev(), 15, 8); // Last tab item precedes its pane.
  settleNative(app.app, 250);
  CHECK(!app.window->sduiLaunch->getTipText().empty());
  auto* tree = panel->tree("sdui");
  auto* model = tree->getFirstItem();
  tree->expandTree(model);
  auto* frame = model->getFirst();
  CHECK(frame);
  tree->setCurrentItem(frame);
  tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] { return app.session.view().text.find("# SDUI composition") == 0; });
  const auto generated = app.session.view().path;
  until(app, [&] {
    return app.host->interactive() && app.host->frame()->token == app.session.view().token;
  });
  CHECK(app.session.view().text.find("hidden") != std::string::npos);
  int diagrams = 0;
  for (const auto& run : app.host->frame()->runs)
    if (dynamic_cast<const DiagramScene*>(run.visual.get()))
      ++diagrams;
  CHECK(diagrams == 3);
  until(app, [&] { return app.sdui->canLaunch(); });
  CHECK(app.window->sdpTools->shown());
  CHECK(!app.commands.enabled(CommandRouter::KanbanAdd));
  CHECK(!app.commands.enabled(CommandRouter::KanbanMove));
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (fs::path(evidence) / "sdui-combined.png").c_str());
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE")) {
    auto pdf = PdfTarget::inspect((fs::path(evidence) / "sdui-combined.pdf").string());
    SharedTextMetrics metrics;
    auto parser = DiagramServices::interpreter();
    MarkdownRenderer renderer;
    ExportControl control;
    ExportRequest request{app.session.snapshot(), {}, metrics.fontSetId(), pdf.path, {}};
    CHECK(ExportPipeline::run(request, pdf, control, *parser, renderer, metrics) > 0);
  }
  // Generated provenance points to exact original source spans.
  std::smatch match;
  auto text = app.session.view().text;
  CHECK(std::regex_search(text, match, std::regex("sdui-source://([0-9]+)/([0-9]+)")));
  app.followLink(generated, match.str());
  CHECK(app.session.view().path == source && app.views->mode() == ViewMode::Editor);
  CHECK(app.window->editor->getSelStartPos() == std::stoi(match[1]));
  CHECK(app.window->editor->getSelEndPos() == std::stoi(match[2]));
  CHECK(app.open(generated));
  until(app, [&] { return app.sdui->canLaunch(); });
  app.views->setMode(ViewMode::Preview);
  // Missing runtime disables the same command used by pointer/keyboard dispatch.
  setenv("XFMD_SDUI_FYNE", "/missing/sdui-fyne", 1);
  until(app, [&] { return !app.window->sduiLaunch->isEnabled(); });
  CHECK(!app.commands.enabled(CommandRouter::SduiLaunch));
  unsetenv("XFMD_SDUI_FYNE");
  until(app, [&] { return app.sdui->canLaunch(); });
  nativeClick(app.window->sduiLaunch, 15, 10);
  until(app, [&] { return fyneWindow(display) != 0; });
  Window fyne = fyneWindow(display);
  until(app, [&] {
    XWindowAttributes attributes{};
    return XGetWindowAttributes(display, fyne, &attributes) && attributes.map_state == IsViewable;
  });
  XRaiseWindow(display, fyne);
  XFlush(display);
  settleNative(app.app, 500);
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE"))
    captureDesktop(app.app, (fs::path(evidence) / "sdui-fyne-launch.png").c_str());
  CHECK(app.open((home / "other.md").string()));
  settleNative(app.app, 300);
  CHECK(fyneWindow(display) == fyne && !app.sdui->canLaunch());
  nativeClick(workspace->tabs->getFirst(), 15, 8);
  until(app, [&] { return !app.window->sdpTools->shown(); });
  CHECK(fyneWindow(display) == fyne);
  // Close the separate window using its normal WM_DELETE protocol.
  XEvent event{};
  event.xclient.type = ClientMessage;
  event.xclient.window = fyne;
  event.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", False);
  event.xclient.format = 32;
  event.xclient.data.l[0] = XInternAtom(display, "WM_DELETE_WINDOW", False);
  event.xclient.data.l[1] = CurrentTime;
  XSendEvent(display, fyne, False, NoEventMask, &event);
  XFlush(display);
  until(app, [&] { return fyneWindow(display) == 0; });
  // An executable that fails after spawn reports a visible diagnostic.
  auto failing = home / "failing host";
  std::ofstream(failing) << "#!/bin/sh\necho expected-native-failure >&2\nexit 7\n";
  fs::permissions(failing, fs::perms::owner_all);
  setenv("XFMD_SDUI_FYNE", failing.c_str(), 1);
  nativeClick(workspace->tabs->getLast(), 15, 8);
  CHECK(app.open(generated));
  until(app, [&] { return app.sdui->canLaunch(); });
  nativeClick(app.window->sduiLaunch, 15, 10);
  until(app, [&] { return app.window->status->getText().contains("expected-native-failure"); });
  unsetenv("XFMD_SDUI_FYNE");
  // Stale source must never highlight offsets from the old generated revision.
  CHECK(app.open(generated));
  std::ofstream(source, std::ios::app) << "# changed\n";
  app.followLink(generated, match.str());
  CHECK(app.session.view().path == generated && app.window->status->getText().contains("changed"));
}
TEST_MAIN(run)
