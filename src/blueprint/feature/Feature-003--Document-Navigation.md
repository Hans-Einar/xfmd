---
id: FTR-003
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-005, UR-009, UR-024, UR-028, SR-002, SR-005, SR-008, SR-012, SR-013, SR-019
uses: FUNC-001, FUNC-002, FUNC-005, FUNC-008, FUNC-009, FUNC-010, FUNC-014
---

# Feature-003: Local document navigation with history

## 1. Purpose and scope

Follow local links and return to the correct file and reading position. Transactional
history combines link handling and document sessions into the user workflow.

## 2. Requirements and acceptance

UR-001, UR-005, UR-009, UR-024, UR-028 and SR-002, SR-005, SR-008, SR-012, SR-013, SR-019. Normative definitions and
acceptance are in the [requirements](../../../xfmd_requirements.md).

## 3. Contracts and ownership

Application classifies user targets and NavigationCoordinator owns internal
navigation. Reuse LocalFileStore, DocumentCoordinator, HistoryStore, LinkResolver
and ScrollCoordinator. Renderer hit-testing reports a link; application decides
which consumer opens it. History stores source-byte anchors, including across
page profiles, zoom and fullscreen, not screen-pixel positions (P11 extension).

## 4. Behavior, state and failures

Enabled FOX hosts receive native press/release. Resolve a relative link from its
originating document directory, not the process working directory. Internal visits
commit only after successful opening and retain source anchors. Back/forward wait
for a matching frame before restoring. Broken links and dirty-cancel preserve
buffer and cursor. Main-document query/fragment navigation remains unsupported;
the generated navigator has its own supported heading behavior.

P053, 2026-09-24, supersedes suffix-only routing: valid plain text opens in the
editor, local HTML and HTTP(S) use the configured browser, binary targets use OS
association. Captured Ctrl uses OS defaults without changing document/history.
Other URI schemes remain rejected. Registered SDL actions and normal navigator
Markdown remain owned by FUNC-031; their links use the originating pane's path.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FoxRenderHost::onPointer` | `IRenderer::hitTest` | `src/contracts/IRenderer.h` | Frame/point → link | Only current interactive frame | Implemented |
| 2 | `host linkActivated callback` | `Application::followLink` | `src/application/ApplicationOpening.cpp` | Origin/target/Ctrl → routing | Source-relative resolution; errors reported | Implemented |
| 3 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → document commit | Dirty/load checks before commit | Implemented |
| 4 | `DocumentCoordinator opened callback` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Successful document → history | Never commit a failed/canceled visit | Implemented |
| 5 | `NavigationCoordinator::commitVisit` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Anchor → pending restoration | Require matching frame token | Implemented |
| 6 | `history restoration` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | SourceAnchor → VisualLocation | Wait for matching FrameKey | Implemented |
| 7 | `Application::followLink` | `Application::openTarget` | `src/application/ApplicationOpening.cpp` | Resolved file/Ctrl → consumer | External opening preserves history | Implemented |

## 6. Reuse and dependencies

[FUNC-001](../functionality/Functionality-001--Document-Session.md),
[FUNC-002](../functionality/Functionality-002--Local-File-Storage.md),
[FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md),
[FUNC-008](../functionality/Functionality-008--Navigation-History.md),
[FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md),
[FUNC-010](../functionality/Functionality-010--Workspace-Controls.md) and
[FUNC-014](../functionality/Functionality-014--Application-Preferences.md).
History and scroll synchronization share source anchors. Sidebar/CLI reuse the
document transaction. No feature-local parser, file store or copied widget.

## 7. Verification

AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-022, AT-023, AT-039, AT-044, AT-048. Historical NavigationTest and
NavigationGuiTest cover dirty cancellation and back/forward after renderer hits.
Source-anchor/resize checks were recorded in P5. Evidence: [P6](../../../docs/evidence/P6.md),
[native links](../../../docs/evidence/document-links.md), [P11](../../../docs/evidence/P11.md).
The P11 extension listed AT-039 as required coverage; older evidence does not prove
later amendments. P7 assesses aggregate coverage; Implemented is not automatically Verified.

P053 native matrix covers main preview, navigator and tree routes, captured Ctrl,
relative paths, asynchronous launch failures and preserved dirty/undo/history.
Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Implemented in P6 and extended in P11/P15/P16/P053. Change contracts, call maps,
consumers and tests together; historical evidence keeps its original scope.

P15: NavigationCoordinator::openAt accepts an explicit SourceAnchor. Same-file
navigation jumps within the active buffer without a read or dirty prompt. A different
file uses DocumentCoordinator and commits history before restoring the pending
anchor. Referenced headings are rediscovered after model acceptance; a removed
heading falls back to the start.

P16: HTTP(S) from preview and references uses the shared argv browser adapter and
FUNC-014 preference. LocalFileStore and NavigationCoordinator do not load network
content; browser opening does not mutate the document/history. P053 extends this
to local HTML and Ctrl routing without changing those ownership boundaries.
