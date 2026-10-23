# Contextual SDP toolbar groups and initial actions

| Field | Value |
| --- | --- |
| id | KB-XFMD-029 |
| project | XFMD |
| type | Change |
| CardState | in-progress |
| created | 2026-10-06T21:49:41.481757+00:00 |
| source | Owner Session 0003 T001, 2026-10-06 |
| next_review | Select the supported KanBan Add/Move service contract; SDUI portion delivered |

## Owner interaction contract

When SDP mode is active, show all groups and their configured actions continuously:
**SDP:**, **KanBan:**, **SDUI:** and **Sessions:**. Changing the selected sub-tab or
view must not hide or reorder groups/tools. Unavailable actions have grey disabled
icons and cannot be clicked; provide an explanation accessible by tooltip/status.
Outside SDP mode these SDP-specific controls are hidden. Define SDP mode using the
existing SDP workspace/tab context before implementation, not a second project flag.

Initial actions:

| Group | Action | Available context |
| --- | --- | --- |
| SDP | Group reserved for future tools | Do not invent an action solely to fill it |
| KanBan | Add | A recognized writable board with a supported create operation |
| KanBan | Move card | A currently identified card and valid destination transition |
| SDUI | Launch Fyne interpreter | A selected entry whose host/profile/binding preflight permits launch |
| Sessions | Group reserved for future tools | Do not invent an action solely to fill it |

All defined tools stay visible when unavailable. Use text/icon labels that remain
understandable without color; toolbar and any menu representation share one action
state. Current document/selection, not merely tab name, determines availability.
For example, a generated SDUI page can identify its entry even after tree refresh.

## Design and acceptance before implementation

Choose the minimal Add form (type/title/initial state) and Move destination control
from the actual board contract. Delegate through supported management operations;
a raw file move cannot maintain CardState, index, incoming links and append-only
history. Preflight permissions, allowed transitions and concurrent revisions;
refresh discovery after success, preserve context on failure. Never silently merge,
change card identity or act on stale selection. The exact CLI/MCP action contract
needs inspection; current discovery/select support does not prove mutation support.
Inspected Toolkit/scripts/cli/kanban.sh exposes read-only status/state listing,
so that CLI alone cannot implement Add/Move. Inspect the supported management
service before selecting a producer mutation contract; do not edit history ad hoc.

Launch uses #027 and the upstream readiness contract. No implicit execution when
selecting a page, icon-only guessed shell command, or assumed Fyne embedding.
The four owner-named groups are selected; an SDL group can be proposed later but
has not been silently added here.

Acceptance: every group visible throughout SDP mode, all defined icons disabled
appropriately across tab/document/selection changes, keyboard activation obeys
same state, real create/move ledger validation, stale/readonly/missing-tool failures,
and launch readiness/error handling. Keep this independent of diagram generation.

[Session 0003](../../Sessions/session-%230003--SDL-navigation-and-SDUI.md). Planning/capture only; no implementation claimed.

## Session 0004 T002 selection

Owner Session 0004 T002 selects the SDUI portion: persistent toolbar groups and explicit Launch; KanBan mutation services remain unselected.

[Plan](../../05--Implementation/SDUI/Plan.md) and [Session](../../Sessions/session-%230004--SDUI-workflow.md).

## Session 0004 T002 delivered — XFMD 0.7

Combined SDUI composition mindmaps/text, exact revision-checked source spans,
readiness and explicit separate-window Fyne launch are implemented and installed.
Four toolbar groups remain visible in SDP mode; unavailable actions have reasons.
89/89 tests and native/PDF/Fyne inspection passed; see
[U4-001](../../05--Implementation/SDUI/evidence/U4-001/README.md).
No embedded host, SDL execution or independent approval claimed.
KanBan Add/Move remain disabled and unimplemented pending their supported management-service contract. This residual scope keeps the card in-progress; SDUI work is complete.

## Related Session0006 — 2026-10-10

[Session0006](../../Sessions/session-%230006--Discoverable-SDPTool-actions.md) / KB030
will reconcile dynamic action discovery, personal toolbar configuration and this
card's shared availability contract. This is a context link, not a merge or new
selection of KanBan Add/Move implementation. Delivered SDUI behavior and this
card's current lifecycle remain unchanged.
