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
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& done) {
  for (int i=0;i<1500;++i) { settleNative(app.app,5); if(done()) return; }
  throw std::runtime_error("Overview timeout: " + std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
NavigationItem* viewpoint(NavigationTree* tree, FXTreeItem* first) {
  for(auto* item=first;item;item=item->getNext()) {
    auto* row=static_cast<NavigationItem*>(item);
    if(row->action.target.size()>5 && row->action.target.substr(row->action.target.size()-5)=="/VP01") return row;
    tree->expandTree(item);
    if(auto* found=viewpoint(tree,item->getFirst())) return found;
  }
  return nullptr;
}
void run() {
  CHECK(getenv("XFMD_SDP_TOOL"));
  const auto root=fs::path(FXSystem::getHomeDirectory().text())/"overview project";
  fs::create_directories(root/"SDP/SDL");
  const auto source=root/"SDP/SDL/Overview.design";
  // Canonical declaration and statement ordering is part of the SDL contract.
  std::ofstream(source) << "language design-core version 0.5.\nusecase Browse.\nunit Desktop.\n"
    "functionality Display.\nusecase Export.\nactor Guest.\nusecase Isolated.\nfeature Navigation.\nactor Reader.\n"
    "Desktop owns Display.\nDisplay contributes-to Navigation.\nNavigation supports Browse.\nReader pursues Browse.\nReader pursues Export.\n";
  int argc=1;char name[]="sdp-overview-test";char* argv[]{name,nullptr};
  Application app;app.initialize(argc,argv);app.window->position(10,10,1250,850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()),app.window->id(),RevertToParent,CurrentTime);
  CHECK(app.startPath(root.string()));
  auto* workspace=app.window->workspacePanel;auto* panel=workspace->sdp;
  until(app,[&]{return panel->tree("sdl") && panel->tree("sdl")->getFirstItem();});
  settleNative(app.app,1000);
  nativeClick(workspace->tabs->getLast(),15,8);
  auto* tree=panel->tree("sdl");
  auto* row=viewpoint(tree,tree->getFirstItem());CHECK(row);
  int activations=0;SdpTarget actual;const auto callback=panel->activated;
  panel->activated=[&](const SdpTarget& t){++activations;actual=t;callback(t);};
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();settleNative(app.app,50);
  const auto before=app.session.view().path;
  nativeKey(tree,XK_Right);settleNative(app.app,120);
  CHECK(activations==0 && app.session.view().path==before);
  tree->setCurrentItem(row);
  nativeKey(tree,XK_Return);
  until(app,[&]{return app.session.view().path!=before && app.host->interactive();});
  CHECK(activations==1 && actual.uri.find("projection=overview")!=std::string::npos);
  CHECK(actual.operation=="select" && !actual.model.empty() && !actual.revision.empty());
  const auto text=app.session.view().text;
  for(const auto* label:{"usecase-beta","Guest","Isolated","Functionality realization","Display | contributes-to | Navigation"}) CHECK(text.find(label)!=std::string::npos);
  int scenes=0;
  for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {
    ++scenes;CHECK(scene->svg.find("usecase-actor")!=std::string::npos);CHECK(scene->svg.find("Isolated")!=std::string::npos);
  }
  CHECK(scenes==1);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;settleNative(app.app,150);captureDesktop(app.app,(out/"vp01-native.png").c_str());
    auto target=PdfTarget::inspect((out/"vp01-native.pdf").string());SharedTextMetrics metrics;
    auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    fs::copy_file(source,out/"native-input.design",fs::copy_options::overwrite_existing);
    fs::create_directories(out/"generated");
    for(const auto& entry:fs::directory_iterator(fs::path(app.session.view().path).parent_path()))
      fs::copy(entry.path(),out/"generated"/entry.path().filename(),fs::copy_options::recursive|fs::copy_options::overwrite_existing);
  }
  // Native label click activates the same exact overview target.
  row=viewpoint(tree,tree->getFirstItem());CHECK(row);tree->makeItemVisible(row);settleNative(app.app,50);
  bool clicked=false;
  for(int y=0;y<tree->getViewportHeight();++y) if(tree->getItemAt(150,y)==row) {nativeClick(tree,150,y+5);clicked=true;break;}
  CHECK(clicked);until(app,[&]{return activations==2;});CHECK(actual.uri.find("projection=overview")!=std::string::npos);
  app.sdpNavigation->stop();
}
TEST_MAIN(run)
