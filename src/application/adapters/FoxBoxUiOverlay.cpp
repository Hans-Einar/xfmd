#include "FoxBoxUiOverlay.h"
#include <algorithm>
#include <cmath>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxBoxUiOverlay)
overlayMap[] = {FXMAPFUNC(SEL_CHANGED, 1, FoxBoxUiOverlay::onChanged),
                FXMAPFUNC(SEL_COMMAND, 1, FoxBoxUiOverlay::onCommand)};
FXIMPLEMENT(FoxBoxUiOverlay, FXObject, overlayMap, ARRAYNUMBER(overlayMap))
FoxBoxUiOverlay::FoxBoxUiOverlay(FoxRenderHost& h, BoxUiSession& s) : host(&h), session(&s) {}
FoxBoxUiOverlay::~FoxBoxUiOverlay() = default;
void FoxBoxUiOverlay::invalidate() {
  for (auto& p : items) {
    auto& i = *p.second;
    if (i.field)
      i.field->disable();
    if (i.button)
      i.button->disable();
  }
}
void FoxBoxUiOverlay::restore(Item& i) {
  if (i.field) {
    i.field->setText(i.accepted.c_str());
    i.dirty = false;
    auto v = i.frame->snapshot.values.find(i.control.valueBinding);
    i.expectedRevision = v == i.frame->snapshot.values.end() ? "0" : v->second.revision;
  }
}
void FoxBoxUiOverlay::submit(Item& i) {
  if (!host->interactive() || !session->current(*i.frame))
    return;
  auto serial = std::to_string(++events);
  BoxUiIntent intent{
      i.frame->key, i.control.id, i.control.commandBinding, i.frame->snapshot.contextRevision,
      serial,       serial,       i.expectedRevision,       {}};
  if (i.field)
    intent.value = std::string(i.field->getText().text());
  auto result = session->dispatch(intent);
  if (result.status == "accepted")
    i.dirty = false;
  if (completed)
    completed(result);
}
long FoxBoxUiOverlay::onChanged(FXObject* sender, FXSelector, void*) {
  for (auto& p : items)
    if (p.second->field == sender) {
      p.second->dirty = true;
      return 1;
    }
  return 0;
}
long FoxBoxUiOverlay::onCommand(FXObject* sender, FXSelector, void*) {
  for (auto& p : items)
    if (p.second->button == sender) {
      submit(*p.second);
      return 1;
    }
  return 0;
}
void FoxBoxUiOverlay::reconcile() {
  if (!host->frame() || !host->interactive()) {
    invalidate();
    return;
  }
  const bool changed = published != host->frame();
  published = host->frame();
  if (changed)
    ++publication;
  std::map<std::string, bool> seen;
  for (auto& run : published->runs) {
    auto frame = std::dynamic_pointer_cast<const BoxUiFrame>(run.visual);
    if (!frame)
      continue;
    for (auto& c : frame->controls) {
      auto id = frame->key.blockId + "/" + c.id;
      seen[id] = true;
      auto compatible =
          std::to_string(frame->key.source.document) + "/" +
          std::to_string(frame->key.source.revision) + "/" + std::to_string(frame->key.epoch) +
          "/" + std::to_string(frame->key.bindingRevision) + "/" + frame->snapshot.contextRevision +
          "/" + c.commandBinding + "/" + c.valueBinding + "/" + std::to_string(unsigned(c.kind)) +
          "/" + std::to_string(c.version) + "/" + c.valueType;
      auto found = items.find(id);
      if (found != items.end() && found->second->compatibility != compatible) {
        items.erase(found);
      }
      auto& ptr = items[id];
      if (!ptr) {
        ptr = std::make_unique<Item>();
        auto& i = *ptr;
        i.compatibility = compatible;
        i.clip = new FXComposite(host, LAYOUT_FIX_X | LAYOUT_FIX_Y | LAYOUT_FIX_WIDTH |
                                           LAYOUT_FIX_HEIGHT);
        if (c.kind == BoxUiKind::Input) {
          i.field = new FoxBoxUiField(i.clip, this, 1);
          i.field->commit = [this, &i] { submit(i); };
          i.field->restore = [this, &i] { restore(i); };
        } else
          i.button = new FoxBoxUiButton(i.clip, c.accessibleName.c_str(), this, 1);
        if (host->id())
          i.clip->create();
      }
      auto& i = *ptr;
      i.frame = frame;
      i.control = c;
      i.bounds = run.bounds;
      auto v = frame->snapshot.values.find(c.valueBinding);
      i.accepted =
          v != frame->snapshot.values.end() && std::holds_alternative<std::string>(v->second.value)
              ? std::get<std::string>(v->second.value)
              : "";
      if (i.field && !i.dirty)
        restore(i);
      if (i.field)
        i.field->setTipText(
            (c.accessibleName + (i.dirty && v != frame->snapshot.values.end() &&
                                         i.expectedRevision != v->second.revision
                                     ? " — value changed; Escape restores accepted value"
                                     : ""))
                .c_str());
      if (i.button) {
        i.button->publication = publication;
        i.button->setTipText(c.accessibleName.c_str());
      }
      bool enabled = c.enabled && session->current(*frame);
      FXWindow* widget = i.field ? static_cast<FXWindow*>(i.field) : i.button;
      if (enabled)
        widget->enable();
      else
        widget->disable();
    }
  }
  for (auto it = items.begin(); it != items.end();)
    if (!seen[it->first])
      it = items.erase(it);
    else
      ++it;
  position();
}
void FoxBoxUiOverlay::position() {
  auto palette = ReadingPalette::from(host->readingColors());
  auto color = [](std::uint32_t c) { return FXRGB((c >> 16) & 255, (c >> 8) & 255, c & 255); };
  for (auto& p : items) {
    auto& i = *p.second;
    double sx = i.bounds.width / (i.frame->width / .75),
           sy = i.bounds.height / (i.frame->height / .75);
    auto a = host->documentToView(
        {i.bounds.x + i.control.rect.x * sx, i.bounds.y + i.control.rect.y * sy});
    auto b = host->documentToView({i.bounds.x + (i.control.rect.x + i.control.rect.width) * sx,
                                   i.bounds.y + (i.control.rect.y + i.control.rect.height) * sy});
    a.x += host->getXPosition();
    a.y += host->getYPosition();
    b.x += host->getXPosition();
    b.y += host->getYPosition();
    int left = int(std::floor(a.x)), top = int(std::floor(a.y)), right = int(std::ceil(b.x)),
        bottom = int(std::ceil(b.y));
    int x = std::max(0, left), y = std::max(0, top),
        w = std::min(host->getViewportWidth(), right) - x,
        h = std::min(host->getViewportHeight(), bottom) - y;
    if (w <= 0 || h <= 0) {
      i.clip->hide();
      continue;
    }
    i.clip->position(x, y, w, h);
    i.clip->show();
    FXWindow* widget = i.field ? static_cast<FXWindow*>(i.field) : i.button;
    widget->position(left - x, top - y, std::max(1, right - left), std::max(1, bottom - top));
    int points = std::clamp(int(std::round(12. * (b.y - a.y) / i.control.rect.height)), 4, 72);
    if (i.fontSize != points) {
      auto font = std::make_unique<FXFont>(host->getApp(), "DejaVu Sans", points);
      font->create();
      if (i.field)
        i.field->setFont(font.get());
      else
        i.button->setFont(font.get());
      i.font = std::move(font);
      i.fontSize = points;
    }
    widget->setBackColor(color(palette.background));
    if (i.field) {
      i.field->setTextColor(color(palette.text));
      i.field->setCursorColor(color(palette.text));
    } else {
      i.button->setTextColor(color(palette.text));
      i.button->setBaseColor(color(palette.surface));
    }
    widget->update();
  }
}
} // namespace xfmd
