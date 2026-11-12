#include "application/Application.h"
#include "application/ui/SdpTree.h"
#include "application/export/ExportPipeline.h"
#include "application/composition/DiagramServices.h"
#include "renderer/MarkdownRenderer.h"
#include "contracts/diagram/DiagramScene.h"
#include "support/NativeInput.h"
#include "support/Capture.h"
#include <X11/keysym.h>
#include <filesystem>
#include <fstream>
#include <regex>
using namespace xfmd;
using namespace FX;
namespace fs = std::filesystem;
std::string entryLabel = "System.design";
void until(Application& app, const std::function<bool()>& done) {
  for (int i = 0; i < 2000; ++i) {
    settleNative(app.app, 5);
    if (done()) return;
  }
  throw std::runtime_error(std::string(app.window->workspacePanel->sdp->status->getText().text()));
}
FXTreeItem* entry(NavigationTree* tree, FXTreeItem* first) {
  for (auto* row = first; row; row = row->getNext()) {
    auto* item = dynamic_cast<SdpItem*>(row);
    if (item && row->getText().contains(entryLabel.c_str())) return row;
    if (item && item->source.empty()) tree->expandTree(row);
    if (auto* found = entry(tree, row->getFirst())) return found;
  }
  return nullptr;
}
void captureStructure(Application& app) {
  const char* evidence = getenv("XFMD_UI_EVIDENCE");
  if (!evidence) return;
  const fs::path out = evidence; fs::create_directories(out);
  app.host->setViewport(0); settleNative(app.app, 100);
  captureDesktop(app.app, (out / "reading-structure-native.png").c_str());
  fs::create_directories(out / "structure-assets");
  std::ofstream(out / "structure-assets/entry.md") << app.session.view().text;
  auto target = PdfTarget::inspect((out / "reading-structure-native.pdf").string());
  SharedTextMetrics metrics; auto parser = DiagramServices::interpreter();
  MarkdownRenderer renderer; ExportControl control;
  ExportRequest request{app.session.snapshot(), {}, metrics.fontSetId(), target.path, {}};
  CHECK(ExportPipeline::run(request, target, control, *parser, renderer, metrics) > 0);
  int visuals = 0;
  for (const auto& run : app.host->frame()->runs) if (run.visual) {
    ++visuals;
    if (visuals <= 4) {
      app.host->setViewport(run.bounds.y); settleNative(app.app, 100);
      captureDesktop(app.app, (out / ("reading-structure-diagram-" + std::to_string(visuals) + ".png")).c_str());
    }
  }
  const auto bundle = fs::path(app.session.view().path).parent_path();
  for (const auto& file : fs::recursive_directory_iterator(bundle)) if (file.path().extension() == ".svg") {
    auto destination = out / "structure-assets" / fs::relative(file.path(), bundle);
    fs::create_directories(destination.parent_path());
    fs::copy_file(file.path(), destination, fs::copy_options::overwrite_existing);
  }
  std::cerr << "reading: structure native/PDF visuals " << visuals << "\n";
  app.host->setViewport(0);
}
void run() {
  CHECK(getenv("XFMD_SDP_TOOL"));
  auto root = fs::path(FXSystem::getHomeDirectory().text()) / "reading-project";
  const char* actualProject = getenv("XFMD_READING_REAL_PROJECT");
  if (actualProject) {
    root = actualProject;
    entryLabel = getenv("XFMD_READING_SOURCE_LABEL") ? getenv("XFMD_READING_SOURCE_LABEL") : "Micro.design";
  } else {
    fs::create_directories(root / "SDP/SDL");
    std::ofstream(root / "SDP/SDL/System.design")
        << "language design-core version 0.6.\nsystem ReadingDemo.\nincludes \"Parts.design\".\nReadingDemo contains Desktop.\n";
    std::ofstream(root / "SDP/SDL/Parts.design")
        << "language design-core version 0.6.\nfunctionality Browse.\ninterface Console.\ncontainer Desktop.\nunit Worker.\nDesktop contains Worker.\nWorker consumes Console.\nWorker owns Browse.\n";
  }
  const std::string entryContent = getenv("XFMD_READING_ENTRY_CONTENT") ? getenv("XFMD_READING_ENTRY_CONTENT") : actualProject ? "ValueRegistry" : "Worker";
  const std::string collectionContent = getenv("XFMD_READING_COLLECTION_CONTENT") ? getenv("XFMD_READING_COLLECTION_CONTENT") : actualProject ? "Contract" : "Console";
  const int collectionViewpoint = getenv("XFMD_READING_VIEWPOINT") ? std::stoi(getenv("XFMD_READING_VIEWPOINT")) : actualProject ? 8 : 4;
  const std::string collectionLabel = getenv("XFMD_READING_COLLECTION") ? getenv("XFMD_READING_COLLECTION") : actualProject ? "Contracts" : "Interfaces";
  int argc = 1; char name[] = "sdp-reading-test"; char* argv[]{name, nullptr};
  Application app; app.initialize(argc, argv); app.window->position(10, 10, 1250, 850);
  XSetInputFocus(static_cast<Display*>(app.app.getDisplay()), app.window->id(), RevertToParent, CurrentTime);
  CHECK(app.startPath(root.string()));
  auto* panel = app.window->workspacePanel->sdp;
  until(app, [&] { return panel->tree("sdl") != nullptr; });
  std::cerr << "reading: discovered native tabs\n";
  nativeClick(app.window->workspacePanel->tabs->getLast(), 15, 8);
  for (auto* child = panel->tabs->getFirst(); child; child = child->getNext())
    if (auto* tab = dynamic_cast<FXTabItem*>(child); tab && tab->getText() == "SDL") nativeClick(tab, 15, 8);
  auto* tree = panel->tree("sdl");
  FXTreeItem* source = nullptr;
  until(app, [&] { source = entry(tree, tree->getFirstItem()); return source != nullptr; });
  std::cerr << "reading: deferred entry visible\n";
  CHECK(!source->getFirst());
  int activations = 0; SdpTarget actual;
  const auto callback = panel->activated;
  panel->activated = [&](const SdpTarget& t) { actual = t; ++activations; callback(t); };
  // A deferred System entry is readable before its child tree is loaded.
  tree->setCurrentItem(source); tree->makeItemVisible(source); tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] { return activations == 1 && app.host->interactive() &&
      app.session.view().text.find(entryContent) != std::string::npos; });
  std::cerr << "reading: entry document delivered\n";
  captureStructure(app);
  CHECK(actual.operation == "select" && actual.uri.find("projection=reading") != std::string::npos);
  const auto selected = app.session.view().path;
  source = entry(tree, tree->getFirstItem());
  tree->setCurrentItem(source); tree->setFocus(); nativeKey(tree, XK_Right);
  until(app, [&] { auto* current = entry(tree, tree->getFirstItem()); return current && current->getFirst(); });
  std::cerr << "reading: model children loaded\n";
  CHECK(activations == 1 && app.session.view().path == selected);
  source = entry(tree, tree->getFirstItem());
  auto* viewpoint = source->getFirst();
  FXTreeItem* interfaces = nullptr;
  for (int vp = 1; vp <= 11; ++vp) {
    const auto prefix = std::string("VP") + (vp < 10 ? "0" : "") + std::to_string(vp);
    CHECK(viewpoint && viewpoint->getText().contains(prefix.c_str()));
    if (vp == collectionViewpoint) interfaces = viewpoint;
    viewpoint = viewpoint->getNext();
  }
  CHECK(interfaces);
  auto* sourceFiles = viewpoint;
  CHECK(sourceFiles && sourceFiles->getText() == "Source files");
  tree->expandTree(interfaces);
  auto* collection = interfaces->getFirst();
  while (collection && collection->getText() != collectionLabel.c_str()) collection = collection->getNext();
  CHECK(collection);
  tree->setCurrentItem(collection); tree->makeItemVisible(collection); tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] { return activations == 2 && app.host->interactive() &&
      app.session.view().path != selected && app.session.view().text.find(collectionContent) != std::string::npos; });
  CHECK(actual.uri.find("collection=") != std::string::npos);
  int expectedActivations = 2;
  if (const char* focus = getenv("XFMD_READING_FOCUS")) {
    tree->expandTree(collection);
    auto* object = collection->getFirst();
    while (object && object->getText() != focus) object = object->getNext();
    CHECK(object);
    const auto aggregate = app.session.view().path;
    tree->setCurrentItem(object); tree->makeItemVisible(object); tree->setFocus();
    nativeKey(tree, XK_Return);
    until(app, [&] { return activations == 3 && app.host->interactive() &&
        app.session.view().path != aggregate && app.session.view().text.find(focus) != std::string::npos; });
    CHECK(actual.uri.find("focus=") != std::string::npos);
    expectedActivations = 3;
  }
  CHECK(app.session.view().text.find("Complete fact registry") == std::string::npos);
  if (const char* evidence = getenv("XFMD_UI_EVIDENCE")) {
    const fs::path out = evidence; fs::create_directories(out);
    settleNative(app.app, 100); captureDesktop(app.app, (out / "reading-native.png").c_str());
    std::ofstream(out / "reading-page.md") << app.session.view().text;
    const auto bundle = fs::path(app.session.view().path).parent_path();
    for (const auto& file : fs::recursive_directory_iterator(bundle)) if (file.path().extension() == ".svg") {
      auto destination = out / fs::relative(file.path(), bundle);
      fs::create_directories(destination.parent_path());
      fs::copy_file(file.path(), destination, fs::copy_options::overwrite_existing);
    }
    auto target = PdfTarget::inspect((out / "reading-native.pdf").string());
    SharedTextMetrics metrics; auto parser = DiagramServices::interpreter();
    MarkdownRenderer renderer; ExportControl control;
    ExportRequest request{app.session.snapshot(), {}, metrics.fontSetId(), target.path, {}};
    CHECK(ExportPipeline::run(request, target, control, *parser, renderer, metrics) > 0);
    for (const auto& run : app.host->frame()->runs) if (run.visual) {
      app.host->setViewport(run.bounds.y); settleNative(app.app, 100);
      captureDesktop(app.app, (out / "reading-diagram-native.png").c_str());
      break;
    }
  }
  const auto generated = app.session.view().path;
  const auto text = app.session.view().text;
  std::smatch match;
  CHECK(std::regex_search(text, match, std::regex("sdl-source://(s[0-9]+)/([0-9]+)/([0-9]+)")));
  const auto uri = match.str();
  const auto start = std::stoul(match[2]), end = std::stoul(match[3]);
  app.followLink(generated, uri);
  CHECK(app.views->mode() == ViewMode::Editor && fs::path(app.session.view().path).extension() == ".design");
  CHECK(sdpContained((root / "SDP").string(), app.session.view().path));
  CHECK(app.window->editor->getSelStartPos() == int(start) && app.window->editor->getSelEndPos() == int(end));
  CHECK(app.session.view().text.substr(start, end - start).find(collectionContent) != std::string::npos);
  CHECK(app.navigation->goBack());
  CHECK(app.session.view().path == generated && app.session.view().text == text);
  app.views->setMode(ViewMode::Preview);
  until(app, [&] { return app.host->interactive() && app.host->frame()->token == app.session.view().token; });
  CHECK(activations == expectedActivations); // History reuses the retained document; no producer regeneration.
  tree->setCurrentItem(sourceFiles); tree->makeItemVisible(sourceFiles); tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] { return activations == expectedActivations + 1 && app.host->interactive() &&
      app.session.view().path != generated; });
  CHECK(actual.uri.find("projection=source") != std::string::npos);
  const auto sourceOverview = app.session.view().path;
  tree->expandTree(sourceFiles);
  auto* file = sourceFiles->getFirst(); CHECK(file);
  tree->setCurrentItem(file); tree->makeItemVisible(file); tree->setFocus();
  nativeKey(tree, XK_Return);
  until(app, [&] { return activations == expectedActivations + 2 && app.host->interactive() &&
      app.session.view().path != sourceOverview; });
  CHECK(actual.uri.find("projection=source") != std::string::npos);
  CHECK(actual.uri.find("source=") != std::string::npos && !app.session.view().text.empty());
  CHECK(app.navigation->goBack() && app.navigation->goBack());
  CHECK(app.session.view().path == generated);
  for (int vp = 1; vp <= 11; ++vp) {
    const auto prefix = std::string("VP") + (vp < 10 ? "0" : "") + std::to_string(vp);
    auto* currentSource = entry(tree, tree->getFirstItem()); CHECK(currentSource);
    auto* landing = currentSource->getFirst();
    for (int i = 1; i < vp && landing; ++i) landing = landing->getNext();
    CHECK(landing && landing->getText().contains(prefix.c_str()));
    const auto previous = app.session.view().path;
    tree->setCurrentItem(landing); tree->makeItemVisible(landing); tree->setFocus();
    nativeKey(tree, XK_Return);
    until(app, [&] { return activations == expectedActivations + 2 + vp &&
        app.session.view().path != previous && app.host->interactive() &&
        app.host->frame()->token == app.session.view().token; });
    CHECK(app.session.view().text.find(prefix) != std::string::npos);
    CHECK(!app.host->frame()->runs.empty());
    int scenes = 0, visuals = 0;
    for (const auto& run : app.host->frame()->runs) if (run.visual) ++visuals;
    for (const auto& run : app.host->frame()->runs)
      if (auto* scene = dynamic_cast<const DiagramScene*>(run.visual.get())) {
        ++scenes;
        CHECK(scene->width > 0 && scene->height > 0 && scene->svg.rfind("<svg", 0) == 0);
        std::cerr << "reading: " << prefix << " backend " << scene->diagnostics << "\n";
      }
    auto occurrences = [&](const std::string& needle) {
      std::size_t count = 0, at = 0;
      while ((at = app.session.view().text.find(needle, at)) != std::string::npos) { ++count; at += needle.size(); }
      return count;
    };
    CHECK(scenes == int(occurrences("```mermaid")));
    CHECK(visuals >= int(occurrences("![")) + scenes);
    std::cerr << "reading: native " << prefix << " landing admitted, diagrams " << scenes << ", visuals " << visuals << "\n";
  }
  for (int vp = 1; vp <= 11; ++vp) CHECK(app.navigation->goBack());
  CHECK(app.session.view().path == generated && app.session.view().text == text);
  CHECK(activations == expectedActivations + 13);

  if (!actualProject) {
    std::ofstream(root / "SDP/SDL/System.design", std::ios::app) << "\n";
    app.followLink(generated, uri);
    CHECK(app.session.view().path == generated);
    CHECK(panel->status->getText().contains("source changed"));
  }
}
TEST_MAIN(run)
