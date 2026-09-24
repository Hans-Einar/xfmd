---
id: FTR-005
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-012, UR-013, UR-014
uses: FUNC-010, FUNC-012, FUNC-013
---

# Feature-005: Workspaces with history

## 1. Purpose and scope

The work root provides a focused file area with reusable filtering and history.
Document navigation, parsing and storage retain their existing owners.

## 2. Requirements and acceptance

UR-001, UR-012, UR-013, UR-014; see the [requirements](../../../xfmd_requirements.md).
AT-001, AT-026, AT-027, AT-028 cover opening, startup, root selection, persistence and combined filtering.

## 3. Contracts and ownership

WorkPathHistory owns the canonical root and a unique MRU list bounded to 32 paths.
WorkspacePanel owns FOX filter/history controls and registry persistence.
SidebarWidget owns FXTreeList nodes and DirectoryScanner. FileNameFilter provides
pure name matching; the worker owns filesystem data only and the GUI polls results.

## 4. Behavior, state and failures

The default root is home. A directory argument sets the root; a file argument
uses its parent. The root appears expanded. The existing root double-click shortcut
broadens to home, then `/`; context/history selection shares validation. Errors
preserve the old root. Filtering is (selected suffixes OR) AND name pattern;
no suffix selection means all. A filtered tree contains matches and ancestors;
an unfiltered tree loads children on expansion. Root/filter changes cancel scans.
Unreadable directories are reported, directory symlinks are not followed, and file
symlinks outside the root are omitted. Changing the root preserves the document.

P053, 2026-09-24: the Open dialog accepts a selected or current folder and changes
only the work root. A successfully opened internal file changes the root to its
parent; loading errors, canceled dirty prompts and external handoff preserve root
and MRU. Links and recent-file activation retain their existing root.

P054: the sidebar Open icon dispatches the same Open command. Up selects the
immediate parent and disables at `/`; the older root-double-click shortcut remains.
The document-path row owns name-filter entry, independent of sidebar visibility.
Typed exact paths reuse routing but internal files retain the work root. Folders
and Files recent tabs preserve separate histories; Files Refresh restores available
tree context without changing the document. See FUNC-010/013 for adapter calls.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `main CLI path argument` | `Application::startPath` | `src/application/Application.cpp` | Directory → root; file → document/parent | Invalid path fails | Implemented |
| 2 | `SidebarWidget root/context callback or history click` | `WorkspacePanel::requestWorkPath` | `src/application/ui/WorkspacePanel.cpp` | Copied path → deferred activation | No tree-node pointer survives native dispatch | Implemented |
| 3 | `WorkspacePanel::onActivate / Application::startPath / openDialogPath` | `WorkspacePanel::setWorkPath` | `src/application/ui/WorkspacePanel.cpp` | Validated root → tree/MRU | Invalid target preserves root/document | Implemented |
| 4 | `WorkspacePanel::setWorkPath` | `WorkPathHistory::activate` | `src/application/workspace/WorkPathHistory.cpp` | Path → canonical root/history | Readability before state commit | Implemented |
| 5 | `WorkspacePanel::setWorkPath` | `SidebarWidget::setRoot` | `src/application/ui/SidebarWidget.cpp` | Root/label → expanded tree/worker | Discard old worker and entries | Implemented |
| 6 | `WorkspacePanel::setWorkPath` | `WorkspacePanel::remember` | `src/application/ui/WorkspacePanel.cpp` | MRU → list/registry | Abbreviated display; absolute persistence | Implemented |
| 7 | `Application Open command` | `Application::openDialogPath` | `src/application/ApplicationOpening.cpp` | Selection → root/document/handoff | Folder preserves dirty document; cancel/error preserves root | Implemented |

## 6. Reuse and dependencies

[FUNC-010](../functionality/Functionality-010--Workspace-Controls.md),
[FUNC-012](../functionality/Functionality-012--Work-Path-History.md) and
[FUNC-013](../functionality/Functionality-013--Filtered-File-Tree.md).
FileNameFilter and DirectoryScanner serve startup, filtering and root changes.
They do not depend on the interpreter/renderer. Dialog document opening uses
Application's shared routing and existing document transaction.

## 7. Verification

Historical P8: WorkPathTest covers matching/history/scanning; WorkPathGuiTest
covers native input, startup, filtering and root changes. SidebarGuiTest and
WheelGuiTest passed. Evidence: [P8](../../../docs/evidence/P8.md).

P053 checks file/selected-folder/current-folder opening, Unicode/spaces and
root/document/MRU retention. Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Implemented in P8, extended in P053. Large or slow filesystems may take time;
progress is visible and old work is canceled. Tree containment is not an OS sandbox.
P054 adds the second Open icon, immediate-parent navigation and tabbed histories.
Native acceptance is recorded in [P054 evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md).
