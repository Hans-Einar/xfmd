# Navigation modeling pilot — current behavior contract

Parent: [PLAN-XFMD-0003](Plan.md).
Architecture: [XM-ADR-001–006](../../03--Architecture/XFMD-Modeling/Decisions.md).
Source baseline: XFMD `41fa494`; read against the current tracked implementation.

This describes existing behavior for UC-001/002/003/004/011, UR-001/003/004/005/
009/012/013/014/021/028/038/044/045 and SR-002/005/006/007/008/009/027/028.
The original requirement IDs remain normative. It does not define a new feature.

## State and responsibility

| State | Owner | Model boundary |
| --- | --- | --- |
| Main text, revision, saved baseline and dirty | DocumentSession | FUNC-001; dirty compares current text to saved baseline |
| Interactive admission/read/save | DocumentCoordinator and LocalFileStore | FUNC-001/002; state changes only at the documented transaction steps |
| Visits and reading anchors | NavigationCoordinator, HistoryStore, ScrollCoordinator | FUNC-008/009/015; a successful open commits the visit |
| Root, filter, histories, favorites | WorkspacePanel with WorkPathHistory, DirectoryScanner, FavoritesPanel | FUNC-010/012/013; favorites are not recent history |
| Current semantic model/frame | PreviewCoordinator, ParserWorker, renderer and host | FUNC-003/004/005/006/007; document/revision/generation govern publication |
| Generated selection, child process, direct bundle and leases | DocumentViews | FUNC-031; captured window, explicit pane and bounded pending work |
| Navigator document/preview | NavigationPanel's separate session/coordinators | Reuses ordinary ports; never aliases the main document buffer |

## Scenarios and exceptional outcomes

| ID | Trigger and actual path | Result / invariant | Evidence target |
| --- | --- | --- | --- |
| NAV-01 | Favorite file → ApplicationWorkspace callback → Application::openTarget → open → NavigationCoordinator → DocumentCoordinator::requestOpen | Uses classification and ordinary unsaved admission; a favorite does not implicitly change work root | FavoritesGuiTest, FileRoutingGuiTest, NavigationGuiTest |
| NAV-02 | Favorite folder → WorkspacePanel::setWorkPath | Changes root/history/tree; document, dirty and document history remain separate | FavoritesGuiTest, WorkPathGuiTest |
| NAV-03 | Right-click tree/history/favorite → copied path → PathContextMenu | Menu selection acts on clicked target; dismiss/empty/separator does nothing; keyboard grab ends before action | FavoritesGuiTest and TreeActionsTest |
| NAV-04 | User open while dirty → resolveUnsaved | Cancel leaves buffer/path/history; Discard permits candidate read; Save must succeed before read is attempted | Document transaction tests; NavigationGuiTest |
| NAV-05 | Save succeeds during admission, then candidate read fails | Previous document remains, but successful save has legitimately cleared dirty; do not assert that all state is identical to before Save | New explicit evidence review needed; GAP-XFMD-EVID-002 |
| NAV-06 | File/Open chooser selects internal file | Proposed root is validated first; successful open then sets parent root. Folder-only selection changes root; external handoff leaves it | OpenPathDialogGuiTest, FileRoutingGuiTest |
| NAV-07 | Edit → PreviewCoordinator::schedule | Debounce and revision/generation checks preserve newest document; unavailable/stale mapping cannot drive scrolling | Pipeline and Scrolling tests |
| NAV-08 | Registered sdl-view URI → DocumentViews::follow | Validate project/tool/URI first; cancel preceding accepted child, increment sequence and start bounded external generation | DocumentViewsGuiTest |
| NAV-09 | Generated main delivery → DocumentViews::open | Dirty main is rejected with status and no modal dialog. This guard is at delivery; follow() does not pre-reject every dirty selection | DocumentViewsGuiTest, DocumentEndpointTest |
| NAV-10 | Invalid URI, child failure or timeout | Reject/report without replacing the active document. Invalid URI is checked before canceling an already active child | DocumentViewsGuiTest; explicit invalid-during-pending coverage to review |
| NAV-11 | Successful panel replacement | Successful-open callback queues prior lease release; new leased delivery registers only after open success | DocumentLeaseGuiTest and P050 evidence |
| NAV-12 | Broker release send fails / queue reaches bound / shutdown | Retry on later polling; admission blocks at queue bound. Successful send is not durable acknowledgment; shutdown does not persist retries | AT-072 remaining evidence; GAP-XFMD-EVID-001 |

Evidence targets are existing test locations or obligations to inspect, not fresh
pass claims. Source inspection establishes the paths above. No UI is exercised by
an SDL structural validator or an SDUI SVG export.

## Model and UI states

The current XfmdDesktop.design records responsibilities and NAV-01–12 as declared
Channel scenarios; see [Model-Semantics](Model-Semantics.md) for exact mappings,
additional workflows and source boundaries. These are ordered examples, not
executable state transitions or complete C++ contract proofs. The prior
Navigation.design pilot is retained only for historical reproduction.
GAP-XFMD-SDL-003 remains open for predicates, binding and full behavior evidence.

Historical navigation SDUI entry frames represent three states of the same FOX shell:

- Files plus clean main document: persistent favorites, a navigator and preview.
- Files plus dirty main document after addressed delivery rejection: unchanged
  content, visible dirty identity and actual rejection status.
- Index plus clean main document: chapter/reference representations; Files-only
  Markdown/Up/Open actions are absent and Refresh remains.

Textual list/tree/editor representations and text labels in place of icons are
explicit approximation records, not replacements for XFMD's widgets. No Add/Remove
buttons are introduced. Context menus remain documented in NAV-03, not simulated
by a permanent toolbar action.
