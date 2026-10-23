#include "application/Application.h"
#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/NativeInput.h"
#include "support/Capture.h"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace xfmd;
int main(int argc,char** argv) {
  if(argc!=3)return 2;
  const std::string source=argv[1],output=argv[2];
  int count=1; char name[]="diagram-native-probe";char* args[]{name,nullptr};
  try {
    Application app;app.initialize(count,args);app.window->position(10,10,1250,850);
    if(!app.startPath(source))return 3;
    bool ready=false;
    for(int i=0;i<500;++i){settleNative(app.app,20);if(app.host->interactive()&&app.host->frame()->key.token==app.session.view().token){ready=true;break;}}
    if(!ready)return 4;
    int diagrams=0;for(const auto& run:app.host->frame()->runs)if(dynamic_cast<const DiagramScene*>(run.visual.get()))++diagrams;
    captureDesktop(app.app,(output+".png").c_str());
    auto target=PdfTarget::inspect(output+".pdf");SharedTextMetrics metrics;auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    const auto pages=ExportPipeline::run(request,target,control,*parser,renderer,metrics);
    std::cout<<"Native diagram scenes="<<diagrams<<" PDF pages="<<pages<<'\n';
    return 0;
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
