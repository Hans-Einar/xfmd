# Compact the sidebar and tab recent folders and files

| Field | Value |
| --- | --- |
| id | KB-XFMD-009 |
| project | XFMD |
| CardState | completed |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner items 4 and 5; sidebar portion of item 3 |
| owner | Codex, owner-selected P054 delivery |

## Need and owner direction

The Files sidebar spends too much vertical space on its header and duplicates
information the owner does not use. Apply these changes:

1. Remove the separate current-folder text label above the tree.
2. Move the Refresh icon to the same row as the Files/Index tabs. On Files it
   refreshes the file tree; on Index it refreshes the index.
3. Keep `.md` and `.txt` filter buttons above the tree. Add an Up-one-folder icon
   and an Open icon there. Open must accept both files and folders through the
   behavior in KB-XFMD-006, including updating work path after dialog file opening.
4. Under the folder/file tree, show the existing recent lists in tabs: Folders
   and Files, so they share the available lower area instead of being stacked.
5. Remove the sidebar text-search row when KB-XFMD-008 replaces it.

The screenshot's `warloc` heading is the redundant label to remove. This request
is not a request to remove the tree's own root node.

## Current evidence and affected scope

[WorkspacePanel.cpp](../../../../src/application/ui/WorkspacePanel.cpp) currently
composes a root/refresh row, filename-filter row, type toggles, tree, and stacked
Recent folders / Recent files in a vertical splitter. Both MRU lists already
exist and persist separately; this card reorganizes them. See
[RecentFilesPanel.cpp](../../../../src/application/ui/RecentFilesPanel.cpp),
[IndexPanel.cpp](../../../../src/application/ui/IndexPanel.cpp) and
[Image 1](../evidence/2026-09-24-ui-wishlist/sidebar-before.png).

Review UR-006/012/013/014/021/038 and
[FUNC-010](../../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md),
[FUNC-012](../../../../src/blueprint/functionality/Functionality-012--Work-Path-History.md),
[FUNC-013](../../../../src/blueprint/functionality/Functionality-013--Filtered-File-Tree.md),
[FUNC-020](../../../../src/blueprint/functionality/Functionality-020--Document-Index.md).

## Agent recommendations and open questions

- Up should change the current work path to its immediate parent, including
  outside the current tree root, and stop at `/`. This interpretation is distinct
  from the existing root-double-click jump to home and then `/`; retain or retire
  that older gesture explicitly during requirement review.
- Keep recent-folder and recent-file histories separate, including their existing
  maximum of 32 and persistence. Folder activation changes work path; file
  activation uses normal dirty-protected opening. Do not equate MRU with Back/Forward.
- Keep a resizable tree/recent boundary and useful empty states. Exact initial
  height, selected recent tab and whether selection persists remain UI details.
- Define Index refresh against the current accepted/edited document, including
  headings and the reference subtree. It must not reload the file and discard
  edits; determine which reference caches are refreshed through the existing owner.
- Refresh should preserve current filters/root and user context where practical;
  switching the active tab must not invoke the wrong refresh target.

## Next review and completion criteria

Review with KB-XFMD-006/008 before selecting sidebar work. This card owns placement
and list presentation; opening transactions remain in 006 and text filtering in
008. There is no need to recreate the histories or introduce an independent
opening path for the new button.

Candidate acceptance: narrow/tall/short sidebar layouts; active-tab Refresh on
Files and Index; immediate-parent navigation and root boundary; both recent tabs,
persistence and missing entries; preserved filters and dirty document; Open icon
behavior identical to menu/Ctrl+O. Compare the delivered screenshot to Image 1.
No behavior has been changed or verified in this capture.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-006](%23006--Change--Open-files-and-folders.md), [KB-XFMD-008](%23008--Change--Document-path-and-file-filter.md).

## Goal selection — 2026-09-24

The owner selected KB-XFMD-006–011 as six subgoals of the active chat goal.
KB-XFMD-003 and optional SDUI modeling are excluded. Delivery is tracked in
[Sprint 007](../../../../sprints/Sprint-007--Workspace-UI/README.md).
Implementation is selected; this card stays queued until its phase starts.

## P054 selection — 2026-09-24

Reviewed together with KB-XFMD-006: preserve these separate outcomes while sharing
the workspace delivery. Opening/routing reuses completed P053; optional SDUI work
remains excluded. [P054](../../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md)
records selected interaction rules, owners and acceptance. This card is now active.

## P054 outcome — 2026-09-24

Files/Index and active-tab Refresh share one row; type filters, immediate-parent Up and shared Open sit above the tree. Separate recent folders/files use tabs. Tests cover dirty-buffer/reference refresh and available expanded tree paths, selection and scroll restoration.

Implemented and locally checked in [P054](../../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md).
[Evidence](../../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md) records the
70-check combined acceptance, focused sanitizer checks, inspected native screenshots
and exact source/binary identities. Physical multi-monitor/high-DPI usability remains
outside this automated acceptance. No merge or installation performed.

## SDP KanBan migration — 2026-09-25

EVT-KB-XFMD-000063: relocated the board and added visible CardState (completed).
Existing lifecycle, IDs, decisions and evidence remain unchanged. The owner now
authorizes XFMD's SDP process area; earlier instructions to remain note-only
are historical. [Migration evidence](../../../Maintenance/SDP1/Plan-and-Evidence.md).
