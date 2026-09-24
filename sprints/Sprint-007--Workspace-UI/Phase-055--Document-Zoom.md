# Phase 055 — Shared document zoom

Branch `sprint/007/phase/055-document-zoom`, base `041e6af`. Autonomous phase:
requirements/design first, implementation, then phase build and native acceptance.
One M1 delivery for KB-XFMD-010; corrective builds follow observed failures.

## Selected behavior

DocumentZoom under application/zoom owns one session-local scale/mode for the main
editor and preview. Default is manual 100%; opening documents and changing view
mode retains the setting, restarting uses 100%. UI fonts and navigator state are
independent. Manual range is 25–300%; each key/wheel notch changes 10 percentage
points, with fractional wheel deltas retained by the numeric scale. Positive wheel
zooms in. A manual action leaves fit mode and starts from its computed percentage,
clamped to the manual range. Reject nonfinite input.

Ctrl+plus (including Shift/equal and keypad Add) and Ctrl+minus/keypad Subtract
share commands with Ctrl+wheel. Consume document-surface and document-scrollbar
Ctrl+wheel exactly once and cancel pending scrolling; trees, recent lists and the
preferences sample retain their existing Ctrl=page policy. Normal/Alt/Shift wheel
behavior remains under FUNC-015. Keyboard gestures must not insert or delete text.

Toolbar dropdown shows the actual rounded percentage and fit mode in its tooltip.
Its entries are 25%, 50%, 100%, 200%, 300%, separator, Fit page width, Fit page height.
A View → Zoom submenu contains those presets, Zoom in and Zoom out with Ctrl++ and
Ctrl+- labels, and no fit entries. Fit actions are disabled in Wrap, which has no
physical page height. Manual zoom works in both modes. Switching from an A4 fit to
Wrap retains the computed scale as bounded manual zoom. A4 initially retains the
current manual scale; fit is explicit, superseding the older default fit-width.

A4 fit uses one page and 16 px surrounding padding. In split view use the smaller
available visible document dimension so both surfaces share one factor; editor-only
uses the editor viewport, preview-only uses the preview viewport. Width/height fit
updates after resizing, splitter movement, Sidebar toggling and view changes.
Automatic fit is bounded to 5–800%; manual controls retain 25–300%. 100% is the
configured screen-DPI point transform. Editor font rasterization may round glyph
sizes/advances; it uses the same requested scale without inventing a second fit.

Wrap preview reflows at viewport-width / (DPI × scale), then draws with that scale.
A4 uses a display transform only: paper geometry, page breaks and PDF are unchanged.
EditorPresentation scales its base font in both modes and keeps A4 source columns.
Preserve caret, selection, dirty/undo, stored bytes and source reading anchor.
Preview logical selection must survive same-token reflow. Stale frames remain
noninteractive; repeated zoom/resize must converge without layout loops.

## Ownership and plumbing

Reuse FUNC-010 (workspace commands), FUNC-005 (host/transform), FUNC-011 (editor),
FUNC-015 (wheel adapter), FUNC-007/009 (layout/anchors), and FTR-007 (page/PDF).
New files: application/zoom/DocumentZoom.{h,cpp} coordinates existing owners;
ZoomMode.h holds the application mode enum; ui/DocumentZoomInput.h normalizes FOX
key gestures; ApplicationZoom.cpp wires the existing composition root. No new
renderer/interpreter dependencies, paper properties or preference storage.

Implemented calls: commands/normalized input → DocumentZoom::setPercent/step/setMode;
viewport/layout changes → DocumentZoom::refresh; refresh → EditorWidget::setViewProfile,
FoxRenderHost::setViewScale and PreviewControls::sync; Wrap host resize → existing
PreviewCoordinator::relayout. Existing source-anchor restoration owns reading position.

## Acceptance / M1

M1 implemented and locally accepted. Native editor-only/preview-only/split × Wrap/A4 checks for
wheel and keyboard, keypad/Shift combinations, all presets and fit actions/menu
structure, boundaries, fractional deltas, actual percentage after resize, preserved
caret/selection/dirty/undo/source anchor and no duplicate page scroll. Verify Wrap
reflow, unchanged A4 frames/PDF geometry, sidebar wheel regressions, rapid changes
and native screenshots. Run all relevant CTests, design checks and a separate
sanitizer check for added coordinator/callback and menu ownership. Record actual
source/binary identity; close #010 and the overall goal only after the complete
six-card acceptance audit.

## Native findings during implementation

The 96/144-DPI matrix exposed unnecessary horizontal overflow from the renderer's
40-point minimum text column and fixed gutters at enlarged Wrap scale. FUNC-004
now specifies positive remaining width plus shrinking Continuous gutters below
160 logical points. InlineLayout takes explicit trailing padding. A4 keeps its
fixed gutters/pagination; PDF comparison passes after this correction.
Fit calculations use laid-out viewport_w/viewport_h after scrollbars, not FOX's
maximum viewport accessors. Editor paper dimensions use the same configured DPI.

## M1 outcome

Delivered on the phase branch with one milestone commit. Built identity:
`0.1 sprint/007/phase/055-document-zoom:194+dirty`, based on `041e6af` plus
identified P055 sources. [Evidence](evidence/P055.md) distinguishes the full
73-test run from final visual correction/acceptance and separate sanitizers;
the manifest records source/binary hashes. The [six-card audit](evidence/Completion.md)
closes selected scope; #010 moves to completed with an append-only ledger event.
No merge, installation or global Verified promotion is implied.

## M2 — Name the UI delivery 0.3

Owner decision on 2026-09-24: the previous navigation milestone is named 0.2 and
this UI delivery is 0.3. Update VERSION and the English version policy together;
retain the actual 0.1 identities in historical acceptance records. This is a
version-only follow-up on the same phase branch, with its own milestone commit.
BuildVersionTest checks the generated version against metadata; the installation
receipt records the final clean commit and installed binary. Application behavior
is unchanged, so the P053–P055 functional evidence remains applicable.
