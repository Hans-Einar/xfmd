#include "MathTypesetter.h"
#include "contracts/DocumentTypes.h"
#include <cctype>
#include <core/formula.h>
#include <filesystem>
#include <fontconfig/fontconfig.h>
#include <glibmm/init.h>
#include <latex.h>
#include <mutex>
#include <pango/pangocairo.h>
#include <pangomm/init.h>
#include <platform/cairo/graphic_cairo.h>
#include <set>
#include <utils/utf.h>
namespace xfmd {
namespace {
void validate(const std::string& text) {
  if (text.size() > 16384)
    throw Error(ErrorCode::TooLarge, "Formula exceeds 16 KiB.");
  // Reject state-changing commands and external resources. This is formelsats, not a TeX engine.
  static const std::set<std::string> forbidden{"input",
                                               "include",
                                               "includegraphics",
                                               "write",
                                               "openin",
                                               "openout",
                                               "read",
                                               "csname",
                                               "def",
                                               "edef",
                                               "gdef",
                                               "xdef",
                                               "let",
                                               "newcommand",
                                               "renewcommand",
                                               "providecommand",
                                               "newenvironment",
                                               "renewenvironment",
                                               "newcolumntype",
                                               "definecolor",
                                               "DeclareMathOperator",
                                               "xml"};
  int depth = 0;
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '\\') {
      auto start = ++i;
      while (i < text.size() && std::isalpha(static_cast<unsigned char>(text[i])))
        ++i;
      if (forbidden.count(text.substr(start, i - start)))
        throw Error(ErrorCode::Unsupported, "Formula command is not supported.");
      if (i > start)
        --i;
    } else if (text[i] == '{') {
      if (++depth > 64)
        throw Error(ErrorCode::TooLarge, "Formula nesting exceeds 64 levels.");
    } else if (text[i] == '}' && --depth < 0)
      throw Error(ErrorCode::Parse, "Unbalanced formula braces.");
  }
  if (depth)
    throw Error(ErrorCode::Parse, "Unbalanced formula braces.");
}
std::string resources() {
  std::error_code error;
  auto executable = std::filesystem::read_symlink("/proc/self/exe", error);
  auto installed = executable.parent_path().parent_path() / "share/xfmd/math";
  if (!error && std::filesystem::exists(installed / ".clatexmath-res_root"))
    return installed.string();
  if (std::filesystem::exists(XFMD_MATH_RESOURCES "/.clatexmath-res_root"))
    return XFMD_MATH_RESOURCES;
  throw Error(ErrorCode::Io, "Math fonts are not installed.");
}
} // namespace
std::shared_ptr<const VisualResource> MathTypesetter::render(const std::string& text,
                                                             bool display) {
  validate(text);
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock(mutex);
  struct Runtime {
    Runtime() {
      Glib::init();
      Pango::init();
      tex::LaTeX::init(resources());
    }
    ~Runtime() {
      tex::LaTeX::release();
      pango_cairo_font_map_set_default(nullptr);
      FcConfigAppFontClear(nullptr);
    }
  };
  static Runtime runtime;
  tex::Formula formula;
  formula.setLaTeX(tex::utf82wide(text));
  tex::TeXRenderBuilder builder;
  std::unique_ptr<tex::TeXRender> layout(
      builder.setStyle(display ? tex::TexStyle::display : tex::TexStyle::text)
          .setTextSize(12)
          .setForeground(tex::black)
          .build(formula));
  double width = layout->getWidth() + 4, height = layout->getHeight() + 4;
  if (!(width > 0 && height > 0 && width <= 16384 && height <= 16384))
    throw Error(ErrorCode::TooLarge, "Formula dimensions exceed the limit.");
  cairo_rectangle_t bounds{0, 0, width, height};
  std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface(
      cairo_recording_surface_create(CAIRO_CONTENT_COLOR_ALPHA, &bounds), cairo_surface_destroy);
  auto context = Cairo::Context::create(
      Cairo::RefPtr<Cairo::Surface>(new Cairo::Surface(surface.get(), false)));
  tex::Graphics2D_cairo graphics(context);
  layout->draw(graphics, 2, 2);
  auto visual = std::make_shared<CairoVisual>(surface.get(), width, height,
                                              2 + layout->getBaseline() * (height - 4), true);
  surface.release();
  return visual;
}
} // namespace xfmd
