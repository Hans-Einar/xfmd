#include "DiagramPainter.h"
#include "SvgDiagramCache.h"
#include <iomanip>
#include <sstream>
namespace xfmd {
namespace {
std::string hex(std::uint32_t rgb) {
  std::ostringstream s;
  s << '#' << std::hex << std::setfill('0') << std::setw(6) << (rgb & 0xffffff);
  return s.str();
}
std::string mapping(const char* original, std::uint32_t value) {
  const auto color = hex(value);
  return std::string("[fill=\"") + original + "\"] {fill:" + color + " !important;}" +
         "[stroke=\"" + original + "\"] {stroke:" + color + " !important;}";
}
} // namespace
void DiagramPainter::paint(cairo_t* cr, const DiagramScene& scene, Rect bounds,
                           const ReadingPalette* value, bool active) {
  const auto palette = value ? *value : ReadingPalette::from(ReadingColors::defaults(false));
  const auto ink = active ? palette.text : palette.inactive;
  // Only the library's standard theme is mapped. Geometry, SVG text and arrows
  // remain exactly the library's presentation; no Mermaid reparsing or layout.
  const auto css = mapping("#FFFFFF", palette.background) + mapping("#F8FAFC", palette.surface) +
                   mapping("#F1F5F9", palette.alternate) + mapping("#E2E8F0", palette.alternate) +
                   mapping("#0F172A", ink) + mapping("#64748B", ink) +
                   mapping("#FFF7ED", palette.surface) + mapping("#FDBA74", palette.border) +
                   mapping("#94A3B8", palette.border) + mapping("#CBD5E1", palette.border) +
                   "text[fill=\"#FFFFFF\"] {fill:#FFFFFF !important;}";
  cairo_save(cr);
  try {
    SvgDiagramCache::paint(cr, scene.svg, bounds, css);
  } catch (...) {
    cairo_restore(cr);
    throw;
  }
  cairo_restore(cr);
}
} // namespace xfmd
