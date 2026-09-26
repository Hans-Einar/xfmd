# KB-XFMD-018 — Sidebar favorites

| Field | Value |
| --- | --- |
| id | KB-XFMD-018 |
| project | XFMD |
| type | Change |
| CardState | completed |
| created | 2026-09-26T09:47:11Z |
| source | owner-conversation-2026-09-26 |
| next_review | None; M2 correction complete |

## Initial owner direction and M1 scope

Add Favorites beside Folders and Files in the Files sidebar. Support folders and
files, with alphabetically sorted folders above a horizontal separator and
alphabetically sorted files below. Owner: Project Owner; executor: Codex.

Implementation choices: Add opens a mixed file/folder chooser; Remove forgets the
selected favorite without deleting its target. Persist favorites independently of
recent histories. Reuse existing folder navigation and file opening/dirty policy.
Show a separator only when both groups are present. Missing favorites remain
removable; opening one reports an error without changing document or root.

## Review and completion criteria

Completed KB-XFMD-009/013 establish the lower tabs/header; this is an additive
follow-up, not a replacement. Backlog KB-XFMD-014 (SDP tab) and KB-XFMD-003 (SDUI)
remain separate. No consolidation or supersession is needed.

Deliver UR-045/AT-073 through FUNC-010 and
[P057](../../../../sprints/Sprint-009--Favorites/Phase-057--Favorites.md), with native
input, restart persistence, sorting/grouping, remove/cancel/error checks and a
reviewable screenshot. Do not expand into drag/drop or a new bookmark format.

## Worklog

- 2026-09-26T09:47:11Z, Codex, EVT-KB-XFMD-000073: captured the explicit implementation request
  directly as selected work; reviewed related cards; started P057.

## Outcome

- 2026-09-26T09:58:39Z, Codex, EVT-KB-XFMD-000074: completed P057-M1. Favorites supports
  persistent folder/file entries, grouped sorting, a real separator, Add/Remove,
  native activation and existing document/root policies. Seven release checks
  and the dedicated ASan/UBSan test passed. Screenshots were inspected in both
  themes. [Evidence](../../../../sprints/Sprint-009--Favorites/evidence/P057.md)
  records source identity, checks and limitations. This closes the explicitly
  selected implementation scope; no global blueprint Verified claim or merge.

## Owner correction — 2026-09-27 / P057-M2

Remove the Add/Remove buttons and their explanatory text. Add favorites through
right-click menus on files or folders in the Files tree or Folders/Files history.
Agent implementation choice: right-click an existing favorite to remove it.
Preserve persisted entries, grouped sorting and normal activation. Context actions
must target the clicked entry, without opening it or changing document/work root.
Empty space and the separator offer no favorite action; dismissal changes nothing.
This refines the same card; unrelated SDP/SDUI backlog remains separate.

Reopened by Codex, EVT-KB-XFMD-000075, for the explicit owner correction.

## M2 outcome — 2026-09-27

Codex, EVT-KB-XFMD-000076: replaced the chooser/buttons/hint with Add to Favorites
on tree/history context menus and Remove from Favorites on the clicked favorite.
Persistence and grouping remain unchanged. Seven release checks and the dedicated
ASan/UBSan test passed; context dismissal and selection independence are covered.
[Evidence](../../../../sprints/Sprint-009--Favorites/evidence/M2/README.md) records
native results, popup keyboard-grab correction, screenshots and scope limits.
