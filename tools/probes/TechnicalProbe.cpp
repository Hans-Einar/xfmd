#include <algorithm>
#include <fx.h>
#include <cmark.h>
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  FX::FXApp app("xfmd-p0", "xfmd");
  app.init(argc, argv);
  auto* window = new FX::FXMainWindow(&app, "xfmd P0: native fonts and source proof", nullptr, nullptr,
                                     FX::DECOR_ALL, 0, 0, 800, 450);
  auto* column = new FX::FXVerticalFrame(window, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);
  std::vector<FX::FXFont*> fonts;
  for (const auto& item : std::vector<std::pair<std::string, int>>{{"DejaVu Sans", 24}, {"DejaVu Sans", 18}, {"DejaVu Serif", 12}, {"DejaVu Sans Mono", 11}}) {
    auto* font = new FX::FXFont(&app, item.first.c_str(), item.second,
                              item.second >= 18 ? FX::FXFont::Bold : FX::FXFont::Normal);
    fonts.push_back(font);
    auto* label = new FX::FXLabel(column, "Heading / brødtekst / code — æøå", nullptr, FX::JUSTIFY_LEFT);
    label->setFont(font);
  }
  auto* text = new FX::FXText(column, nullptr, 0, FX::LAYOUT_FILL_X | FX::LAYOUT_FILL_Y);
  const std::string bytes = "\xef\xbb\xbf# Heading\r\næøå\nrepeat\r\nrepeat\n";
  text->setText(bytes.data(), static_cast<int>(bytes.size()));
  auto roundtrip = text->getText();
  assert(std::string(roundtrip.text(), roundtrip.length()) == bytes);
  app.create();
  for (auto* font : fonts) {
    assert(font->getTextWidth("abc", 3) > 0);
    std::cout << "font height=" << font->getFontHeight() << " width=" << font->getTextWidth("abc", 3) << '\n';
  }
  const std::string md = "# æøå\n\nrepeat &amp; **repeat**\n\n> - item\n>   - nested\n\n```\nrepeat\n```\n";
  auto* doc = cmark_parse_document(md.data(), md.size(), CMARK_OPT_DEFAULT);
  assert(doc);
  auto* it = cmark_iter_new(doc);
  while (cmark_iter_next(it) != CMARK_EVENT_DONE) {
    if (cmark_iter_get_event_type(it) != CMARK_EVENT_ENTER) continue;
    auto* node = cmark_iter_get_node(it);
    std::cout << cmark_node_get_type_string(node) << ' '
              << cmark_node_get_start_line(node) << ':' << cmark_node_get_start_column(node)
              << '-' << cmark_node_get_end_line(node) << ':' << cmark_node_get_end_column(node) << '\n';
  }
  cmark_iter_free(it);
  cmark_node_free(doc);
  std::string large;
  while (large.size() < 1024 * 1024) large += md;
  std::vector<double> samples;
  for (int i=0; i<30; ++i) {
    auto start=std::chrono::steady_clock::now();
    doc=cmark_parse_document(large.data(), large.size(), CMARK_OPT_DEFAULT);
    cmark_node_free(doc);
    samples.push_back(std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count());
  }
  std::sort(samples.begin(), samples.end());
  std::cout << "cmark=" << cmark_version_string() << " parse+free p95_ms=" << samples[28] << " bytes=" << large.size() << '\n';
  std::cout << "PASS: FOX byte roundtrip and font resources\n" << std::flush;
  window->show(FX::PLACEMENT_SCREEN);
  app.addTimeout(&app, FX::FXApp::ID_QUIT, 1500);
  return app.run();
}
