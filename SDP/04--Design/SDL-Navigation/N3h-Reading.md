# N3h — Read large diagrams at their measured size

Session0005 T013 / PLAN-XFMD-0009 / KB020 and KB028. Selected under the owner's
authorization to resume diagram work, independent of the pending KB030 API.

## Observation and selected behavior

DiagramPlacement always reduces a wide scene to the continuous text width.
Document zoom reduces that logical width before layout, so increasing zoom can
leave the already fitted diagram at the same screen width. A4 additionally fits
the scene to page height. N3e records unreadable real-model overview captions.

Provide **View → Full-size diagrams (Wrap)**, off by default. In Wrap this draws
each prepared diagram at its measured logical size. Existing document zoom then
magnifies it normally, and the existing horizontal scrollbar reaches its full
width. Surrounding prose still wraps to the viewport. The user can turn the
option off to recover the complete fitted overview. This is window-local view
state, retained across documents and A4/Wrap switches, not a saved preference.
The menu is disabled in A4; A4 preview and PDF always fit diagrams to the page.

Increasing the global zoom limit is rejected: fitting during reflow cancels its
effect. Replacing the default fit behavior is rejected: the whole overview must
remain available without scrolling. A new diagram viewer duplicates existing
SVG painting, pan/zoom and source ownership. Producer partitioning and readable
multi-page publication may still be useful, but are separate work.

## Ownership and plumbing

FUNC010 adds one CommandRouter command and a checked View-menu item. Application
copies the current LayoutProfile, toggles `fullSizeDiagrams`, captures/restores
the source anchor, and calls FUNC007 PreviewCoordinator::setLayoutProfile.
The flag participates in LayoutProfile/FrameKey equality, preventing acceptance
of frames from the previous policy. No parse or graph layout is performed.

FUNC024 DiagramPlacement applies scale 1 only for Continuous plus this flag,
extends RenderFrame.contentWidth through the right gutter, and retains the same
DiagramScene pointer, aspect ratio and approximate source mapping. The existing
FoxRenderHost/ViewTransform own horizontal scrolling, DPI and zoom. Block and
ordinary text layout still use request.width. Paged layout ignores the flag.
No new parser, renderer fork change, ABI revision or producer interpretation.

## Verification and boundaries

Verify a real wide diagram through native menu activation, horizontal reach,
100/200% zoom and resize; keep token, dirty buffer, editor selection, undo and
scene identity. Verify returning to fit clears overflow and preserves prose.
Compare paged geometry/page count with the flag on/off, check FrameKey inequality,
and run existing diagram/PDF, document zoom and SDP overview regressions.
Capture real VP07 overview fit/full-size screenshots. A4/PDF large-overview
readability remains open; this increment must not be reported as all N3/N4 done.
