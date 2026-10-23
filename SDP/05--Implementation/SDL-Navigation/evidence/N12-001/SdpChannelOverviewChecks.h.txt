#pragma once
// Kept separate so the cross-viewpoint workflow remains readable as it grows.
void checkChannelOverview(Application& app, NavigationTree* tree, int& activations, SdpTarget& actual) {
  auto* row=viewpoint(tree,tree->getFirstItem(),"/VP08");CHECK(row);
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
  const auto before=activations;
  nativeKey(tree,XK_Right);settleNative(app.app,100);CHECK(activations==before);
  tree->setCurrentItem(row);nativeKey(tree,XK_Return);
  until(app,[&]{return activations==before+1 && app.host->interactive() && app.session.view().text.find("Scenario index")!=std::string::npos;});
  CHECK(actual.uri.find("/VP08?")!=std::string::npos && actual.uri.find("projection=overview")!=std::string::npos);
  const auto index=app.session.view().text;
  CHECK(index.find("Choose a mode")!=std::string::npos && index.find("Example")!=std::string::npos);
  CHECK(index.find("```mermaid")==std::string::npos);
  for(const auto& run:app.host->frame()->runs) CHECK(!dynamic_cast<const DiagramScene*>(run.visual.get()));
  for(const auto* mode:{"Offline","Online","Quiet"}) {
    const auto previous=activations;
    row=viewpoint(tree,tree->getFirstItem(),std::string("/VP08/overview-mode/")+mode);CHECK(row);
    tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();nativeKey(tree,XK_Return);
    until(app,[&]{return activations==previous+1 && app.host->interactive() && app.session.view().text.find(std::string("Selected mode: **")+mode)!=std::string::npos;});
    CHECK(actual.uri.find("/VP08?")!=std::string::npos && actual.uri.find(std::string("mode=")+mode)!=std::string::npos);
    const auto text=app.session.view().text;const bool quiet=std::string(mode)=="Quiet";
    for(const auto* want:{"Channel register","Scenario index","IdleBus","sdl-source://"}) CHECK(text.find(want)!=std::string::npos);
    CHECK((text.find("| [Example]")!=std::string::npos)==(std::string(mode)=="Online"));
    if(quiet) CHECK(text.find("No participation is declared")!=std::string::npos);
    else {
      const auto sender=std::string(mode)=="Online"?"Desktop":"Remote";
      CHECK(text.find(std::string("n_")+sender+" -->|sender| n_Bus")!=std::string::npos);
      CHECK(text.find("Participation register")!=std::string::npos);
    }
    int scenes=0;
    for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {
      ++scenes;
      if(scene->svg.find("IdleBus")!=std::string::npos) {CHECK(scene->nodes.size()==1);CHECK(scene->edges.empty());}
      else {CHECK(scene->nodes.size()==(quiet?1:3));CHECK(scene->edges.size()==(quiet?0:2));}
    }
    CHECK(scenes==2);
    if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
      const fs::path out=evidence;const std::string prefix=std::string("vp08-")+mode;
      captureDesktop(app.app,(out/(prefix+".png")).c_str());
      auto target=PdfTarget::inspect((out/(prefix+".pdf")).string());SharedTextMetrics metrics;
      auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
      ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
      CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
      std::ofstream(out/(prefix+".md")) << text;
    }
  }
  // The original individual Scenario stays a sequence, generated only on selection.
  row=viewpoint(tree,tree->getFirstItem(),"/VP08/diagram/VP08-Example");CHECK(row);
  const auto beforeDetail=activations;
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();nativeKey(tree,XK_Return);
  until(app,[&]{return activations==beforeDetail+1 && app.host->interactive() && app.session.view().text.find("sequenceDiagram")!=std::string::npos;});
  CHECK(actual.uri.find("projection=overview")==std::string::npos);
  int sequences=0;for(const auto& run:app.host->frame()->runs) if(dynamic_cast<const DiagramScene*>(run.visual.get())) ++sequences;
  CHECK(sequences==1);
}
