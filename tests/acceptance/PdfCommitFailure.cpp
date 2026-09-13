#include "application/export/ExportPipeline.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include "support/TestSupport.h"
int main(int argc, char** argv) {
  try {
    CHECK(argc == 2);
    xfmd::SharedTextMetrics metrics;
    xfmd::CmarkInterpreter parser;
    xfmd::MarkdownRenderer renderer;
    xfmd::ExportRequest request{
        {{1, 2}, "# New PDF\n", {}, false}, {}, metrics.fontSetId(), argv[1], {}};
    xfmd::ExportControl control;
    bool failed = false;
    try {
      xfmd::ExportPipeline::run(request, xfmd::PdfTarget::inspect(argv[1]), control, parser,
                                renderer, metrics);
    } catch (const xfmd::Error& e) {
      failed = e.code == xfmd::ErrorCode::Io;
    }
    CHECK(failed && !control.published);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
