#include "ScrollCoordinator.h"
namespace xfmd {
void ScrollCoordinator::invalidate(DocumentToken token) {
  if (token.document != expected.document) { last = {}; pending = SourceAnchor{}; lastSequence = 0; }
  else if (!pending) pending = last;
  expected = token; valid = false;
}
void ScrollCoordinator::setFrame(LayoutResult value) {
  if (!value || value->token != expected) return;
  frame = std::move(value); valid = true;
  if (pending) { auto anchor = *pending; pending.reset(); restoreAnchor(anchor); }
}
void ScrollCoordinator::restoreAnchor(SourceAnchor anchor) {
  last = anchor;
  if (!valid || !frame) { pending = anchor; return; }
  auto target = AnchorMapper::map(anchor, *frame);
  if (target.quality == MappingQuality::Unavailable) return;
  synchronizing = true;
  if (setPreview) setPreview(target.y);
  if (split && setEditor) setEditor(anchor);
  synchronizing = false;
}
void ScrollCoordinator::onViewportChanged(ViewOrigin origin, std::size_t position, DocumentToken token,
                                          std::uint64_t sequence, bool programmatic) {
  if (synchronizing || programmatic || !valid || !frame || token != expected) return;
  if (sequence && sequence <= lastSequence) return;
  if (sequence) lastSequence = sequence;
  last = origin == ViewOrigin::Editor ? SourceAnchor{position} : AnchorMapper::anchorAt(int(position), *frame);
  if (!split || last.quality == MappingQuality::Unavailable) return;
  synchronizing = true;
  if (origin == ViewOrigin::Editor) {
    auto target = AnchorMapper::map(last, *frame);
    if (target.quality != MappingQuality::Unavailable && setPreview) setPreview(target.y);
  } else if (setEditor) setEditor(last);
  synchronizing = false;
}
}
