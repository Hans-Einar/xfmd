#include "application/Application.h"
#include "application/ui/SdpTree.h"
#include "support/NativeInput.h"
#include "support/Capture.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& done) {
  for (int i=0;i<12000;++i) { settleNative(app.app,5); if(done()) return; }
  throw std::runtime_error("Deferred timeout: " + std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
FXTreeItem* source(SdpPanel* panel, const std::string& name) {
  if (auto* tree=panel->tree("sdl"))
    for(auto* row=tree->getFirstItem();row;row=row->getNext())
      if(row->getText().contains(name.c_str())) return row;
  return nullptr;
}
void run() {
  const auto home=fs::path(FXSystem::getHomeDirectory().text());
  auto root=home/"deferred-project";
  const char* actual=getenv("XFMD_DISCOVERY_PROJECT");
  if (actual) root=actual;
  else {
    fs::create_directories(root/"SDP/SDL");
    for (const auto* name:{"First", "Second"})
      std::ofstream(root/"SDP/SDL"/(std::string(name)+".design"))
        <<"language design-core version 0.5.\nfunctionality Browse.\nunit Desktop.\nDesktop owns Browse.\n";
  }
  const auto modelName=actual ? "SDP/SDL/XFMD/XfmdDesktop.design" : "First.design";
  const auto other=home/"keep.md";
  std::ofstream(other)<<"# Keep this document\n";
  const auto wrapper=home/"instrumented-tool", calls=home/"tool-calls";
  const char* tool=getenv("XFMD_SDP_TOOL");CHECK(tool && *tool);
  setenv("XFMD_TEST_PRODUCER",tool,1);
  setenv("XFMD_TEST_CALLS",calls.c_str(),1);
  std::ofstream(wrapper)<<R"py(#!/usr/bin/python3
import os,sys,time,json
with open(os.environ['XFMD_TEST_CALLS'],'a') as f: f.write(' '.join(sys.argv[1:])+'\n')
mode=os.environ.get('XFMD_TEST_MODE','')
if 'actions' in sys.argv and mode in ('legacy','io'):
    code='arguments' if mode=='legacy' else 'io'
    print(json.dumps({'schema':'sdptool/0.2','error':{'code':code,'message':'fixture command unavailable'}}),file=sys.stderr)
    sys.exit(1)
if 'tree' in sys.argv:
    time.sleep(0.2)
    if mode=='stale':
        print(json.dumps({'schema':'sdptool/0.2','error':{'code':'stale','message':'Model changed; Refresh'}}),file=sys.stderr)
        sys.exit(1)
os.execv(os.environ['XFMD_TEST_PRODUCER'],[os.environ['XFMD_TEST_PRODUCER']]+sys.argv[1:])
)py";
  fs::permissions(wrapper,fs::perms::owner_all);setenv("XFMD_SDP_TOOL",wrapper.c_str(),1);
  int argc=1;char name[]="sdp-deferred-test";char* argv[]{name,nullptr};
  Application app;app.initialize(argc,argv);app.window->position(20,20,1250,850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()),app.window->id(),RevertToParent,CurrentTime);
  CHECK(app.startPath(root.string()));CHECK(app.open(other.string()));
  auto* panel=app.window->workspacePanel->sdp;
  until(app,[&]{return source(panel,modelName)!=nullptr;});
  auto* tree=panel->tree("sdl");auto* first=source(panel,modelName);
  CHECK(first->hasItems() && !first->getFirst());
  app.edits.applyEdit({0,0,"keep dirty "});const auto token=app.session.view().token;
  tree->setCurrentItem(first);tree->expandTree(first);
  if(!actual) tree->expandTree(source(panel,"Second.design")); // Serialized, not canceled.
  until(app,[&]{return source(panel,modelName)->getFirst()!=nullptr &&
      (actual || source(panel,"Second.design")->getFirst()!=nullptr);});
  CHECK(app.session.view().token==token && app.session.dirty());
  CHECK(app.session.view().path==other);
  int viewpoints=0;
  for(auto* n=source(panel,modelName)->getFirst();n;n=n->getNext()) ++viewpoints;
  CHECK(viewpoints==11);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    nativeClick(app.window->workspacePanel->tabs->getLast(),15,8);
    tree->makeItemVisible(source(panel,modelName)->getLast());settleNative(app.app,100);
    captureDesktop(app.app,(fs::path(evidence)/"deferred-model.png").c_str());
    fs::copy_file(calls,fs::path(evidence)/"tool-calls.log",fs::copy_options::overwrite_existing);
  }
  // Explicit refresh cancels work and reloads expanded sources at current revisions.
  app.window->workspacePanel->sdpRefresh();
  until(app,[&]{return panel->status->getText().contains("Reading SDP");});
  until(app,[&]{return panel->status->getText().contains("Navigation updated") &&
      source(panel,modelName) && source(panel,modelName)->getFirst();});
  CHECK(app.session.view().token==token && app.session.dirty());
  if(actual) {
    app.documents.chooseUnsaved=[] { return UnsavedChoice::Discard; };
    CHECK(app.open(other.string()));
    auto* row=source(panel,modelName)->getFirst();
    while(row && !row->getText().contains("VP02")) row=row->getNext();
    CHECK(row);
    tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
    nativeKey(tree,XK_Return);
    until(app,[&]{return app.session.view().path!=other && app.host->interactive();});
    CHECK(app.session.view().text.find("```mermaid")!=std::string::npos);
    if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
      settleNative(app.app,100);
      captureDesktop(app.app,(fs::path(evidence)/"actual-vp02.png").c_str());
      std::ofstream(fs::path(evidence)/"actual-vp02.md")<<app.session.view().text;
      fs::copy_file(calls,fs::path(evidence)/"tool-calls.log",fs::copy_options::overwrite_existing);
    }
    return;
  }
  // A failed model fetch preserves the document and remains retryable by label.
  setenv("XFMD_TEST_MODE","stale",1);
  app.sdpNavigation->refresh(root.string());
  until(app,[&]{return panel->status->getText().contains("stale:");});
  CHECK(app.session.view().token==token && !source(panel,modelName)->getFirst());
  unsetenv("XFMD_TEST_MODE");
  auto retry=static_cast<NavigationItem*>(source(panel,modelName))->action;
  panel->tree("sdl")->activated(retry);
  until(app,[&]{return source(panel,modelName)->getFirst()!=nullptr;});
  // A project switch cannot apply an outstanding model response to the new root.
  app.sdpNavigation->refresh(root.string());
  until(app,[&]{return panel->status->getText().contains("Loading SDL");});
  CHECK(app.window->workspacePanel->setWorkPath(home.string()));
  settleNative(app.app,500);CHECK(!panel->tree("sdl"));
  CHECK(app.session.view().token==token);
  // Legacy fallback is only for explicit unsupported-command arguments errors.
  setenv("XFMD_TEST_MODE","legacy",1);
  CHECK(app.window->workspacePanel->setWorkPath(root.string()));
  until(app,[&]{return source(panel,modelName)!=nullptr;});
  tree=panel->tree("sdl");tree->expandTree(source(panel,modelName));
  CHECK(source(panel,modelName)->getFirst());
  setenv("XFMD_TEST_MODE","io",1);
  const auto before=fs::file_size(calls);
  app.window->workspacePanel->sdpRefresh();
  until(app,[&]{return panel->status->getText().contains("io:");});
  std::ifstream log(calls);log.seekg(before);std::string tail((std::istreambuf_iterator<char>(log)),{});
  CHECK(tail.find(" discover ")==std::string::npos);
  CHECK(app.session.view().token==token && app.session.dirty());
  unsetenv("XFMD_TEST_MODE");
}
TEST_MAIN(run)
