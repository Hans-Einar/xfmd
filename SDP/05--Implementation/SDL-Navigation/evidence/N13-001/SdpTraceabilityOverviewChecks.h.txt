#pragma once
void checkTraceabilityOverview(Application& app, NavigationTree* tree, int& activations, SdpTarget& actual, const fs::path& source) {
  auto* row=viewpoint(tree,tree->getFirstItem(),"/VP11");CHECK(row);
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
  const auto before=activations;
  nativeKey(tree,XK_Right);settleNative(app.app,100);CHECK(activations==before);
  tree->setCurrentItem(row);nativeKey(tree,XK_Return);
  until(app,[&]{return activations==before+1 && app.host->interactive() && app.host->frame()->token==app.session.view().token && app.session.view().text.find("Snapshot coverage")!=std::string::npos;});
  CHECK(actual.uri.find("/VP11?")!=std::string::npos && actual.uri.find("projection=overview")!=std::string::npos);
  const auto text=app.session.view().text;const auto generated=app.session.view().path;
  for(const auto* want:{"| Declarations | 47 |", "Overview.design, line", "| Facts | 87 |", "| Explicit properties | 29 |", "Reported model gaps", "Declaration register", "Complete fact registry", "repeatability = deterministic", "state-retention = stateful", "NO_ENCODING", "NO_PARTICIPANTS", "NO_ACTOR", "Missing properties are unspecified", "sdl-source://"}) CHECK(text.find(want)!=std::string::npos);
  CHECK(text.find("```mermaid")==std::string::npos && text.find("No diagrams match")==std::string::npos);
  for(const auto& run:app.host->frame()->runs) CHECK(!dynamic_cast<const DiagramScene*>(run.visual.get()));
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;captureDesktop(app.app,(out/"vp11-native.png").c_str());
    auto target=PdfTarget::inspect((out/"vp11-native.pdf").string());SharedTextMetrics metrics;
    auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    std::ofstream(out/"vp11-generated.md") << text;
    std::ofstream(out/"vp11-source.design.txt") << overviewFixture;
  }
  // Follow an actual property fact link and check the exact editor selection.
  const std::string input=overviewFixture,statement="Display has repeatability = deterministic.";
  const auto start=input.find(statement),end=start+statement.size();CHECK(start!=std::string::npos);
  const auto uri="sdl-source://s0/"+std::to_string(start)+"/"+std::to_string(end);
  CHECK(text.find(uri)!=std::string::npos);app.followLink(generated,uri);
  CHECK(app.session.view().path==source && app.views->mode()==ViewMode::Editor);
  CHECK(app.window->editor->getSelStartPos()==int(start) && app.window->editor->getSelEndPos()==int(end));
  CHECK(app.session.view().text.substr(start,end-start)==statement);
  settleNative(app.app,150);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) captureDesktop(app.app,(fs::path(evidence)/"vp11-source.png").c_str());
  CHECK(app.open(generated));
  app.views->setMode(ViewMode::Preview);
  until(app,[&]{return app.host->interactive() && app.host->frame()->token==app.session.view().token;});
}
