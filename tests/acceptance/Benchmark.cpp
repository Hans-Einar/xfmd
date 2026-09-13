#include "application/adapters/SharedTextMetrics.h"
#include "interpreter/CmarkInterpreter.h"
#include "renderer/MarkdownRenderer.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/resource.h>
using namespace xfmd;
int main(int argc, char** argv) {
  CmarkInterpreter parser;
  MarkdownRenderer renderer;
  SharedTextMetrics metrics;
  std::string source;
  int section = 0;
  while (source.size() < 1024 * 1024) {
    source += "## Section " + std::to_string(section++) + "\n\n";
    for (int line = 0; line < 6; ++line)
      source += "A local Markdown document contains readable paragraphs, **important details**, "
                "links and `code`. The renderer wraps words and preserves source anchors.\n";
    source += "\n- One useful item\n- Another useful item\n\n```cpp\nint example() { return 42; "
              "}\n```\n\n";
  }
  if (argc > 1)
    std::ofstream(argv[1], std::ios::binary) << source;
  std::vector<double> parseTimes, layoutTimes, totals;
  std::size_t runs = 0;
  const int samples=(argc>2 && std::string(argv[2])=="quick")?3:31;
  for (int i = 0; i < samples; ++i) {
    auto start = std::chrono::steady_clock::now();
    auto model = parser.parse({{1, 1}, source, "benchmark.md", false});
    auto parsed = std::chrono::steady_clock::now();
    LayoutRequest request{675,std::uint64_t(i)};
    if(std::getenv("XFMD_BENCH_PAGED"))request.profile.mode=LayoutMode::Paged;
    auto frame = renderer.layout(*model,request,metrics);
    auto end = std::chrono::steady_clock::now();
    runs = frame->runs.size();
    double parse = std::chrono::duration<double, std::milli>(parsed - start).count();
    double layout = std::chrono::duration<double, std::milli>(end - parsed).count();
    if (!i)
      std::cout << "cold_parse_layout_ms=" << parse + layout << '\n';
    else {
      parseTimes.push_back(parse);
      layoutTimes.push_back(layout);
      totals.push_back(parse + layout);
    }
  }
  auto p95 = [](std::vector<double> times) {
    std::sort(times.begin(), times.end());
    return times[std::size_t((times.size()-1)*.95)];
  };
  struct rusage usage{};
  getrusage(RUSAGE_SELF, &usage);
  std::cout << "bytes=" << source.size() << " runs=" << runs << " samples=" << samples-1 << "\n"
            << "parse_p95_ms=" << p95(parseTimes) << " layout_p95_ms=" << p95(layoutTimes)
            << " total_p95_ms=" << p95(totals) << " peak_rss_kib=" << usage.ru_maxrss << '\n';
}
