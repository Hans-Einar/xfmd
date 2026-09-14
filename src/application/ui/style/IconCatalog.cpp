#include "IconCatalog.h"
#include <algorithm>
#include <cmath>
#include <new>
using namespace FX;
namespace xfmd {
void IconCatalog::update(const UiPalette& p, int size) {
  for (int k = 1; k < int(UiIcon::Count); ++k)
    for (int disabled = 0; disabled < 2; ++disabled) {
      FXColor* pixels = nullptr;
      if (!FXMALLOC(&pixels, FXColor, size * size))
        throw std::bad_alloc();
      std::fill(pixels, pixels + size * size, FXRGBA(0, 0, 0, 0));
      const FXColor color = disabled ? p.muted : p.text;
      auto point = [&](int x, int y) {
        for (int dy = 0; dy < std::max(1, int(std::lround(size / 10.0))); ++dy)
          for (int dx = 0; dx < std::max(1, int(std::lround(size / 10.0))); ++dx)
            if (x + dx >= 0 && x + dx < size && y + dy >= 0 && y + dy < size)
              pixels[(y + dy) * size + x + dx] = color;
      };
      auto line = [&](double ax, double ay, double bx, double by) {
        int x = int(std::lround(ax * (size - 1) / 20)), y = int(std::lround(ay * (size - 1) / 20));
        int endX = int(std::lround(bx * (size - 1) / 20)),
            endY = int(std::lround(by * (size - 1) / 20));
        int dx = std::abs(endX - x), sx = x < endX ? 1 : -1, dy = -std::abs(endY - y),
            sy = y < endY ? 1 : -1, e = dx + dy;
        for (;;) {
          point(x, y);
          if (x == endX && y == endY)
            break;
          int e2 = 2 * e;
          if (e2 >= dy) {
            e += dy;
            x += sx;
          }
          if (e2 <= dx) {
            e += dx;
            y += sy;
          }
        }
      };
      auto rect = [&](int x, int y, int w, int h) {
        line(x, y, x + w, y);
        line(x + w, y, x + w, y + h);
        line(x + w, y + h, x, y + h);
        line(x, y + h, x, y);
      };
      auto circle = [&](double x, double y, double r, double begin = 0, double end = 6.283185307) {
        for (double a = begin; a < end; a += .15)
          line(x + r * std::cos(a), y + r * std::sin(a), x + r * std::cos(std::min(a + .15, end)),
               y + r * std::sin(std::min(a + .15, end)));
      };
      switch (UiIcon(k)) {
      case UiIcon::Open:
        line(2, 5, 7, 5);
        line(7, 5, 9, 8);
        line(9, 8, 17, 8);
        line(2, 5, 2, 17);
        line(2, 17, 15, 17);
        line(15, 17, 18, 10);
        line(18, 10, 5, 10);
        line(5, 10, 2, 17);
        break;
      case UiIcon::Save:
        rect(3, 2, 14, 16);
        rect(6, 2, 8, 6);
        rect(6, 12, 8, 6);
        break;
      case UiIcon::Back:
        line(3, 10, 9, 4);
        line(3, 10, 9, 16);
        line(3, 10, 17, 10);
        break;
      case UiIcon::Forward:
        line(17, 10, 11, 4);
        line(17, 10, 11, 16);
        line(3, 10, 17, 10);
        break;
      case UiIcon::Sidebar:
        rect(2, 3, 16, 14);
        line(7, 3, 7, 17);
        break;
      case UiIcon::Refresh:
        circle(10, 10, 7, .4, 5.6);
        line(15, 2, 15, 7);
        line(15, 7, 10, 7);
        break;
      case UiIcon::Search:
        circle(8, 8, 6);
        line(12, 12, 18, 18);
        break;
      case UiIcon::Theme:
        circle(10, 10, 8);
        for (int x = 3; x <= 10; ++x) {
          double h = std::sqrt(64 - (x - 10) * (x - 10));
          line(x, 10 - h, x, 10 + h);
        }
        break;
      case UiIcon::Editor:
        line(6, 5, 2, 10);
        line(2, 10, 6, 15);
        line(14, 5, 18, 10);
        line(18, 10, 14, 15);
        line(12, 4, 8, 16);
        break;
      case UiIcon::Split:
        rect(2, 3, 16, 14);
        line(10, 3, 10, 17);
        line(4, 7, 7, 7);
        line(4, 11, 7, 11);
        line(13, 7, 16, 7);
        line(13, 11, 16, 11);
        break;
      case UiIcon::Preview:
        line(1, 10, 5, 6);
        line(5, 6, 10, 4);
        line(10, 4, 15, 6);
        line(15, 6, 19, 10);
        line(19, 10, 15, 14);
        line(15, 14, 10, 16);
        line(10, 16, 5, 14);
        line(5, 14, 1, 10);
        circle(10, 10, 2);
        break;
      case UiIcon::Pdf:
        rect(4, 2, 12, 16);
        line(7, 7, 13, 7);
        line(7, 11, 13, 11);
        line(7, 14, 11, 14);
        break;
      default:
        break;
      }
      auto& icon = icons[std::size_t(k) * 2 + disabled];
      if (!icon)
        icon = std::make_unique<FXIcon>(&app);
      bool created = icon->id();
      if (created)
        icon->destroy();
      icon->setData(pixels, IMAGE_OWNED | IMAGE_KEEP, size, size);
      if (created)
        icon->create();
    }
}
} // namespace xfmd
