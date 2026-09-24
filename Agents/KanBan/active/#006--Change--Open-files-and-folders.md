# Open documents and folders from the same dialog

| Field | Value |
| --- | --- |
| id | KB-XFMD-006 |
| project | XFMD |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner items 1 and 5 |
| owner | Codex |
| next_review | P054 sidebar Open-icon integration |

## Need and owner direction

The Open dialog must accept both documents and folders. The Open icon above the
Files tree and the existing File/Open entrypoints must use the same capability.
Selecting a document in the dialog must set the work path to that document's
parent folder. Selecting a folder must navigate the work path to that folder.
This connects the dialog to the workspace instead of leaving the tree elsewhere.

## Current evidence and affected scope

Read-only inspection on 2026-09-24: `Application::execute` currently calls
`FXFileDialog::getOpenFilename` with a Markdown/text filter and then `open`.
`Application::startPath` already distinguishes startup files and folders;
`WorkspacePanel::setWorkPath` owns validated root changes and folder history.
Ordinary dialog opening currently does not change the root.

The requested dialog behavior revises the explicit-open exception in section 8 of
[requirements](../../../xfmd_requirements.md), alongside UR-001/009/012/013.
Reuse [FTR-005](../../../src/blueprint/feature/Feature-005--Work-Path-Navigation.md),
[FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md)
and existing document transactions. Source entrypoints:
[ApplicationCommands.cpp](../../../src/application/ApplicationCommands.cpp),
[WorkspacePanel.cpp](../../../src/application/ui/WorkspacePanel.cpp).
No new API is designed or claimed here.

## Agent recommendations and open questions

- Commit the new root only after a successful internal document open. Canceling
  the dirty prompt or failing to load must preserve both document and work path.
  Folder selection should preserve the open document and its dirty state.
- Support selecting the current folder itself, not only traversing folders inside
  a file-only chooser. Resolve the FOX dialog interaction during blueprint work.
- Keep the dialog's scope distinct from link, recent-file and path-bar behavior:
  the owner explicitly required parent-folder changes for dialog document opens.
  Decide other entrypoints deliberately; do not silently change all navigation.
- Define whether a file sent to an external application from this dialog changes
  the root; the current owner instruction does not settle external handoff.

## Next review and completion criteria

Review before selecting the file/workspace portion of the UI work, together with
KB-XFMD-007/008/009. Selection must identify a sprint/phase and update requirements
and plumbing before implementation.

Candidate acceptance: open a folder (including one named `notes.md`); open a file
and observe its parent as work path; exercise menu, Ctrl+O and both Open icons;
check spaces/Unicode, unreadable/missing targets, dirty Cancel and failed loading.
A canceled/failed operation must not add false MRU entries or hide the sidebar.
These are proposed checks, not executed evidence.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-007](../completed/%23007--Change--File-types-and-external-opening.md), [KB-XFMD-008](../backlog/%23008--Change--Document-path-and-file-filter.md), [KB-XFMD-009](../backlog/%23009--Change--Sidebar-controls-and-recent-tabs.md).

## Goal selection — 2026-09-24

The owner selected KB-XFMD-006–011 as six subgoals of the active chat goal.
KB-XFMD-003 and optional SDUI modeling are excluded. Delivery is tracked in
[Sprint 007](../../../sprints/Sprint-007--Workspace-UI/README.md).
This card is active in P053 (file/folder opening and target routing).

## P053 outcome — 2026-09-24

The shared chooser and dialog-origin root policy are implemented and checked.
Native menu, Ctrl+O, toolbar, file/selected-folder/current-folder/cancel and error
retention are covered. [P053 evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md)
records source/binary identity, native screenshots and the final regression run.

Remaining selected scope: add the second Open icon above the Files tree in P054
and check that it invokes this same chooser/policy. This card stays active until
that integration passes; it is not completed by the P053 partial delivery.
