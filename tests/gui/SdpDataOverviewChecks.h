#pragma once
void checkDataOverview(Application& app, NavigationTree* tree, int& activations, SdpTarget& actual) {
  auto* row=viewpoint(tree,tree->getFirstItem(),"/VP09");CHECK(row);
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
  const auto before=activations;
  nativeKey(tree,XK_Right);settleNative(app.app,100);CHECK(activations==before);
  tree->setCurrentItem(row);nativeKey(tree,XK_Return);
  until(app,[&]{return activations==before+1 && app.host->interactive() && app.session.view().text.find("Database register")!=std::string::npos;});
  CHECK(actual.uri.find("/VP09?")!=std::string::npos && actual.uri.find("projection=overview")!=std::string::npos);
  const auto text=app.session.view().text;
  for(const auto* want:{"Data definitions","Database-backed persistence","Unit-held; persistence unspecified","Contract register","Variant register","Field register","Projection responsibilities","UnusedStore","UnusedShape","WireFamily","optional","sdl-source://"}) CHECK(text.find(want)!=std::string::npos);
  int scenes=0;std::size_t nodes=0,edges=0;
  for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {
    ++scenes;nodes+=scene->nodes.size();edges+=scene->edges.size();
  }
  CHECK(scenes==2 && nodes==9 && edges==7);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;captureDesktop(app.app,(out/"vp09-native.png").c_str());
    auto target=PdfTarget::inspect((out/"vp09-native.pdf").string());SharedTextMetrics metrics;
    auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    std::ofstream(out/"vp09-generated.md") << text;
  }
  // Contract structure remains a separate detail selection.
  row=viewpoint(tree,tree->getFirstItem(),"/VP09/diagram/VP09-contract-DataRecord");CHECK(row);
  const auto prior=activations;
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();nativeKey(tree,XK_Return);
  until(app,[&]{return activations==prior+1 && app.host->interactive() && app.session.view().text.find("Contract structure: DataRecord")!=std::string::npos;});
  CHECK(actual.uri.find("projection=overview")==std::string::npos);
  scenes=0;for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {++scenes;CHECK(scene->nodes.size()==2 && scene->edges.size()==1);}
  CHECK(scenes==1);
}
