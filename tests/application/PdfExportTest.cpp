#include "application/adapters/PdfOutput.h"
#include "application/export/ExportPipeline.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace xfmd;
std::string read(const std::string& path) {
  std::ifstream f(path);
  return {std::istreambuf_iterator<char>(f), {}};
}
template <class F> void fails(F operation) {
  bool caught = false;
  try {
    operation();
  } catch (const std::exception&) {
    caught = true;
  }
  CHECK(caught);
}
void test() {
  char name[] = "/tmp/xfmd-pdf-test-XXXXXX";
  CHECK(mkdtemp(name));
  std::filesystem::path directory(name);
  struct Cleanup {
    std::filesystem::path p;
    ~Cleanup() { std::filesystem::remove_all(p); }
  } cleanup{directory};
  auto path = (directory / "output.pdf").string();
  SharedTextMetrics metrics;
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  ExportRequest request{
      {{2, 9}, "# Unsaved æøå\n\nPDF content.\n", {}, false}, {}, metrics.fontSetId(), path, {}};
  request.frame =
      renderer.layout(*parser.parse(request.source), {600, 1, {LayoutMode::Paged, {}}}, metrics);
  ExportControl control;
  CHECK(ExportPipeline::run(request, PdfTarget::inspect(path), control, parser, renderer,
                            metrics) == 1);
  CHECK(control.published && read(path).substr(0, 5) == "%PDF-");
  auto original = read(path);
  // Real Cairo write failure, including finalization; existing PDF remains untouched.
  int full = ::open("/dev/full", O_WRONLY);
  CHECK(full >= 0);
  ExportControl disk;
  fails([&] { PdfOutput::write(*request.frame, metrics.catalog, full, disk); });
  ::close(full);
  CHECK(read(path) == original);
  // Target changed after the immutable overwrite decision.
  {
    PdfFilePublisher publisher(PdfTarget::inspect(path));
    std::ofstream(path) << "external";
    fails([&] { publisher.commit(control); });
  }
  CHECK(read(path) == "external");
  {
    PdfFilePublisher publisher(PdfTarget::inspect(path));
    ExportControl cancel;
    cancel.cancelled = true;
    fails([&] { publisher.commit(cancel); });
  }
  CHECK(read(path) == "external");
  auto absent = (directory / "absent.pdf").string();
  {
    PdfFilePublisher publisher(PdfTarget::inspect(absent));
    std::ofstream(absent) << "other writer";
    ExportControl c;
    fails([&] { publisher.commit(c); });
  }
  CHECK(read(absent) == "other writer");
  auto link = (directory / "link.pdf").string();
  ::symlink(path.c_str(), link.c_str());
  fails([&] { PdfTarget::inspect(link); });
  // Mismatched cached frame must be rebuilt from the frozen snapshot.
  request.source.token.revision++;
  request.source.text = "new content";
  ExportControl next;
  CHECK(ExportPipeline::run(request, PdfTarget::inspect(path), next, parser, renderer, metrics) ==
        1);
  CHECK(read(path).substr(0, 5) == "%PDF-");
  request.fonts++;
  ExportControl wrong;
  fails([&] {
    ExportPipeline::run(request, PdfTarget::inspect(path), wrong, parser, renderer, metrics);
  });
  for (auto& entry : std::filesystem::directory_iterator(directory))
    CHECK(entry.path().string().find("xfmd-pdf-") == std::string::npos ||
          entry.path().filename().string().find(".xfmd-pdf-") == std::string::npos);
}
TEST_MAIN(test)
