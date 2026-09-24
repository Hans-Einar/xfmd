---
id: FUNC-004
kind: Functionality
audience: System
role: Service
owner: renderer
status: Implemented
scope: FirstRelease
requirements: UR-040, UR-041, UR-035, UR-036, UR-037, UR-005, UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013, UR-017, UR-018, SR-016, SR-019
uses: FUNC-024, FUNC-017
---

# Functionality-004: Layout and visual document model

## 1. Purpose and scope

Compute presentation from semantic models without FOX. Own semantic FontSpec choices,
block/inline layout, hit regions and mapping; no parsing, opening or document state.

## 2. Requirements and acceptance

Requirements are listed in metadata and defined in the [requirements](../../../xfmd_requirements.md).
Baseline acceptance: AT-005, AT-002, AT-008, AT-011, AT-013, AT-015, AT-018, AT-019,
AT-021, AT-023. P11 added AT-031, AT-032, AT-036, AT-039; P23 added AT-055, AT-056, AT-057.

## 3. Contracts and ownership

MarkdownRenderer implements IRenderer::layout/hitTest; BlockLayout/InlineLayout own
placement and HitTester owns link hits. DrawRun contains FontSpec/document coordinates,
not FOX resources. P11's revision 1.1 introduced measured FlowLayout visual lines,
point units and PageComposer; shaping is injected through pure ports. Renderer owns
placement, never native font resources.

## 4. Behavior, state and failures

Ordinary text wraps at words/shaper clusters; code preserves whitespace and can
scroll horizontally in Wrap. Heading fonts, nested lists/quotes, inline code and
inert HTML remain supported. Frames carry run and block ranges for hidden syntax.
LinkMarker produces #, /# or font-drawn ↗; linkId distinguishes separate links from
styled fragments. Marker/text share targets; marker anchors are empty/approximate.
TableLayout owns columns, cell InlineLayout, backgrounds/borders/alignment and whole
rows at page boundaries. Rows/cells retain anchors; no FXTable/HTML, one frame for
screen and PDF. P23 logical UTF-8 intervals address RenderFrame.readingText; embedded
visuals preserve aspect/baseline/height bounds before pagination, retaining anchors/links.

P055 narrow Wrap correction: use the actual positive width remaining after gutters,
not a forced 40-point text column when zoom reduces logical viewport width.
Continuous gutters shrink below 160 logical points (15% per side, capped at 24);
InlineLayout receives the corresponding trailing extent padding, capped at the
existing 20 points. A4 retains its fixed gutters and padding. Existing A4 widths and code/table behavior are unchanged. A single glyph or fixed indentation
can still exceed an extremely narrow viewport; never clip source or alter font size
behind the selected zoom. The native zoom test exposed the prior unnecessary overflow.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::relayout` | `MarkdownRenderer::layout` | `src/renderer/MarkdownRenderer.cpp` | Model/request/metrics → frame | Validate width | Implemented |
| 2 | `MarkdownRenderer::layout` | `BlockLayout::layout` | `src/renderer/BlockLayout.cpp` | Blocks → vertical geometry | Resource bounds | Implemented |
| 3 | `BlockLayout::layout` | `InlineLayout::layout` | `src/renderer/InlineLayout.cpp` | Runs → lines/anchors | Never split inside UTF-8 characters | Implemented |
| 4 | `InlineLayout::layout` | `ITextMetrics::measure` | `src/contracts/ITextMetrics.h` | Text/font → extent | Pure port | Implemented |
| 5 | `MarkdownRenderer::hitTest` | `HitTester::hitTest` | `src/renderer/HitTester.cpp` | Frame/point → HitResult | No navigation side effect | Implemented |
| 6 | `InlineLayout::layout` | `LinkMarker::make` | `src/renderer/LinkMarker.cpp` | Link/font → marker run | No I/O; synthetic source range | Implemented |
| 7 | `MarkdownRenderer::layout` | `PageComposer::compose` | `src/renderer/PageComposer.cpp` | FlowLayout + PaperSpec → PageLayout | Bound oversized blocks | Implemented |

| 8 | `BlockLayout::layout` | `TableLayout::layout` | `src/renderer/TableLayout.cpp` | Table/available width → cells, rows, decorations | Narrow A4 or oversized row raises Error | Implemented |


| 99 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | Implemented diagram extension | Block errors/stale data follow Mermaid design | Implemented |

## 6. Reuse and dependencies

Shared data follow the architecture. Preview consumes layout, FOX host consumes
hitTest, mapping reads RenderFrame; renderer is not an application controller.
FUNC-017 and FUNC-024 retain page/diagram ownership. New primitives need contract review.

## 7. Verification

RendererTest uses deterministic metrics for wrapping/styles/UTF-8/decorations/anchors/
hits; PresentationTest uses native metrics. Historical evidence: [P3](../../../docs/evidence/P3.md),
[native links](../../../docs/evidence/document-links.md), [P11](../../../docs/evidence/P11.md).
P7 assessed aggregate coverage. P14 TableTest/TablePreviewTest/PdfFidelityTest cover
tables; [P14](../../../docs/evidence/P14.md). P23 RichPreviewTest/PreviewSelectionGuiTest
cover logical selection/embedded visuals. P055 adds narrow flow-width regression and
native zoom checks under its [plan](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md).

## 8. Status, risks and change impact

Implemented in P3, revision 1.1 P11. Update contracts/calls/consumers/tests together;
old evidence retains dated scope. P21 DecorationRole (Border/Surface/Alternate/Background)
coexists with print color; BlockLayout/TableLayout assign roles so screen palettes
need no magic RGB matching. Geometry/font/FrameKey remain unchanged, no native types
or preferences enter renderer. ReadingColorsTest checks roles and default print colors.
P25 originally proposed DiagramPlacement for UR-040/041 and AT-060, AT-061; the current
implemented row reflects later delivery. Expensive diagram layout stays off the GUI
renderer path; see [design](../../../docs/design/mermaid-integration.md). That dated
proposal alone was not implementation evidence. P055 does not change those contracts.

P055 local zoom acceptance and bounded limits: [evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md).
