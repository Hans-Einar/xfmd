#include "application/Application.h"
#include "support/NativeInput.h"
#include <filesystem>
#include <fstream>
using namespace xfmd;
namespace fs = std::filesystem;
int main(int argc, char** argv) {
  try {
    CHECK(argc == 2);
    const fs::path output = argv[1];
    std::string source = "\xef\xbb\xbf# Zoom publication\r\n\r\n";
    for (int i = 0; i < 24; ++i)
      source += "## Marker" + std::to_string(i) +
                "\r\n\r\nUnicode æøå and **bold** text with `code`.\r\n\r\n";
    auto file = output / "source.md";
    std::ofstream(file, std::ios::binary) << source;
    argc = 1;
    Application app;
    app.initialize(argc, argv);
    CHECK(app.open(file.string()));
    app.edits.applyEdit(
        {app.session.view().text.size(), app.session.view().text.size(), "ENDOFFROZENBUFFER\r\n"});
    settleNative(app.app, 500);
    CHECK(app.host->interactive());
    const auto token = app.session.view().token;
    const auto text = app.session.view().text;
    for (int variant = 0; variant < 4; ++variant) {
      app.execute(variant == 3 ? CommandRouter::WindowWrap : CommandRouter::A4);
      app.execute(variant == 0   ? CommandRouter::ActualSize
                  : variant == 1 ? CommandRouter::Zoom300
                  : variant == 2 ? CommandRouter::FitHeight
                                 : CommandRouter::Zoom50);
      settleNative(app.app, 200);
      CHECK(app.startExport((output / ("zoom-" + std::to_string(variant) + ".pdf")).string()));
      for (int i = 0; app.exporter->busy() && i < 100; ++i)
        settleNative(app.app, 100);
      CHECK(!app.exporter->busy());
      CHECK(app.window->status->getText().find("Exported revision") >= 0);
      CHECK(app.session.view().token == token && app.session.view().text == text);
      CHECK(app.session.dirty() && app.edits.canUndo());
    }
    std::ifstream disk(file, std::ios::binary);
    CHECK(std::string(std::istreambuf_iterator<char>(disk), {}) == source);
    std::cout << "PASS zoom variants retain dirty buffer, undo and BOM/CRLF disk bytes\n";
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
