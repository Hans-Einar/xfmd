#include "application/Application.h"
#include "support/TestSupport.h"
#include <X11/Xlib.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
using namespace xfmd;
void pump(Application& a,int n=100){for(int i=0;i<n;i++){a.app.runWhileEvents();std::this_thread::sleep_for(std::chrono::milliseconds(3));}}
void click(Application& a,FoxRenderHost* host,const DrawRun& link){
  XEvent event{};auto* display=static_cast<Display*>(a.app.getDisplay());
  event.xbutton.display=display;event.xbutton.window=host->id();event.xbutton.root=DefaultRootWindow(display);
  event.xbutton.button=Button1;event.xbutton.same_screen=True;
  auto point=host->documentToView({link.bounds.x+2,link.bounds.y+2});event.xbutton.x=int(point.x)+host->getXPosition();event.xbutton.y=int(point.y)+host->getYPosition();
  event.type=ButtonPress;CHECK(XSendEvent(display,host->id(),False,ButtonPressMask,&event));event.type=ButtonRelease;CHECK(XSendEvent(display,host->id(),False,ButtonReleaseMask,&event));XFlush(display);
}
void run(){
 const char* tool=std::getenv("SDL_TOOL");CHECK(tool);
 char pattern[]="/tmp/xfmd-document-views-XXXXXX";std::filesystem::path dir=mkdtemp(pattern);
 struct Cleanup{std::filesystem::path dir;~Cleanup(){std::filesystem::remove_all(dir);}} cleanup{dir};
 auto source=(dir/"model.design").string(),nav=(dir/"navigator.md").string(),initial=(dir/"initial.md").string();
 std::ofstream(source)<<"language design-core version 0.5.\nunit Parser.\n";
 const std::string uri="sdl-view://demo/VP02?diagram=VP02-roots&target=main&consumer=xfmd";
 std::ofstream(nav)<<"# Navigator\n\n[Open architecture]("<<uri<<")\n";std::ofstream(initial)<<"# Initial view\n";
 int argc=1;char name[]="xfmd-document-views";char* argv[]={name,nullptr};Application a;a.initialize(argc,argv);CHECK(a.open(initial));
 DocumentViewConfig c{nav,tool,source,"demo","","gui-"+std::to_string(getpid())};a.documentViews=std::make_unique<DocumentViews>(a,c);pump(a,200);
 auto* host=a.documentViews->navigator->host;CHECK(host->interactive());const DrawRun* link=nullptr;
 for(const auto& run:host->frame()->runs)if(run.link==uri){link=&run;break;}CHECK(link);click(a,host,*link);
 // Change focus immediately; delivery must stay in the captured window/panel.
 a.host->setFocus();pump(a,400);CHECK(a.session.view().path.find("entry.md")!=std::string::npos);CHECK(a.session.view().text.find("Parser")!=std::string::npos);CHECK(a.documentViews->navigator->path()==nav);
 auto retained=a.session.view().path;CHECK(!a.documentViews->follow("sdl-view://other/VP02"));CHECK(a.session.view().path==retained);
 CHECK(a.documentViews->follow("sdl-view://demo/VP99"));pump(a,200);CHECK(a.session.view().path==retained);
 // Keep both documents available for external visual capture when requested.
 if(std::getenv("SDL_CAPTURE"))pump(a,1500);
 a.documentViews.reset();CHECK(std::filesystem::exists(retained)==false);
 try{WindowEndpoint::request(c.window,"XFMD1\tINFO\t"+c.window+"\n");CHECK(false);}catch(const std::exception&){}
}
TEST_MAIN(run)
