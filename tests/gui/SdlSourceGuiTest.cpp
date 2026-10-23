#include "application/Application.h"
#include "application/composition/DiagramServices.h"
#include "application/export/ExportPipeline.h"
#include "application/ui/SdpTree.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/NativeInput.h"
#include "support/Capture.h"
#include <X11/keysym.h>
#include <fstream>
#include <regex>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& done) {
  for (int i=0;i<2400;++i) { settleNative(app.app,5); if(done()) return; }
  throw std::runtime_error("SDL detail timeout: " + std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
NavigationItem* findRow(NavigationTree* tree, FXTreeItem* first, const std::string& suffix) {
  for(auto* item=first;item;item=item->getNext()) {
    auto* row=static_cast<NavigationItem*>(item);
    if(row->action.target.size()>suffix.size() && row->action.target.substr(row->action.target.size()-suffix.size())==suffix) return row;
    tree->expandTree(item);
    if(auto* found=findRow(tree,item->getFirst(),suffix)) return found;
  }
  return nullptr;
}
void run() {
  CHECK(getenv("XFMD_SDP_TOOL"));
  const auto root=fs::path(FXSystem::getHomeDirectory().text())/"source project";
  fs::create_directories(root/"SDP/SDL");
  const auto entry=root/"SDP/SDL/System.design", child=root/"SDP/SDL/Åctors.design";
  const std::string entryText="language design-core version 0.6.\nsystem Demo.\nincludes \"Åctors.design\".\nDemo contains Caller.\n";
  const std::string childText=R"(language design-core version 0.6.
contract Body.
usecase Browse.
channel Bus.
unit Caller.
contract Events.
unit Handler.
scenario Happy.
usecase Isolated.
mode Normal.
message Notice.
actor Reader.
scenario Unrelated.
field Value.
Body has completeness = closed.
Body has-field Value.
Bus upholds Events.
Caller uses Bus as sender of Notice in mode Normal.
Events has completeness = closed.
Events permits Notice.
Handler uses Bus as receiver of Notice in mode Normal.
Happy exercises Browse.
Happy has completeness = closed.
Happy runs-in Normal.
Happy step 1 sends Notice from Caller to Handler via Bus.
Notice has message-kind = event.
Notice upholds Body.
Reader pursues Browse.
Reader pursues Isolated.
Unrelated has completeness = closed.
Unrelated runs-in Normal.
Unrelated step 1 sends Notice from Caller to Handler via Bus.
Value has presence = required.
Value has value-type = text.
)";
  std::ofstream(entry)<<entryText;std::ofstream(child)<<childText;
  int argc=1;char name[]="sdl-source-test";char* argv[]{name,nullptr};
  Application app;app.initialize(argc,argv);app.window->position(10,10,1250,850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()),app.window->id(),RevertToParent,CurrentTime);
  CHECK(app.startPath(root.string()));
  auto* panel=app.window->workspacePanel->sdp;
  until(app,[&]{return panel->tree("sdl") && panel->tree("sdl")->getFirstItem();});
  settleNative(app.app,1000);
  nativeClick(app.window->workspacePanel->tabs->getLast(),15,8);
  auto* tree=panel->tree("sdl");
  NavigationItem* row=nullptr;
  until(app,[&]{row=findRow(tree,tree->getFirstItem(),"/diagram/VP01-Browse");return row!=nullptr;});
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();nativeKey(tree,XK_Return);
  until(app,[&]{return app.host->interactive() && app.session.view().text.find("Scenario: Happy")!=std::string::npos;});
  const auto generated=app.session.view().path;
  const auto text=app.session.view().text;
  CHECK(text.find("Scenario: Unrelated")==std::string::npos);
  CHECK(text.find("Use case: Isolated")==std::string::npos);
  CHECK(text.find("Preconditions: not specified")!=std::string::npos);
  int scenes=0;for(const auto& r:app.host->frame()->runs) if(dynamic_cast<const DiagramScene*>(r.visual.get()))++scenes;
  CHECK(scenes==2);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;captureDesktop(app.app,(out/"focused-native.png").c_str());
    auto target=PdfTarget::inspect((out/"focused-native.pdf").string());SharedTextMetrics metrics;
    auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
  }
  std::smatch match;CHECK(std::regex_search(text,match,std::regex("sdl-source://(s[0-9]+)/([0-9]+)/([0-9]+)")));
  const auto uri=match.str(); const auto start=std::stoul(match[2]), end=std::stoul(match[3]);
  app.followLink(generated,uri);
  CHECK(app.session.view().path==child && app.views->mode()==ViewMode::Editor);
  CHECK(app.window->editor->getSelStartPos()==int(start) && app.window->editor->getSelEndPos()==int(end));
  CHECK(app.session.view().text.substr(start,end-start)==childText.substr(start,end-start));
  settleNative(app.app,400);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) captureDesktop(app.app,(fs::path(evidence)/"source-native.png").c_str());
  CHECK(app.open(generated));
  const auto rootStart=entryText.find("Demo contains Caller."), rootEnd=rootStart+std::string("Demo contains Caller.").size();
  app.followLink(generated,"sdl-source://s0/"+std::to_string(rootStart)+"/"+std::to_string(rootEnd));
  CHECK(app.session.view().path==entry);
  CHECK(app.window->editor->getSelStartPos()==int(rootStart));
  CHECK(app.window->editor->getSelEndPos()==int(rootEnd));
  CHECK(app.open(generated));
  const auto middle=entryText.find("Åctors")+1;
  app.followLink(generated,"sdl-source://s0/"+std::to_string(middle)+"/"+std::to_string(rootEnd));
  CHECK(app.session.view().path==generated);
  // Invalid metadata in links cannot change the current document.
  for(const auto& invalid:{"sdl-source://unknown/0/4","sdl-source://s0/999999/9999999","sdl-source://s0/0/4?extra=1"}) {
    app.followLink(generated,invalid);CHECK(app.session.view().path==generated);
  }
  app.edits.applyEdit({0,0,"dirty "});const auto token=app.session.view().token;
  app.documents.chooseUnsaved=[] {return UnsavedChoice::Cancel;};
  app.followLink(generated,uri);CHECK(app.session.dirty() && app.session.view().token==token);
  app.documents.chooseUnsaved=[] {return UnsavedChoice::Discard;};
  CHECK(app.open(generated));
  // A change in a different file invalidates this composed-model link.
  std::ofstream(entry)<<entryText<<"\n";
  app.followLink(generated,uri);CHECK(app.session.view().path==generated);
  CHECK(std::string(panel->status->getText().text()).find("source changed")!=std::string::npos);
  std::ofstream(entry)<<entryText;
  fs::rename(child,child.string()+".saved");fs::create_symlink(child.string()+".saved",child);
  app.followLink(generated,uri);CHECK(app.session.view().path==generated);
  fs::remove(child);fs::rename(child.string()+".saved",child);
  // Whole graph is restored; historical generated context remains usable.
  app.followLink(generated,uri);CHECK(app.session.view().path==child);
  app.sdpNavigation->stop();
}
TEST_MAIN(run)
