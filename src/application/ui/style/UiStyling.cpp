#include "UiContext.h"
#include "application/ui/controls/UiLayout.h"
using namespace FX;
namespace xfmd {
void UiContext::apply(FXWindow* window) {
  if (!window)
    return;
  if (dynamic_cast<FXRootWindow*>(window)) {
    for (auto* child = window->getFirst(); child; child = child->getNext())
      apply(child);
    return;
  }
  const auto& p = palette();
  auto m = metrics();
  auto* font = window->getApp()->getNormalFont();
  window->setBackColor(p.surface);
  if (auto* frame = dynamic_cast<FXFrame*>(window)) {
    frame->setBaseColor(p.surface);
    frame->setBorderColor(p.border);
    frame->setHiliteColor(p.panel);
    frame->setShadowColor(p.border);
  }
  if (auto* packer = dynamic_cast<FXPacker*>(window)) {
    packer->setBaseColor(p.surface);
    packer->setBorderColor(p.border);
    packer->setHiliteColor(p.panel);
    packer->setShadowColor(p.border);
  }
  if (auto* label = dynamic_cast<FXLabel*>(window)) {
    label->setFont(font);
    label->setTextColor(p.text);
  }
  if (auto* menu = dynamic_cast<FXMenuCaption*>(window)) {
    menu->setFont(font);
    menu->setTextColor(p.text);
    menu->setSelBackColor(p.selected);
    menu->setSelTextColor(p.text);
    menu->setHiliteColor(p.panel);
    menu->setShadowColor(p.border);
  }
  if (auto* title = dynamic_cast<FXMenuTitle*>(window)) {
    title->setSelBackColor(p.selected);
    title->setSelTextColor(p.text);
  }
  if (auto* field = dynamic_cast<FXTextField*>(window)) {
    field->setFont(font);
    field->setBackColor(p.field);
    field->setTextColor(p.text);
    field->setCursorColor(p.accent);
    field->setSelBackColor(p.selected);
    field->setSelTextColor(p.text);
    field->setPadTop(m.gap / 2);
    field->setPadBottom(m.gap / 2);
  }
  if (auto* edit = dynamic_cast<FXText*>(window)) {
    // Preserve the editor's own monospace font and document semantics.
    edit->setBackColor(p.field);
    edit->setTextColor(p.text);
    edit->setCursorColor(p.accent);
    edit->setSelBackColor(p.selected);
    edit->setSelTextColor(p.text);
    edit->setHiliteBackColor(p.selected);
    edit->setHiliteTextColor(p.text);
    edit->setActiveBackColor(p.field);
    edit->setBarColor(p.surface);
    edit->setNumberColor(p.muted);
  }
  if (auto* tree = dynamic_cast<FXTreeList*>(window)) {
    tree->setFont(font);
    tree->setBackColor(p.panel);
    tree->setTextColor(p.text);
    tree->setSelBackColor(p.selected);
    tree->setSelTextColor(p.text);
    tree->setLineColor(p.border);
  }
  if (auto* list = dynamic_cast<FXList*>(window)) {
    list->setFont(font);
    list->setBackColor(p.panel);
    list->setTextColor(p.text);
    list->setSelBackColor(p.selected);
    list->setSelTextColor(p.text);
  }
  if (auto* bar = dynamic_cast<FXScrollBar*>(window)) {
    bar->setBackColor(p.surface);
    bar->setHiliteColor(p.panel);
    bar->setShadowColor(p.border);
    bar->setBorderColor(p.border);
    bar->setArrowColor(p.text);
  }
  if (auto* tabs = dynamic_cast<FXTabItem*>(window)) {
    tabs->setPadLeft(m.inset);
    tabs->setPadRight(m.inset);
    tabs->setPadTop(m.gap);
    tabs->setPadBottom(m.gap);
  }
  if (auto* row = dynamic_cast<UiRow*>(window))
    row->restyle();
  if (auto* header = dynamic_cast<PanelHeader*>(window))
    header->restyle();
  for (auto* child = window->getFirst(); child; child = child->getNext())
    apply(child);
  window->recalc();
  window->update();
}
} // namespace xfmd
