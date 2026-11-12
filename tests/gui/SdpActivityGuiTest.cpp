#include "application/Application.h"
#include "application/ui/SdpTree.h"
#include "support/Capture.h"
#include "support/NativeInput.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
void until(Application& app, const std::function<bool()>& done) {
  for (int i = 0; i < 6000; ++i) {
    settleNative(app.app, 5);
    if (done())
      return;
  }
  throw std::runtime_error(std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
void realProducer(const std::string& project) {
  int argc = 1;
  char name[] = "activity-real-producer";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(20, 20, 1250, 850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  CHECK(app.startPath(project));
  auto* panel = app.window->workspacePanel->sdp;
  until(app, [&] { return panel->tree("activities") != nullptr; });
  CHECK(!panel->tree("sessions"));
  nativeClick(app.window->workspacePanel->tabs->getLast(), 15, 8);
  for (auto* child = panel->tabs->getFirst(); child; child = child->getNext())
    if (auto* tab = dynamic_cast<FXTabItem*>(child); tab && tab->getText() == "Activities")
      nativeClick(tab, 15, 8);
  const std::string group =
      getenv("XFMD_ACTIVITY_REAL_GROUP") ? getenv("XFMD_ACTIVITY_REAL_GROUP") : "Active";
  auto* tree = panel->tree("activities");
  auto* active = tree->getFirstItem();
  while (active && active->getText() != group.c_str())
    active = active->getNext();
  CHECK(active);
  tree->expandTree(active);
  until(app, [&] {
    auto* row = tree->getFirstItem();
    while (row && row->getText() != group.c_str())
      row = row->getNext();
    return row && row->getFirst() != nullptr;
  });
  // Atomic page presentation rebuilds widgets, so reacquire the current rows.
  active = tree->getFirstItem();
  while (active && active->getText() != group.c_str())
    active = active->getNext();
  CHECK(active && active->getFirst());
  auto* need = active->getFirst();
  tree->setCurrentItem(need);
  tree->makeItemVisible(need);
  tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] {
    return app.session.view().text.find("Lifecycle timeline") != std::string::npos &&
           app.host->interactive();
  });
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE")) {
    fs::create_directories(evidence);
    XWarpPointer(static_cast<Display*>(app.app.getDisplay()), None, tree->id(), 0, 0, 0, 0, 190,
                 400);
    settleNative(app.app, 150);
    captureDesktop(app.app, (fs::path(evidence) / "actual-activities-native.png").c_str());
    std::ofstream(fs::path(evidence) / "actual-activity-page.md") << app.session.view().text;
    const auto timeline = fs::path(app.session.view().path).parent_path() / "timeline.svg";
    if (fs::exists(timeline))
      fs::copy_file(timeline, fs::path(evidence) / "timeline.svg",
                    fs::copy_options::overwrite_existing);
    app.host->setViewport(100000);
    settleNative(app.app, 150);
    captureDesktop(app.app, (fs::path(evidence) / "actual-activity-timeline.png").c_str());
  }
}
void run() {
  if (const char* project = getenv("XFMD_ACTIVITY_REAL_PROJECT")) {
    realProducer(project);
    return;
  }
  auto home = fs::path(FXSystem::getHomeDirectory().text()), root = home / "activity-project",
       tool = home / "activity-tool";
  fs::create_directories(root / "SDP");
  auto keep = home / "keep.md";
  std::ofstream(keep) << "# Retained document\n";
  std::ofstream(tool) << R"py(#!/usr/bin/python3
import sys,os,json,pathlib
args=sys.argv[1:]; root=args[0]; area=root+'/SDP'; rev='a'*64
with open(root+'/calls','a') as f:f.write(' '.join(args)+'\n')
def out(x): print(json.dumps(dict(schema='sdptool/0.2',**x)))
def target(sel):return dict(operation='activity-view',project='demo',uri='activity-view://demo/'+sel,revision=rev)
def parent(deferred):
 n=dict(id='activities/state/active',kind='group',label='Active',state='available',target=target('state/active'))
 if deferred:n.update(childrenDeferred=True,expansionTarget=dict(operation='activity-tree',project='demo',uri='activity-tree://demo/state/active?parent=activities%2Fstate%2Factive',revision=rev))
 else:n['children']=['activities/state/active/activity/1']
 return n
if 'actions' in args:print(json.dumps(dict(schema='sdptool.actions/1',operation='actions',actions=[])))
elif 'discover' in args:out(dict(operation='discover',status='valid',root=root,area=area,navigation=dict(schema='sdptool/0.2',operation='tree',project='demo',roots=['activities'],nodes=[dict(id='activities',kind='tab',label='Activities',target=target('overview'),children=['activities/state/active']),parent(True)])))
elif 'tree' in args:out(dict(operation='activity-tree',project='demo',revision=rev,roots=['activities/state/active'],nodes=[parent(False),dict(id='activities/state/active/activity/1',kind='activity',label='Useful need',state='active',target=target('activity/1'))]))
elif 'view' in args:
 if '--model' in args:sys.exit(3)
 if os.environ.get('XFMD_ACTIVITY_FAILURE'):
  print(json.dumps(dict(schema='sdptool/0.2',error=dict(code='stale',message='Activity changed; Refresh'))),file=sys.stderr);sys.exit(1)
 output=args[args.index('--output')+1];path=output+'/entry.md';pathlib.Path(path).write_text('# Useful Activity page\n\nNeed and desired outcome.\n')
 out(dict(operation='activity-view',profile='activity/1',source=area,entry=path,directory=output,revision=rev))
)py";
  fs::permissions(tool, fs::perms::owner_all);
  setenv("XFMD_SDP_TOOL", tool.c_str(), 1);
  int argc = 1;
  char name[] = "activity-native-test";
  char* argv[]{name, nullptr};
  Application app;
  app.initialize(argc, argv);
  app.window->position(20, 20, 1250, 850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent,
                 CurrentTime);
  CHECK(app.startPath(root.string()));
  CHECK(app.open(keep.string()));
  auto* panel = app.window->workspacePanel->sdp;
  until(app, [&] { return panel->tree("activities") != nullptr; });
  CHECK(!panel->tree("sessions"));
  nativeClick(app.window->workspacePanel->tabs->getLast(), 15, 8);
  settleNative(app.app, 100);
  auto* tree = panel->tree("activities");
  auto activeRow = [&] {
    auto* n = tree->getFirstItem();
    while (n && !n->getText().contains("Active"))
      n = n->getNext();
    return n;
  };
  auto* active = activeRow();
  CHECK(active && !active->getFirst());
  tree->setCurrentItem(active);
  tree->makeItemVisible(active);
  tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app,
        [&] { return app.session.view().text.find("Useful Activity page") != std::string::npos; });
  CHECK(!activeRow()->getFirst());
  CHECK(app.open(keep.string()));
  app.edits.applyEdit({0, 0, "dirty "});
  const auto token = app.session.view().token;
  tree->expandTree(activeRow());
  until(app, [&] { return activeRow()->getFirst() != nullptr; });
  CHECK(app.session.dirty() && app.session.view().token == token);
  auto* need = activeRow()->getFirst();
  tree->setCurrentItem(need);
  tree->makeItemVisible(need);
  tree->setFocus();
  nativeKey(tree, XK_Return);
  settleNative(app.app, 200);
  CHECK(app.session.view().token == token);
  app.documents.chooseUnsaved = [] { return UnsavedChoice::Discard; };
  CHECK(app.open(keep.string()));
  setenv("XFMD_ACTIVITY_FAILURE", "1", 1);
  tree->setCurrentItem(activeRow()->getFirst());
  nativeKey(tree, XK_Return);
  until(app, [&] { return panel->status->getText().contains("stale:"); });
  CHECK(app.session.view().path == keep);
  unsetenv("XFMD_ACTIVITY_FAILURE");
  app.sdpNavigation->refresh(root.string());
  until(app, [&] { return panel->status->getText().contains("Navigation updated"); });
  tree = panel->tree("activities");
  until(app, [&] { return activeRow()->getFirst() != nullptr; });
  tree->setCurrentItem(activeRow()->getFirst());
  tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app,
        [&] { return app.session.view().text.find("Useful Activity page") != std::string::npos; });
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE")) {
    fs::create_directories(evidence);
    XWarpPointer(static_cast<Display*>(app.app.getDisplay()), None, tree->id(), 0, 0, 0, 0, 190,
                 400);
    settleNative(app.app, 150);
    captureDesktop(app.app, (fs::path(evidence) / "activities-native.png").c_str());
    fs::copy_file(root / "calls", fs::path(evidence) / "activity-tool-calls.log",
                  fs::copy_options::overwrite_existing);
    std::ofstream(fs::path(evidence) / "activity-page.md") << app.session.view().text;
  }
  // A catalog larger than the visible-row budget remains browsable by releasing
  // collapsed widget subtrees. Retained producer nodes do not consume row budget.
  SdpReply large;
  large.project = "large";
  large.roots = {"catalog"};
  SdpNode tab;
  tab.id = "catalog";
  tab.kind = "tab";
  tab.label = "Large catalog";
  for (int g = 0; g < 50; ++g) {
    SdpNode group;
    group.id = "catalog/group/" + std::to_string(g);
    group.kind = "group";
    group.label = "Group " + std::to_string(g);
    tab.children.push_back(group.id);
    for (int d = 0; d < 100; ++d) {
      SdpNode doc;
      doc.id = group.id + "/" + std::to_string(d);
      doc.kind = "document";
      doc.label = "Document " + std::to_string(d);
      group.children.push_back(doc.id);
      large.nodes.emplace(doc.id, doc);
    }
    large.nodes.emplace(group.id, group);
  }
  large.nodes.emplace(tab.id, tab);
  panel->present(std::move(large));
  tree = panel->tree("catalog");
  int visited = 0;
  for (auto* group = tree->getFirstItem(); group; group = group->getNext()) {
    tree->expandTree(group);
    int count = 0;
    for (auto* child = group->getFirst(); child; child = child->getNext())
      ++count;
    CHECK(count == 100);
    visited += count;
    tree->collapseTree(group);
    CHECK(!group->getFirst());
  }
  CHECK(visited == 5000);
}
TEST_MAIN(run)
