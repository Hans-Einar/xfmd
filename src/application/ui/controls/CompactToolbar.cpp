#include "CompactToolbar.h"
#include <algorithm>
using namespace FX;
namespace xfmd {
CompactToolbar::CompactToolbar(FXComposite* p)
    : FXHorizontalFrame(p, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 2, 2, 1, 1, 4, 0) {}
FXint CompactToolbar::rowsHeight(FXint width) const {
  int x = 2, height = 0, row = 0;
  for (auto* c = getFirst(); c; c = c->getNext()) {
    if (!c->shown())
      continue;
    int w =
        std::min(std::max(1, width - 4),
                 ((c->getLayoutHints() & LAYOUT_FIX_WIDTH) ? c->getWidth() : c->getDefaultWidth()));
    if (x > 2 && x + w > width - 2) {
      height += row;
      row = 0;
      x = 2;
    }
    x += w + 4;
    row = std::max(row, c->getDefaultHeight());
  }
  return height + row + 2;
}
FXint CompactToolbar::getDefaultHeight() { return rowsHeight(getParent()->getWidth()); }
void CompactToolbar::layout() {
  int x = 2, y = 1, row = 0;
  for (auto* c = getFirst(); c; c = c->getNext()) {
    if (!c->shown())
      continue;
    int w =
        std::min(std::max(1, getWidth() - 4),
                 ((c->getLayoutHints() & LAYOUT_FIX_WIDTH) ? c->getWidth() : c->getDefaultWidth()));
    int h = c->getDefaultHeight();
    if (x > 2 && x + w > getWidth() - 2) {
      y += row;
      row = 0;
      x = 2;
    }
    c->position(c == rightAligned ? getWidth() - 2 - w : x, y, w, h);
    x += w + 4;
    row = std::max(row, h);
  }
  flags &= ~FLAG_DIRTY;
}
} // namespace xfmd
