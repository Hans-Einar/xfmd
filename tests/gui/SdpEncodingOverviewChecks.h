#pragma once
void checkEncodingOverview(Application& app, NavigationTree* tree, int& activations, SdpTarget& actual) {
  auto* row=viewpoint(tree,tree->getFirstItem(),"/VP10");CHECK(row);
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();
  const auto before=activations;
  nativeKey(tree,XK_Right);settleNative(app.app,100);CHECK(activations==before);
  tree->setCurrentItem(row);nativeKey(tree,XK_Return);
  until(app,[&]{return activations==before+1 && app.host->interactive() && app.session.view().text.find("Encoding register")!=std::string::npos;});
  CHECK(actual.uri.find("/VP10?")!=std::string::npos && actual.uri.find("projection=overview")!=std::string::npos);
  const auto text=app.session.view().text;
  for(const auto* want:{"Packet: PacketBig", "Packet: PacketLittle", "big-endian", "little-endian", "most-significant-first", "least-significant-first", "29 bits", "Variant coverage", "WireVariant", "No Encoding declared; no fixed wire layout", "Header:", "13–28 / 16", "sdl-source://"}) CHECK(text.find(want)!=std::string::npos);
  int scenes=0;
  for(const auto& run:app.host->frame()->runs) if(auto* scene=dynamic_cast<const DiagramScene*>(run.visual.get())) {
    ++scenes;
    CHECK(scene->svg.find("[1] Bits 0: PacketFlag")!=std::string::npos);
    for(const auto* label:{"PacketFlag","PacketNumber","PacketPayload"}) CHECK(scene->svg.find(label)!=std::string::npos);
  }
  CHECK(scenes==2);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;captureDesktop(app.app,(out/"vp10-native.png").c_str());
    auto target=PdfTarget::inspect((out/"vp10-native.pdf").string());SharedTextMetrics metrics;
    auto parser=DiagramServices::interpreter();MarkdownRenderer renderer;ExportControl control;
    ExportRequest request{app.session.snapshot(),{},metrics.fontSetId(),target.path,{}};
    CHECK(ExportPipeline::run(request,target,control,*parser,renderer,metrics)>0);
    std::ofstream(out/"vp10-generated.md") << text;
    std::ofstream(out/"vp10-source.design.txt") << overviewFixture;
  }
  row=viewpoint(tree,tree->getFirstItem(),"/VP10/diagram/VP10-PacketBig");CHECK(row);
  const auto prior=activations;const auto overviewPath=app.session.view().path;
  tree->setCurrentItem(row);tree->makeItemVisible(row);tree->setFocus();nativeKey(tree,XK_Return);
  until(app,[&]{return activations==prior+1 && app.session.view().path!=overviewPath && app.host->interactive() && app.host->frame()->token==app.session.view().token && app.session.view().text.find("Packet: PacketBig")!=std::string::npos;});
  CHECK(actual.uri.find("projection=overview")==std::string::npos);
  CHECK(app.session.view().text.find("Packet: PacketLittle")==std::string::npos);
  scenes=0;for(const auto& run:app.host->frame()->runs) if(dynamic_cast<const DiagramScene*>(run.visual.get())) ++scenes;
  CHECK(scenes==1);
  if(const char* evidence=getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out=evidence;captureDesktop(app.app,(out/"vp10-detail.png").c_str());
    std::ofstream(out/"vp10-detail.md") << app.session.view().text;
  }
}
