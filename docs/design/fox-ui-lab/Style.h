#pragma once
#include <algorithm>
#include <fx.h>
#include <stdexcept>
using namespace FX;

struct Style {
  FXColor surface, panel, field, text, muted, border, accent, selected, hover;
  int gap, inset, height, radius;
  bool classic;
  static Style load(const char* path, const char* name) {
    FXSettings settings;
    if (!settings.parseFile(path, true))
      throw std::runtime_error("Cannot read style profiles");
    auto color = [&](const char* key) {
      return settings.readColorEntry(name, key, FXRGB(240, 240, 240));
    };
    Style s{color("surface"),
            color("panel"),
            color("field"),
            color("text"),
            color("muted"),
            color("border"),
            color("accent"),
            color("selected"),
            color("hover"),
            std::clamp(settings.readIntEntry(name, "gap", 8), 4, 16),
            std::clamp(settings.readIntEntry(name, "inset", 12), 6, 24),
            std::clamp(settings.readIntEntry(name, "height", 34), 26, 48),
            std::clamp(settings.readIntEntry(name, "radius", 5), 0, 10),
            bool(settings.readBoolEntry(name, "classic", false))};
    return s;
  }
  void apply(FXWindow* window) const {
    window->setBackColor(surface);
    if (auto* frame = dynamic_cast<FXFrame*>(window)) {
      frame->setBaseColor(surface);
      frame->setBorderColor(border);
      frame->setHiliteColor(panel);
      frame->setShadowColor(border);
    }
    if (auto* label = dynamic_cast<FXLabel*>(window))
      label->setTextColor(text);
    if (auto* menu = dynamic_cast<FXMenuTitle*>(window)) {
      menu->setTextColor(text);
      menu->setSelBackColor(selected);
      menu->setSelTextColor(text);
    }
    if (auto* input = dynamic_cast<FXTextField*>(window)) {
      input->setBackColor(field);
      input->setTextColor(text);
      input->setCursorColor(accent);
    }
    if (auto* editor = dynamic_cast<FXText*>(window)) {
      editor->setBackColor(field);
      editor->setTextColor(text);
      editor->setCursorColor(accent);
    }
    if (auto* tree = dynamic_cast<FXTreeList*>(window)) {
      tree->setBackColor(panel);
      tree->setTextColor(text);
      tree->setSelBackColor(selected);
      tree->setSelTextColor(text);
      tree->setLineColor(border);
    }
    for (auto* child = window->getFirst(); child; child = child->getNext())
      apply(child);
  }
};
