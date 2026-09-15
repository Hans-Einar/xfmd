#include "DiagramPainter.h"
#include <cmath>
namespace xfmd {
namespace {
void color(cairo_t* cr, std::uint32_t rgb) {
  cairo_set_source_rgb(cr, ((rgb >> 16) & 255) / 255.0, ((rgb >> 8) & 255) / 255.0,
                       (rgb & 255) / 255.0);
}
void shape(cairo_t* cr, Rect r, DiagramShape type) {
  const double x = r.x, y = r.y, w = r.width, h = r.height;
  if (type == DiagramShape::Diamond) {
    cairo_move_to(cr, x + w / 2, y);
    cairo_line_to(cr, x + w, y + h / 2);
    cairo_line_to(cr, x + w / 2, y + h);
    cairo_line_to(cr, x, y + h / 2);
    cairo_close_path(cr);
  } else if (type == DiagramShape::Circle) {
    cairo_save(cr);
    cairo_translate(cr, x + w / 2, y + h / 2);
    cairo_scale(cr, w / 2, h / 2);
    cairo_arc(cr, 0, 0, 1, 0, 2 * 3.141592653589793);
    cairo_restore(cr);
  } else if (type == DiagramShape::Rounded) {
    const double radius = std::min(6.0, std::min(w, h) / 2), pi = 3.141592653589793;
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - radius, y + radius, radius, -pi / 2, 0);
    cairo_arc(cr, x + w - radius, y + h - radius, radius, 0, pi / 2);
    cairo_arc(cr, x + radius, y + h - radius, radius, pi / 2, pi);
    cairo_arc(cr, x + radius, y + radius, radius, pi, 3 * pi / 2);
    cairo_close_path(cr);
  } else
    cairo_rectangle(cr, x, y, w, h);
}
void arrow(cairo_t* cr, Point tip, Point next) {
  const double length = std::hypot(tip.x - next.x, tip.y - next.y);
  if (length < .001)
    return;
  const double dx = (tip.x - next.x) / length, dy = (tip.y - next.y) / length;
  cairo_move_to(cr, tip.x, tip.y);
  cairo_line_to(cr, tip.x - dx * 7 - dy * 3, tip.y - dy * 7 + dx * 3);
  cairo_line_to(cr, tip.x - dx * 7 + dy * 3, tip.y - dy * 7 - dx * 3);
  cairo_close_path(cr);
  cairo_fill(cr);
}
} // namespace
void DiagramPainter::paint(cairo_t* cr, const DiagramScene& scene, Rect bounds,
                           const ReadingPalette* value, bool active) {
  auto palette = value ? *value : ReadingPalette::from(ReadingColors::defaults(false));
  auto ink = active ? palette.text : palette.inactive;
  cairo_save(cr);
  cairo_translate(cr, bounds.x, bounds.y);
  cairo_scale(cr, bounds.width / scene.width, bounds.height / scene.height);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_set_line_width(cr, 1);
  for (auto box : scene.groups) {
    shape(cr, box, DiagramShape::Rounded);
    color(cr, palette.alternate);
    cairo_fill_preserve(cr);
    color(cr, palette.border);
    cairo_stroke(cr);
  }
  for (const auto& edge : scene.edges) {
    color(cr, ink);
    cairo_set_line_width(cr, edge.style == 2 ? 2.5 : 1.2);
    const double dash[] = {3, 3};
    cairo_set_dash(cr, dash, edge.style == 1 ? 2 : 0, 0);
    cairo_move_to(cr, edge.points.front().x, edge.points.front().y);
    for (std::size_t i = 1; i < edge.points.size(); ++i)
      cairo_line_to(cr, edge.points[i].x, edge.points[i].y);
    cairo_stroke(cr);
    cairo_set_dash(cr, nullptr, 0, 0);
    if (edge.start) {
      for (std::size_t i = 1; i < edge.points.size(); ++i)
        if (std::hypot(edge.points[i].x - edge.points.front().x,
                       edge.points[i].y - edge.points.front().y) > .001) {
          arrow(cr, edge.points.front(), edge.points[i]);
          break;
        }
    }
    if (edge.end) {
      for (std::size_t i = edge.points.size() - 1; i > 0; --i)
        if (std::hypot(edge.points[i - 1].x - edge.points.back().x,
                       edge.points[i - 1].y - edge.points.back().y) > .001) {
          arrow(cr, edge.points.back(), edge.points[i - 1]);
          break;
        }
    }
  }
  cairo_set_line_width(cr, 1.2);
  for (const auto& node : scene.nodes) {
    shape(cr, node.bounds, node.shape);
    color(cr, palette.surface);
    cairo_fill_preserve(cr);
    color(cr, ink);
    cairo_stroke(cr);
  }
  cairo_restore(cr);
}
} // namespace xfmd
