# Compact the Files header and use one Markdown filter

| Field | Value |
| --- | --- |
| id | KB-XFMD-013 |
| project | XFMD |
| CardState | completed |
| type | Change |
| created | 2026-09-24T20:15:57+00:00 |
| source | Owner conversation, 2026-09-24: local XFMD 0.3 UI follow-up |
| owner | Codex |

## Owner direction

Move the file controls beside Reload. Hide them when Index is selected. Remove
.txt entirely and make the remaining Markdown filter an oval icon button. The
owner asked whether M with a downward arrow is the Markdown mark.

## Review and selected scope

This corrects the earlier two-toggle/second-row placement in completed KB-XFMD-009;
its other delivery remains complete. KB-XFMD-003/012 are unrelated and remain in
backlog. No consolidation or reopening of the whole earlier card is needed.

The [Markdown Mark source](https://github.com/dcurtis/markdown-mark) describes M,
down arrow and enclosing box; it is public domain. The UI uses an adapted small
glyph in the existing icon catalog. Agent decision: on means Markdown-only; off
means all types, intersected with the current name filter. Keep state across tabs.

[P056](../../../../sprints/Sprint-008--Sidebar-Header/Phase-056--Sidebar-Header.md)
is selected by this explicit implementation request. Completion requires native
geometry/visibility/filter checks and screenshots. Existing text opening remains.

## Outcome — P056

Implemented the single header and oval Markdown-only/all-types toggle. The three
Files controls hide on Index and restore their checked state on return. Native
mouse/Space, filtering, geometry, Up/Open/Refresh and theme regressions pass.
[P056 evidence](../../../../sprints/Sprint-008--Sidebar-Header/evidence/P056.md)
records nine relevant checks plus the final appearance/capture rerun, inspected
screenshots and source/binary hashes. No merge or formal release is performed.

## SDP KanBan migration — 2026-09-25

EVT-KB-XFMD-000066: relocated the board and added visible CardState (completed).
Existing lifecycle, IDs, decisions and evidence remain unchanged. The owner now
authorizes XFMD's SDP process area; earlier instructions to remain note-only
are historical. [Migration evidence](../../../Maintenance/SDP1/Plan-and-Evidence.md).
