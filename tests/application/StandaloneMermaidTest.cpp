#include "application/composition/DiagramServices.h"
#include "application/document/DocumentCoordinator.h"
#include "application/export/ExportPipeline.h"
#include "application/io/FileOpenPolicy.h"
#include "contracts/diagram/DiagramScene.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
namespace fs = std::filesystem;
std::string read(const fs::path& path) {
  std::ifstream file(path, std::ios::binary);
  return {std::istreambuf_iterator<char>(file), {}};
}
void run() {
  char pattern[] = "/tmp/xfmd-mmd-XXXXXX";
  CHECK(mkdtemp(pattern));
  fs::path dir(pattern);
  struct Cleanup {
    fs::path path;
    ~Cleanup() { fs::remove_all(path); }
  } cleanup{dir};
  const auto original = read(XFMD_MMD_FIXTURE);
  auto path = dir / "diagram æ.MmD";
  const auto bytes = std::string("\xef\xbb\xbf") + original;
  std::ofstream(path, std::ios::binary) << bytes;
  LocalFileStore store;
  DocumentSession session;
  DocumentCoordinator docs(session, store);
  CHECK(FileOpenPolicy::classify(path.string()) == FileOpenKind::Mermaid);
  CHECK(docs.requestOpen(path.string()));
  CHECK(session.view().standaloneMermaid && !session.view().plainText);
  CHECK(session.view().text == bytes);
  auto parser = DiagramServices::interpreter();
  auto parsed = parser->parse(session.snapshot());
  CHECK(parsed->blocks.size() == 1 && parsed->blocks[0].diagram);
  CHECK(parsed->blocks[0].diagram->nodes.size() == 5);
  CHECK(parsed->blocks[0].diagram->edges.size() == 4);
  CHECK(parsed->blocks[0].source.begin == 3 && parsed->blocks[0].source.end == bytes.size());
  CHECK(parsed->blocks[0].diagramSource == original);
  CHECK(docs.save() && read(path) == bytes);
  SharedTextMetrics metrics;
  MarkdownRenderer renderer;
  auto model = DiagramServices::prepare(parsed, session.snapshot(), metrics, [] { return false; });
  CHECK(model->blocks[0].diagramScene && !model->blocks[0].diagramScene->svg.empty());
  auto pdf = dir / "standalone.pdf";
  ExportRequest request{session.snapshot(), {}, metrics.fontSetId(), pdf.string(), {}};
  ExportControl control;
  CHECK(ExportPipeline::run(request, PdfTarget::inspect(pdf.string()), control, *parser, renderer,
                            metrics) >= 1);
  CHECK(control.published && read(pdf).substr(0, 5) == "%PDF-");
  if (const char* output = getenv("XFMD_UI_EVIDENCE"))
    fs::copy_file(pdf, fs::path(output) / "standalone.pdf", fs::copy_options::overwrite_existing);
  auto token = session.view().token;
  CHECK(docs.save((dir / "raw.md").string()));
  CHECK(!session.view().standaloneMermaid && !session.view().plainText);
  CHECK(session.view().token.revision > token.revision);
  for (const auto& block : parser->parse(session.snapshot())->blocks)
    CHECK(!block.diagram);
  token = session.view().token;
  CHECK(docs.save((dir / "again.mmd").string()));
  CHECK(session.view().standaloneMermaid && session.view().token.revision > token.revision);
  CHECK(parser->parse(session.snapshot())->blocks[0].diagram);
  CHECK(docs.save((dir / "literal.txt").string()));
  CHECK(session.view().plainText && !session.view().standaloneMermaid);
  CHECK(!parser->parse(session.snapshot())->blocks[0].diagram);
  CHECK(read(dir / "literal.txt") == bytes);
  auto invalid = session.snapshot();
  invalid.plainText = false;
  invalid.standaloneMermaid = true;
  invalid.text = "this is not Mermaid\n";
  auto rejected = parser->parse(invalid);
  CHECK(!rejected->blocks[0].diagram && rejected->blocks[0].kind == BlockKind::Code);
  CHECK(rejected->blocks[0].diagramSource == invalid.text);
  CHECK(rejected->blocks[0].runs[0].text.find("Mermaid:") == 0);
  invalid.text.clear();
  CHECK(!parser->parse(invalid)->blocks[0].diagram);
  SourceSnapshot embedded{{1, 1}, "```mermaid\n" + original + "```\n", {}, false};
  CHECK(parser->parse(embedded)->blocks[0].diagram);
}
TEST_MAIN(run)
