#include "EditorWidget.h"
#include "application/adapters/FoxWheelScrollBar.h"
#include <algorithm>
using namespace FX;
namespace xfmd {
FXDEFMAP(EditorWidget)
editorMap[] = {FXMAPFUNC(SEL_KEYPRESS, 0, EditorWidget::onKeyPress),
               FXMAPFUNC(SEL_INSERTED, EditorWidget::ID_EDIT, EditorWidget::onChanged),
               FXMAPFUNC(SEL_DELETED, EditorWidget::ID_EDIT, EditorWidget::onChanged),
               FXMAPFUNC(SEL_REPLACED, EditorWidget::ID_EDIT, EditorWidget::onChanged)};
FXIMPLEMENT(EditorWidget, FXText, editorMap, ARRAYNUMBER(editorMap))
EditorWidget::EditorWidget(FXComposite* parent)
    : FXText(parent, this, ID_EDIT, TEXT_WORDWRAP | LAYOUT_FILL_X | LAYOUT_FILL_Y),
      projection(std::make_unique<TextProjection>("")) {
  horizontal = FoxWheelScrollBar::replace(horizontal);
  vertical = FoxWheelScrollBar::replace(vertical);
  setMarginLeft(12);
  setMarginRight(12);
  setMarginTop(10);
  setMarginBottom(10);
}
long EditorWidget::onKeyPress(FXObject* sender, FXSelector sel, void* data) {
  FoxWheelScrollBar::cancelTree(this);
  return FXText::onKeyPress(sender, sel, data);
}
long EditorWidget::onChanged(FXObject*, FXSelector, void*) {
  if (!projecting && edited) {
    auto text = getText();
    edited(std::string(text.text(), text.length()));
  }
  return 1;
}
void EditorWidget::applyProjection(const SourceSnapshot& source) {
  projecting = true;
  auto cursor = getCursorPos(), start = getSelStartPos(), end = getSelEndPos();
  projection = std::make_unique<TextProjection>(source.text);
  auto text = getText();
  if (std::string(text.text(), text.length()) != projection->text) {
    FoxWheelScrollBar::cancelTree(this);
    setText(projection->text.data(), static_cast<FXint>(projection->text.size()), false);
    setCursorPos(std::min(cursor, getLength()));
    if (end > start)
      setSelection(std::min(start, getLength()), std::max(0, std::min(end, getLength()) - start));
  }
  projecting = false;
}
std::size_t EditorWidget::sourceAnchor() const {
  return projection ? projection->sourceOffset(std::max(0, getTopLine())) : 0;
}
void EditorWidget::setSourceAnchor(SourceAnchor anchor) {
  FoxWheelScrollBar::cancelTree(this);
  scrolling = true;
  if (projection)
    setTopLine(static_cast<FXint>(projection->displayOffset(anchor.byte)));
  scrolling = false;
}
void EditorWidget::moveContents(FXint x, FXint y) {
  FXText::moveContents(x, y);
  if (!projecting && !scrolling && viewportChanged)
    viewportChanged(sourceAnchor());
}
} // namespace xfmd
