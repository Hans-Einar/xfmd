---
id: FTR-007
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-017, UR-018, SR-001, SR-008, SR-016, SR-017, SR-019
uses: FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-009, FUNC-010, FUNC-014, FUNC-016, FUNC-017, FUNC-018
---

# Feature-007: Paged preview and faithful PDF publication

## 1. Purpose and scope

Provide page preview that predicts PDF output. Preview/publication share a feature
because their acceptance is correspondence; FTR-001 also gains a reading mode.

## 2. Requirements and acceptance

UR-017, UR-018, SR-001, SR-008, SR-016, SR-017, SR-019; [requirements](../../../xfmd_requirements.md).
AT-011, AT-018, AT-031, AT-032, AT-036, AT-037, AT-039. P055 extends view zoom without changing publication.

## 3. Contracts and ownership

User selects Wrap or A4. FUNC-004/017 lay out pages; FUNC-016 shares glyph data;
FUNC-005 transforms viewports; FUNC-018 exports. Snapshot, PaperSpec and FontSetId
freeze at export start; no new document session or implicit save.

## 4. Behavior, state and failures

Historical A4 offered page gaps and fit-width/100%; P055 adds shared manual scale
in Wrap/A4, explicit width/height fit and defaults to manual 100%. Zoom is display
state. Wrap reflows at logical viewport width; A4 page geometry stays fixed. Export
from either mode uses the same A4 profile. Live preview/sync continue; source anchors
survive format/zoom changes. Failed layout/export leaves the editor usable with feedback.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `View A4 / Wrap` | `PreviewCoordinator::setLayoutProfile` | `src/application/preview/PreviewCoordinator.cpp` | Profile → FrameKey | Stale frame noninteractive | Implemented |
| 2 | `MarkdownRenderer::layout` | `PageComposer::compose` | `src/renderer/PageComposer.cpp` | Flow/paper → pages | A4 constraints/anchors | Implemented |
| 3 | `File Export PDF` | `ExportCoordinator::start` | `src/application/export/ExportCoordinator.cpp` | Frozen buffer/profile → job | No dirty change | Implemented |
| 4 | `Page frame + viewport` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | FrameKey → transform | A4 zoom is not reflow | Implemented |
| 5 | `zoom command` | `DocumentZoom::setPercent` | `src/application/zoom/DocumentZoom.cpp` | Percentage → shared editor/preview scale | Paper/export unchanged | Implemented |

## 6. Reuse and dependencies

Reuse interpreter/renderer ports and source mapping. Export consumes document
services directly, not preview's private parser job. Shared FUNC-016/017 prevent
separate typography/pagination engines. P055 reuses workspace/host/editor ownership.

## 7. Verification

WYSIWYG means matching format, line/page breaks, glyph positions and contents for
the same profile/token/font set. Screen antialiasing need not pixel-match a PDF reader.
Test the entire document, not only the visible page. Historical acceptance:
[P11](../../../docs/evidence/P11.md), [P12](../../../docs/evidence/P12.md).
P055 acceptance follows its [plan](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md).

## 8. Status, risks and change impact

Revision 1.2 implemented P11/P12, following P9 proof. No Verified status without
independent PDF-reader and font/layout contract checks. Older evidence retains
its dated scope. [Design](../../../softwareDesign.md), [historical plan](../../../implementationPlan.md).

P055 local zoom acceptance and bounded limits: [evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md).
