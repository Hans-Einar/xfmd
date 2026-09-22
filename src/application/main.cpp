#include "Application.h"
#include "build/BuildVersion.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
int main(int argc, char** argv) {
  try {
    xfmd::DocumentViewConfig config;
    std::string target, pane="main", client="cli-"+std::to_string(getpid()), request="1", path;
    bool info=false;
    for(int i=1;i<argc;i++) {
      std::string a=argv[i];
      auto value=[&]{if(++i>=argc)throw std::runtime_error("Missing option argument");return std::string(argv[i]);};
      if(a=="--version"){std::cout<<"xfmd "<<xfmd::buildVersion()<<'\n';return 0;}
      if(a=="--help"){std::cout<<"Usage: xfmd [file|directory] [--navigator file.md --sdl-tool program --sdl-source model --project ID --renderer program --window-id ID]\n       xfmd --window ID --pane main|navigation [--client ID --request N] file.md\n       xfmd --window ID --info\n";return 0;}
      if(a=="--navigator")config.navigator=value();else if(a=="--sdl-tool")config.tool=value();
      else if(a=="--sdl-source")config.source=value();else if(a=="--project")config.project=value();
      else if(a=="--renderer")config.renderer=value();else if(a=="--window-id")config.window=value();
      else if(a=="--window")target=value();else if(a=="--pane")pane=value();
      else if(a=="--client")client=value();else if(a=="--request")request=value();
      else if(a=="--info")info=true;
      else if(a.rfind("--",0)==0||!path.empty())throw std::runtime_error("Unknown or duplicate argument");else path=a;
    }
    if(!target.empty()){
      std::string packet=info?"XFMD1\tINFO\t"+target+"\n":"XFMD1\tOPEN\t"+target+"\t"+client+"\t"+request+"\t"+pane+"\t"+path+"\tend\n";
      auto answer=xfmd::WindowEndpoint::request(target,packet);std::cout<<answer;return answer.rfind("OK\t",0)==0?0:1;
    }
    if(config.window.empty())config.window=std::to_string(getpid())+"-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    xfmd::Application application;
    int foxargc=1;char* foxargv[]={argv[0],nullptr};application.initialize(foxargc,foxargv);
    if(!path.empty()&&!application.startPath(path)){std::cerr<<application.window->status->getText().text()<<'\n';return 1;}
    if(!config.navigator.empty()){
      application.documentViews=std::make_unique<xfmd::DocumentViews>(application,config);
      std::cout<<"window="<<config.window<<std::endl;
    }
    return application.app.run();
  }catch(const std::exception& e){std::cerr<<"xfmd: "<<e.what()<<'\n';return 1;}
}
