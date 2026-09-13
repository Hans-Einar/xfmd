#include "application/export/ExportPipeline.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/resource.h>
int main(int argc, char** argv) {
  if (argc != 3)
    return 2;
  try {
    std::ifstream input(argv[1]);
    std::string source{std::istreambuf_iterator<char>(input), {}};
    auto begin = std::chrono::steady_clock::now();
    xfmd::SharedTextMetrics metrics;
    xfmd::CmarkInterpreter parser;
    xfmd::MarkdownRenderer renderer;
    xfmd::ExportRequest request{
        {{1, 1}, source, argv[1], false}, {}, metrics.fontSetId(), argv[2], {}};
    xfmd::ExportControl control;
    auto count = xfmd::ExportPipeline::run(request, xfmd::PdfTarget::inspect(argv[2]), control,
                                           parser, renderer, metrics);
    auto ms =
        std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - begin).count();
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);
    std::cout << "bytes=" << source.size() << " pages=" << count
              << " pdf_bytes=" << std::filesystem::file_size(argv[2]) << " export_ms=" << ms
              << " peak_rss_kib=" << usage.ru_maxrss << '\n';
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
