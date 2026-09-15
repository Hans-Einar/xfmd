#include "application/Application.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/DrainEvents.h"
#include "support/TestSupport.h"
#include <chrono>
#include <thread>
using namespace xfmd;
void pump(Application& app) { for(int i=0;i<100;++i){drainEvents(app.app);std::this_thread::sleep_for(std::chrono::milliseconds(5));} }
void run() {
  int argc=1;char name[]="xfmd-mermaid-test";char* argv[]={name,nullptr};
  Application app;app.initialize(argc,argv);
  app.edits.applyEdit({0,0,"# Diagram\n\n```mermaid\nflowchart LR\nA[Blåbær] -->|Target| B{Ready}\n```\n\nAfter"});
  for(int i=0;i<8 && (!app.host->interactive() || app.host->frame()->readingText.find("Target")==std::string::npos);++i)pump(app);
  CHECK(app.host->interactive());
  auto frame=app.host->frame();
  unsigned diagrams=0;
  for(const auto& run:frame->runs) diagrams+=dynamic_cast<const DiagramScene*>(run.visual.get())!=nullptr;
  CHECK(diagrams==1 && frame->readingText.find("Blåbær\nReady\nTarget")!=std::string::npos);
  app.execute(CommandRouter::ToggleTheme);pump(app);
  CHECK(app.host->frame()==frame);
  app.window->resize(640,700);pump(app);
  CHECK(app.host->interactive());
  app.execute(CommandRouter::A4);pump(app);
  CHECK(app.host->interactive() && app.host->frame()->pages.slices.size()==1);
  app.edits.applyEdit({0,app.session.snapshot().text.size(),"```mermaid\nflowchart LR\nA-->B\nclick A bad()\n```"});
  for(int i=0;i<4;++i)pump(app);
  CHECK(app.host->interactive() && app.host->frame()->readingText.find("Mermaid:")!=std::string::npos);
}
TEST_MAIN(run)
