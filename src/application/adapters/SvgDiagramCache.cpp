#include "SvgDiagramCache.h"
#include "contracts/DocumentTypes.h"
#include <librsvg/rsvg.h>
#include <list>
#include <memory>
#include <pango/pangocairo.h>
namespace xfmd {
namespace {
struct Entry {
  std::string svg, css;
  std::unique_ptr<RsvgHandle, decltype(&g_object_unref)> handle{nullptr, g_object_unref};
};
struct Cache {
  std::list<Entry> entries;
  std::size_t bytes = 0;
  ~Cache() {
    // librsvg's text renderer uses Pango's per-thread default font map.
    // Drop handles first, then release that map while this thread is alive.
    entries.clear();
    pango_cairo_font_map_set_default(nullptr);
  }
};
thread_local Cache cache;
void check(bool ok, GError* error) {
  std::string message = error ? error->message : "Unable to render diagram SVG";
  if (error)
    g_error_free(error);
  if (!ok)
    throw Error(ErrorCode::Layout, message);
}
Entry& entryFor(const std::string& svg) {
  auto found = cache.entries.begin();
  while (found != cache.entries.end() && found->svg != svg)
    ++found;
  if (found == cache.entries.end()) {
    if (svg.empty() || svg.size() > 8 * 1024 * 1024)
      throw Error(ErrorCode::Layout, "Invalid diagram SVG size");
    GError* error = nullptr;
    Entry entry;
    entry.handle.reset(
        rsvg_handle_new_from_data(reinterpret_cast<const guint8*>(svg.data()), svg.size(), &error));
    check(bool(entry.handle), error);
    entry.svg = svg;
    while (!cache.entries.empty() &&
           (cache.bytes + svg.size() > 16 * 1024 * 1024 || cache.entries.size() >= 8)) {
      cache.bytes -= cache.entries.back().svg.size();
      cache.entries.pop_back();
    }
    cache.bytes += svg.size();
    cache.entries.push_front(std::move(entry));
  } else
    cache.entries.splice(cache.entries.begin(), cache.entries, found);
  return cache.entries.front();
}
} // namespace
void SvgDiagramCache::validate(const std::string& svg) { entryFor(svg); }
void SvgDiagramCache::paint(cairo_t* cr, const std::string& svg, Rect bounds,
                            const std::string& stylesheet) {
  auto& entry = entryFor(svg);
  GError* error = nullptr;
  if (entry.css != stylesheet) {
    bool ok = rsvg_handle_set_stylesheet(entry.handle.get(),
                                         reinterpret_cast<const guint8*>(stylesheet.data()),
                                         stylesheet.size(), &error);
    check(ok, error);
    error = nullptr;
    entry.css = stylesheet;
  }
  const RsvgRectangle viewport{bounds.x, bounds.y, bounds.width, bounds.height};
  bool ok = rsvg_handle_render_document(entry.handle.get(), cr, &viewport, &error);
  check(ok, error);
}
} // namespace xfmd
