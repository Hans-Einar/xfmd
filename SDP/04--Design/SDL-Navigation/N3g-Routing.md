# N3g — Preserve caption identity during rerouting

Session0005 T010 / KB020/028 / PLAN-XFMD-0009 / SR-024 / FUNC-024.

The exact native ChapterNavigation fixture fails at edge7: its padded caption is
38 units from its owner and about15 units from edge10. Attachment-zone exclusion
alone does not express the existing nearest-owner validation rule. Increasing
candidate sampling exceeded the unchanged diagnostic budget and is not selected.

Select a bounded geometry correction in the renderer fork: retain the attachment
obstacle and add an exclusion area around each caption whose radius follows the
current owner distance, the existing3-unit ambiguity tolerance and Libavoid's
clearance. Use the same Manhattan metric as validation (a rectangle with chamfered
corners). This also preserves owner clearance near diagonal corners; a square
halo would unnecessarily obstruct those routes. Separate overlapping obstacles
express the union without introducing application-side geometry or port search.

No change to graph semantics, four label orders, three routing transactions, one
expanded-placement retry, deadlines or strict final validation. Later owner-route
movement can invalidate an earlier exclusion radius; final validation remains
mandatory. Regression acceptance includes the exact native fixture, geometry
unit checks, previous VP07/DocumentReferences/backend tests and a ten-detail
native sweep plus inspected ChapterNavigation preview/PDF.

## Verification disposition

Adopted through the exact local renderer commit and checksum-pinned archive in
N3g-001. The ten-detail sweep and native/PDF verification pass. This corrects the
reproduced ambiguity; it does not promise universal routing success or establish
independent/owner acceptance of the entire diagram Session.
