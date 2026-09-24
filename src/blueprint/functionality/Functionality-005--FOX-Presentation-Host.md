---
id: FUNC-005
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-040, UR-041, UR-035, UR-034, UR-030, UR-031, UR-011, UR-002, UR-005, UR-008, UR-028, SR-001, SR-008, SR-009, SR-010, SR-013, UR-017, SR-016, SR-019
uses: FUNC-021, FUNC-004, FUNC-015, FUNC-016
---

# Functionality-005: FOX presentation host

## 1. Purpose and scope

Connect an independent renderer to the FOX window. Own FOX resources, viewport,
font measurement, painting and input translation. No Markdown semantics, file I/O
or history belong here.

## 2. Requirements and acceptance

UR-040, UR-041, UR-035, UR-034, UR-030, UR-031, UR-011, UR-002, UR-005, UR-008, UR-017, UR-028 and SR-001, SR-008, SR-009, SR-010, SR-013, SR-016, SR-019;
see the [requirements](../../../xfmd_requirements.md). Chapter 7 identifies evidence.

## 3. Contracts and ownership

FoxRenderHost is an FXScrollArea adapter. present/expect/invalidate control the
frame and interactivity. SharedTextMetrics shapes text; DisplayListPainter draws
the same glyph data through FontCatalog. The host uses IRenderer only for hit-testing.

P11 retains the FOX host, enabled input and viewport notifications. Wheel policy
belongs to FUNC-015; font measurement/glyph drawing belongs to FUNC-016. ViewTransform
owns points/zoom/DPI and page gaps. Frame acceptance uses layout-profile validation
instead of the original frame-width/viewport-width equality.

P053: linkActivated carries `(target, systemDefault)`. Main and navigator consumers
receive Ctrl captured on press. linkHovered remains a target-only callback; neither
callback introduces renderer or interpreter dependencies on FOX input.

## 4. Behavior, state and failures

Accept only the matching document token and layout width/profile. A new document
removes the old frame; pending edits disable interaction. Painting clips and visits
visible runs; installed fonts may provide fallback glyphs. The host has no Markdown
rules and must be enabled for native mouse and keyboard events. It draws the Unicode
link marker ↗ through Pango/Cairo; the renderer chooses marker type and placement.

FoxWheelScrollBar is shared by both axes. It retains fractional deltas, owns a
retargetable timer and emits changed/command notifications. Constructors replace
bars before create(); parent widgets own the adapters.

Balance press/release and release the host's grab before calling consumers. Only
a left-click on the same target without dragging or a button chord activates.
Other buttons retain focus but do not activate links. A stale frame cancels activation.
Captured Ctrl changes the consumer route; it does not cause another activation.

P055 implemented zoom behavior follows the [phase plan](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md).
Shared manual scale applies in Wrap/A4; explicit A4 width/height fit uses visible
viewports. Application owns state; host transforms and editor fonts consume it.
Native zoom acceptance is recorded in [P055 evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md); previous evidence retains its dated scope.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Frame → presentation | Reject wrong token/layout width | Implemented |
| 2 | `FOX paint` | `FoxRenderHost::onPaint` | `src/application/adapters/FoxRenderHost.cpp` | Display list → native drawing | Clip; no parser calls | Implemented |
| 3 | `InlineLayout::layout` | `SharedTextMetrics::measure` | `src/application/adapters/SharedTextMetrics.cpp` | Text/font → measurements | Fallback per segment | Implemented |
| 4 | `DisplayListPainter::text` | `FontCatalog::font` | `src/application/adapters/FontCatalog.cpp` | Font identity → native font | Reject changed font identity | Implemented |
| 5 | `FOX pointer` | `FoxRenderHost::onPointer` | `src/application/adapters/FoxPreviewInput.cpp` | Point → hit-test/callback | Current frame only; captured Ctrl passed once | Implemented |
| 6 | `ScrollCoordinator setPreview callback` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Y → clamped viewport | Suppress programmatic echo | Implemented |
| 7 | `FoxRenderHost constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → precise wheel adapter | Parent ownership; before create | Implemented |
| 8 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/remainder → target/timer | Clamp; retain fractional remainder | Implemented |
| 9 | `FoxRenderHost::present` | `ViewTransform::configure` | `src/application/adapters/ViewTransform.cpp` | PageLayout/viewport → transform | Paged zoom is not reflow | Implemented |
| 10 | `FoxRenderHost paint` | `DisplayListPainter::paint` | `src/application/adapters/DisplayListPainter.cpp` | Frame/target → pixels | Shared glyph basis | Implemented |
| 11 | `FoxRenderHost::onPaint` | `FoxCairoCanvas::present` | `src/application/adapters/FoxCairoCanvas.cpp` | Cairo buffer → FOX pixmap | Native lifetime; no Cairo Xlib device | Implemented |
| 12 | `FOX press` | `FoxRenderHost::onButtonPress` | `src/application/adapters/FoxPreviewInput.cpp` | Button/frame/link/Ctrl → click state/grab | Button chord cancels activation | Implemented |
| 13 | `FOX grab loss` | `FoxRenderHost::onUngrabbed` | `src/application/adapters/FoxPreviewInput.cpp` | Grab loss → clear click | No link callback | Implemented |
| 14 | `Application::changeReadingColors / applyAppearance` | `FoxRenderHost::setReadingColors` | `src/application/adapters/FoxRenderHost.cpp` | Reading palette → repaint/profile | No document mutation | Implemented |
| 15 | `FoxRenderHost hover callback` | `Application::showLinkTarget` | `src/application/ApplicationAppearance.cpp` | Link → status text | No activation; invalid paths shown as targets | Implemented |
| 16 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | Diagram paths/text → pixels | Block errors/stale data follow Mermaid design | Implemented |

| 55 | commands / normalized input | `DocumentZoom::step` | `src/application/zoom/DocumentZoom.cpp` | signed steps → shared bounded scale | leave fit; cancel scroll; retain document | Implemented |
| 56 | viewport/profile changes | `DocumentZoom::refresh` | `src/application/zoom/DocumentZoom.cpp` | geometry → editor/host scale and controls | guard reentrancy; preserve anchor | Implemented |

## 6. Reuse and dependencies

[FUNC-004](Functionality-004--Render-Layout.md), FUNC-015, FUNC-016 and FUNC-021.
Presentation, preview, navigation and synchronization consume this host.
Application wires callbacks to coordinators; the host knows no feature workflow.

Sprint 004 consumer, reconstructed 2026-09-24: [FUNC-031](Functionality-031--Generated-Document-Navigation.md)
creates another host for its navigator. Registered SDL actions and ordinary links
are dispatched by its application callback. P053 extends that callback with Ctrl routing.

## 7. Verification

AT-025, AT-002, AT-005, AT-008, AT-011, AT-018, AT-019, AT-020, AT-023, plus the P11 extension's AT-031, AT-036, AT-039 and
P053 AT-048. PresentationTest checks actual font metrics, frame tokens, width and
stale rejection. The P3 screenshot was visually checked with CJK fallback.
Evidence: [P3](../../../docs/evidence/P3.md), [native links](../../../docs/evidence/document-links.md),
[wheel gestures/bounds](../../../docs/evidence/wheel-scrolling.md).
P7 evaluates aggregate coverage; Implemented is not automatically Verified.
Older evidence does not automatically verify newer requirements.

P053: PointerTest/PreviewSelectionGuiTest retain input/selection regressions and
FileRoutingGuiTest exercises real Ctrl/ordinary clicks in both hosts. Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Implemented in P3 with [P11 evidence](../../../docs/evidence/P11.md) for the 1.1
extension. Keep contracts, calls, consumers and tests together; historical evidence
retains its original baseline.

P21: UR-030, UR-031, AT-050, AT-051. ReadingColors are reading preferences and
PreviewColorControls uses UiRow/UiContext. Only the screen host supplies a palette
to DisplayListPainter; PDF retains defaults. DecorationRole survives PageComposer
without FOX types in renderer/contracts. ReadingLight/ReadingDark profiles persist
additively through preferences. Live changes repaint, release commits, and save
failure rolls back. Evidence: [P21](../../../docs/evidence/P21.md).

P22: compact shared toolbar, revised UR-032, UR-033, UR-034; AT-054, CompactWorkspaceTest
and regressions. Evidence: [P22](../../../docs/evidence/P22.md).
Hover/restored status handles `&` literally rather than as a FOX mnemonic.

P23: FoxPreviewInput owns selection/clipboard input. PreviewSelection stores logical
text positions; repaint preserves selection, new document token clears it. AT-055:
RichPreviewTest and PreviewSelectionGuiTest.

P25's proposal for UR-040, UR-041 and AT-060, AT-061 reused palette, transforms and selection
for diagrams without another host. See the [Mermaid design](../../../docs/design/mermaid-integration.md).
That dated proposal was not itself implementation evidence; the current implemented
DiagramPainter row above reflects the subsequently reconstructed call map.
