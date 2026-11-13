#include "application/Application.h"
#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "application/ui/SdpTree.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/NativeInput.h"
#include "support/Capture.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
#include <regex>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
std::string stage;
void until(Application& app, const std::function<bool()>& done) {
  for (int i=0;i<1500;++i) { settleNative(app.app,5); if(done()) return; }
  throw std::runtime_error("Overview timeout at " + stage + ": " + std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
NavigationItem* viewpoint(NavigationTree* tree, FXTreeItem* first, const std::string& suffix="/VP01") {
  for(auto* item=first;item;item=item->getNext()) {
    auto* row=static_cast<NavigationItem*>(item);
    if(row->action.target.size()>suffix.size() && row->action.target.substr(row->action.target.size()-suffix.size())==suffix) return row;
    tree->expandTree(item);
    if(auto* found=viewpoint(tree,item->getFirst(),suffix)) return found;
  }
  return nullptr;
}
#include "OverviewFixture.h"
void run() {
  CHECK(getenv("XFMD_SDP_TOOL"));
  const auto root=fs::path(FXSystem::getHomeDirectory().text())/"overview project";
  fs::create_directories(root/"SDP/SDL");
  const auto source=root/"SDP/SDL/Overview.design";
  // Canonical declaration and statement ordering is part of the SDL contract.
  std::ofstream(source) << overviewFixture;
  const std::string actualEngine=getenv("XFMD_SDP_TOOL");
  const auto calls=root/"calls.jsonl",wrapper=root/"instrumented-tool";
  setenv("XFMD_OVERVIEW_ENGINE",actualEngine.c_str(),1);
  setenv("XFMD_OVERVIEW_CALLS",calls.c_str(),1);
  std::ofstream(wrapper)<<R"py(#!/usr/bin/python3
import os,sys,json
with open(os.environ['XFMD_OVERVIEW_CALLS'],'a') as f:f.write(json.dumps(sys.argv[1:])+'\n')
os.execv(os.environ['XFMD_OVERVIEW_ENGINE'],[os.environ['XFMD_OVERVIEW_ENGINE']]+sys.argv[1:])
)py";
  fs::permissions(wrapper,fs::perms::owner_all);setenv("XFMD_SDP_TOOL",wrapper.c_str(),1);
  int argc=1;char name[]="sdp-overview-test";char* argv[]{name,nullptr};
  Application app;app.initialize(argc,argv);app.window->position(10,10,1250,850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()),app.window->id(),RevertToParent,CurrentTime);
  CHECK(app.startPath(root.string()));
  auto* workspace=app.window->workspacePanel;auto* panel=workspace->sdp;
  until(app,[&]{return panel->tree("sdl") && panel->tree("sdl")->getFirstItem();});
  settleNative(app.app,1000);
  nativeClick(workspace->tabs->getLast(),15,8);
  auto* tree=panel->tree("sdl");
  NavigationItem* row=nullptr;
  until(app,[&]{row=viewpoint(tree,tree->getFirstItem());return row!=nullptr;});
  int activations=0;SdpTarget actual;const auto callback=panel->activated;
  panel->activated=[&](const SdpTarget& t){++activations;actual=t;callback(t);};
  // Activity3 replaced legacy projection=overview and /diagram children with
  // producer-owned primary reading targets and chapter/object navigation.
  // Verify native selection, all generated diagrams, chapter delivery, exact
  // source navigation and Back against that supplied contract for all11 views.
  SdpToolJob catalogJob;
  auto command=sdpCommand();
  command.insert(command.end(),{root.string(),"discover","--json"});
  catalogJob.start(command);
  std::optional<SdpReply> catalog;
  until(app,[&]{if(auto result=catalogJob.take()) {
    if(!result->error.empty()) throw std::runtime_error(result->error);
    catalog=std::move(result->reply);
  } return catalog.has_value();});
  const std::vector<std::string> samples={"Browse","Desktop","Display","Api","Delivery",
      "ABuild","Navigation","Bus","MemoryData","PacketLittle","Properties"};
  for(int vp=1;vp<=11;++vp) {
    const auto suffix=std::string("/VP")+(vp<10?"0":"")+std::to_string(vp);
    stage=suffix+" locating viewpoint";
    NavigationItem* row=nullptr;
    until(app,[&]{row=viewpoint(tree,tree->getFirstItem(),suffix);return row!=nullptr;});
    const auto& supplied=catalog->nodes.at(static_cast<SdpItem*>(row)->node).target;
    const auto before=app.session.view().path;
    const int count=activations;
    tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
    if(row->hasItems()) {
      nativeKey(tree,XK_Right);settleNative(app.app,80);
      CHECK(activations==count && app.session.view().path==before);
    }
    stage=suffix+" primary selection";
    std::cerr<<stage<<" at "<<activations<<"\n";
    tree->setCurrentItem(row);nativeKey(tree,XK_Return);
    until(app,[&]{return activations==count+1 && app.session.view().path!=before &&
        app.host->interactive() && app.host->frame()->token==app.session.view().token;});
    CHECK(actual.operation==supplied.operation && actual.uri==supplied.uri &&
          actual.model==supplied.model && actual.revision==supplied.revision);
    const auto generated=app.session.view().path;
    const auto text=app.session.view().text;
    CHECK(text.find("# "+suffix.substr(1))==0);
    CHECK(text.find(samples.at(vp-1))!=std::string::npos);
    std::size_t diagrams=0;
    for(std::size_t at=0;(at=text.find("```mermaid",at))!=std::string::npos;at+=10) ++diagrams;
    std::size_t nativeDiagrams=0;
    for(const auto& run:app.host->frame()->runs)
      if(dynamic_cast<const DiagramScene*>(run.visual.get())) ++nativeDiagrams;
    CHECK(nativeDiagrams==diagrams);
    if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
      const fs::path out=evidence;fs::create_directories(out);
      std::ofstream(out/(suffix.substr(1)+"-reading.md"))<<text;
      captureDesktop(app.app,(out/(suffix.substr(1)+"-native.png")).c_str());
      auto target=PdfTarget::inspect((out/(suffix.substr(1)+"-native.pdf")).string());
      SharedTextMetrics metrics;auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
      ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
      CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    }
    const auto chapters=text.find("## Reading chapters");
    CHECK(chapters!=std::string::npos);
    std::smatch chapter;
    const auto chapterText=text.substr(chapters);
    CHECK(std::regex_search(chapterText,chapter,std::regex(R"(\[[^\]]+\]\((sdl-view://[^)]+)\))")));
    stage=suffix+" chapter selection";
    std::cerr<<stage<<" at "<<activations<<"\n";
    app.followLink(generated,chapter[1]);
    until(app,[&]{return activations==count+1 && app.session.view().path!=generated &&
        app.host->interactive() && app.host->frame()->token==app.session.view().token;});
    std::ifstream invoked(calls);const std::string argvLog((std::istreambuf_iterator<char>(invoked)),{});
    CHECK(argvLog.find("\"--uri\", \""+chapter[1].str()+"\"")!=std::string::npos);
    // Generated links reach the engine unchanged and bypass sidebar callbacks.
    const auto collection=app.session.view().path;
    const auto collectionText=app.session.view().text;
    std::smatch span;
    CHECK(std::regex_search(collectionText,span,std::regex(R"(sdl-source://(s[0-9]+)/([0-9]+)/([0-9]+))")));
    app.followLink(collection,span.str());
    CHECK(app.session.view().path==source && app.views->mode()==ViewMode::Editor);
    CHECK(app.window->editor->getSelStartPos()==std::stoi(span[2]) &&
          app.window->editor->getSelEndPos()==std::stoi(span[3]));
    CHECK(app.navigation->goBack() && app.session.view().path==collection);
    CHECK(app.session.view().text==collectionText && activations==count+1);
    CHECK(app.navigation->goBack() && app.session.view().path==generated);
    CHECK(app.session.view().text==text && activations==count+1);
    app.views->setMode(ViewMode::Preview);
  }
  if(const char* actualSource=getenv("XFMD_OVERVIEW_ACTUAL_SOURCE")) {
    CHECK(app.open(actualSource));
    until(app,[&]{return app.host->interactive() && app.host->frame()->token==app.session.view().token;});
    int actualScenes=0;std::size_t actualNodes=0,actualEdges=0;
    for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {
      ++actualScenes;
      actualNodes+=scene->nodes.size();actualEdges+=scene->edges.size();
      if(const char* nodes=getenv("XFMD_OVERVIEW_EXPECTED_NODES")) CHECK(scene->nodes.size()==std::stoul(nodes));
      if(const char* edges=getenv("XFMD_OVERVIEW_EXPECTED_EDGES")) CHECK(scene->edges.size()==std::stoul(edges));
    }
    const int expectedScenes=getenv("XFMD_OVERVIEW_EXPECTED_SCENES")?std::stoi(getenv("XFMD_OVERVIEW_EXPECTED_SCENES")):1;
    if(actualScenes!=expectedScenes) if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
      const fs::path out=evidence;
      std::ofstream(out/"actual-render-failure.txt") << app.host->frame()->readingText;
      for(const auto& run:app.host->frame()->runs) std::ofstream(out/"actual-runs.txt",std::ios::app) << run.text << "\n";
      captureDesktop(app.app,(out/"actual-render-failure.png").c_str());
    }
    CHECK(actualScenes==expectedScenes);
    if(const char* nodes=getenv("XFMD_OVERVIEW_EXPECTED_TOTAL_NODES")) CHECK(actualNodes==std::stoul(nodes));
    if(const char* edges=getenv("XFMD_OVERVIEW_EXPECTED_TOTAL_EDGES")) CHECK(actualEdges==std::stoul(edges));
    if(const char* expectedText=getenv("XFMD_OVERVIEW_EXPECTED_TEXT")) CHECK(app.session.view().text.find(expectedText)!=std::string::npos);
    if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
      const fs::path out=evidence;
      const std::string prefix=getenv("XFMD_OVERVIEW_ACTUAL_PREFIX")?getenv("XFMD_OVERVIEW_ACTUAL_PREFIX"):"vp02";
      settleNative(app.app,200);captureDesktop(app.app,(out/(prefix+"-actual.png")).c_str());
      auto target=PdfTarget::inspect((out/(prefix+"-actual.pdf")).string());SharedTextMetrics metrics;
      auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
      ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
      CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    }
  }
  app.sdpNavigation->stop();
}
TEST_MAIN(run)
