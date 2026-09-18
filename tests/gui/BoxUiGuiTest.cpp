#include "application/Application.h"
#include "support/Capture.h"
#include "support/TestSupport.h"
#include <chrono>
#include <fstream>
#include <fxkeys.h>
#include <thread>
using namespace xfmd;
using namespace FX;
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
void ready(Application& app) {
  for (int i = 0; i < 100; ++i) {
    events(app, 50);
    if (!app.preview->busy() && app.host->interactive())
      return;
  }
  throw std::runtime_error("BoxUI preview did not become ready");
}
template <class T> T* widget(FXWindow* root, const std::string& label = "") {
  if (auto* v = dynamic_cast<T*>(root)) {
    if constexpr (std::is_same_v<T, FoxBoxUiButton>) {
      if (label.empty() || v->getText() == label.c_str())
        return v;
    } else
      return v;
  }
  for (auto* c = root->getFirst(); c; c = c->getNext())
    if (auto* v = widget<T>(c, label))
      return v;
  return nullptr;
}
void key(FXWindow* w, FXuint code, bool release) {
  FXEvent e{};
  e.code = code;
  w->handle(w, FXSEL(release ? SEL_KEYRELEASE : SEL_KEYPRESS, 0), &e);
}
void activate(FoxBoxUiButton* button) {
  CHECK(button && button->isEnabled());
  key(button, KEY_space, false);
  key(button, KEY_space, true);
}
void run() {
  int argc = 1;
  char name[] = "boxui-gui";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  std::ifstream f(XFMD_BOXUI_FIXTURE);
  std::string json((std::istreambuf_iterator<char>(f)), {});
  app.edits.applyEdit({0, 0,
                       "# BoxUI native integration\n\n```boxui\nboxui 0.1\n" + json +
                           "\n```\n\nOrdinary Markdown after BoxUI.\n"});
  ready(app);
  events(app, 400);
  ready(app);
  auto* input = widget<FoxBoxUiField>(app.host);
  CHECK(input && !input->isEnabled());
  app.execute(CommandRouter::BoxUiPrototype);
  ready(app);
  input = widget<FoxBoxUiField>(app.host);
  CHECK(input && input->isEnabled());
  CHECK(input->getText() == "C1");
  input->setText("Draft — Æøå");
  input->getTarget()->handle(input, FXSEL(SEL_CHANGED, 1), nullptr);
  input->setFocus();
  input->setCursorPos(5);
  input->setSelection(1, 3);
  auto* same = input;
  app.execute(CommandRouter::ToggleTheme);
  events(app);
  CHECK(widget<FoxBoxUiField>(app.host) == same);
  CHECK(input->getText() == "Draft — Æøå");
  app.window->resize(950, 720);
  events(app, 400);
  ready(app);
  input = widget<FoxBoxUiField>(app.host);
  CHECK(input == same && input->getText() == "Draft — Æøå");
  key(input, KEY_Escape, false);
  CHECK(input->getText() == "C1");
  activate(widget<FoxBoxUiButton>(app.host, "Suspend"));
  ready(app);
  activate(widget<FoxBoxUiButton>(app.host, "Resume"));
  ready(app);
  activate(widget<FoxBoxUiButton>(app.host, "Suspend"));
  ready(app);
  input = widget<FoxBoxUiField>(app.host);
  input->setText("C2");
  input->getTarget()->handle(input, FXSEL(SEL_CHANGED, 1), nullptr);
  key(input, KEY_Return, false);
  key(input, KEY_Return, false);
  key(input, KEY_Return, true);
  ready(app);
  CHECK(widget<FoxBoxUiField>(app.host)->getText() == "C2");
  activate(widget<FoxBoxUiButton>(app.host, "Resume"));
  events(app);
  CHECK(std::string(app.window->status->getText().text()).find("context changed") !=
        std::string::npos);
  app.execute(CommandRouter::A4);
  ready(app);
  app.host->setViewScale(false, 1.5);
  events(app);
  input = widget<FoxBoxUiField>(app.host);
  CHECK(input && input->getText() == "C2");
  app.host->setViewport(100);
  events(app);
  CHECK(input->getParent()->getY() >= 0);
  app.host->setViewport(0);
  app.host->setViewScale(true);
  events(app);
  captureDesktop(app.app, "boxui-dark.png");
  app.execute(CommandRouter::ToggleTheme);
  events(app);
  captureDesktop(app.app, "boxui-light.png");
  auto before = app.boxUiSession.freeze(app.session.view().token);
  input = widget<FoxBoxUiField>(app.host);
  input->setText("UNSUBMITTED-DRAFT");
  input->getTarget()->handle(input, FXSEL(SEL_CHANGED, 1), nullptr);
  CHECK(app.startExport("boxui-native.pdf"));
  for (int i = 0; i < 200 && app.exporter && app.exporter->busy(); ++i)
    events(app, 50);
  CHECK(app.boxUiSession.freeze(app.session.view().token).stateRevision == before.stateRevision);
  app.edits.applyEdit({0, 0, "Changed\n\n"});
  CHECK(!app.boxUiSession.simulated());
  ready(app);
  CHECK(widget<FoxBoxUiField>(app.host) && !widget<FoxBoxUiField>(app.host)->isEnabled());
}
TEST_MAIN(run)
