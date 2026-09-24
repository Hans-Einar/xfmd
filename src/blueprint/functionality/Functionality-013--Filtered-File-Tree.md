---
id: FUNC-013
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-014
uses: FUNC-012
---

# Functionality-013: Filename filtering and tree contents

## 1. Purpose and scope

Deliver filename matches and tree contents without blocking FOX on recursive searches.
FUNC-012 owns root/history; existing document services own opening.

## 2. Requirements and acceptance

UR-014 / AT-028; see the [requirements](../../../xfmd_requirements.md).

## 3. Contracts and ownership

FileNameFilter owns type flags and a name pattern without FOX or I/O.
DirectoryScanner owns one worker, directory jobs and at most 4096 pending entries.
GUI take() retrieves at most 512 per poll; the worker never calls FOX. SidebarWidget
owns FXTreeList nodes, its path-to-node map and requested directories. setRoot stops
and joins the old worker before deleting entries. WorkspacePanel owns the active
name pattern; DocumentPathField is its P054 UI consumer, replacing the sidebar input.

## 4. Behavior, state and failures

The filter contract retains type OR followed by name AND. P056 exposes only its
Markdown flag in the workspace, with the text flag false. Markdown off admits all
types, including plain text; on admits .md. Tab switches preserve that choice; a pattern without wildcards matches substrings, otherwise the whole basename.
`?` counts UTF-8 characters and ASCII letters compare case-insensitively. Empty
filter loads children on expansion; active filtering searches recursively and emits
only matching files. GUI adds ancestors and retains the root even without matches.
Unreadable directories are reported; directory symlinks are not followed and file
symlinks outside the root are omitted. Cancellation wakes producer backpressure.

P054 Files Refresh preserves the root/filter, rescans and restores available expanded
paths, selection and scroll. Disappeared entries are omitted. Changing a root/filter
still cancels obsolete restoration and scans. A queued filter edit is applied before
Refresh instead of being dropped. The path field changes filtering even when Files
is hidden, without opening targets or silently selecting that tab.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `WorkspacePanel::onApplyFilter` | `SidebarWidget::setFilter` | `src/application/ui/SidebarWidget.cpp` | Toggle states and pattern → rebuild current root | Old scan stopped; document unaffected | Implemented |
| 2 | `SidebarWidget::setRoot` | `DirectoryScanner::start` | `src/application/workspace/DirectoryScanner.cpp` | Root and filter → one worker with root job | Stop/join old worker and discard old pending entries | Implemented |
| 3 | `SidebarWidget::expandTree` | `DirectoryScanner::request` | `src/application/workspace/DirectoryScanner.cpp` | Unfiltered directory → queued direct-child job | Once per node generation; no request outside root | Implemented |
| 4 | `DirectoryScanner::scan` | `FileNameFilter::matches` | `src/application/workspace/FileNameFilter.cpp` | Basename → type OR followed by name AND | No parsing or FOX dependency | Implemented |
| 5 | `FOX poll timer` | `SidebarWidget::onPoll` | `src/application/ui/SidebarWidget.cpp` | Entries → path nodes and matching ancestors | GUI thread only; status includes unreadable count | Implemented |
| 6 | `SidebarWidget::onPoll` | `DirectoryScanner::take` | `src/application/workspace/DirectoryScanner.cpp` | Up to 512 entries, busy state and errors | Mutex exchange releases bounded producer backpressure | Implemented |
| 7 | `DirectoryScanner::scan` | `DirectoryScanner::publish` | `src/application/workspace/DirectoryScanner.cpp` | Entry → pending queue | Wait at 4096 entries; cancellation wakes producer | Implemented |

| 8 | `DirectoryScanner::start / destructor` | `DirectoryScanner::stop` | `src/application/workspace/DirectoryScanner.cpp` | cancellation predicate under mutex → notify → join | prevents lost wakeup between predicate check and wait | Implemented |

| 9 | DocumentPathField filter callback | `WorkspacePanel::setNameFilter` | `src/application/ui/WorkspacePanel.cpp` | typed basename → stored pattern/debounce | no document open while typing | Implemented |
| 10 | WorkspacePanel Files Refresh | `SidebarWidget::refresh` | `src/application/ui/SidebarWidget.cpp` | tree context → rescan/restoration | stale/removed nodes never reused | Implemented |

## 6. Reuse and dependencies

[FUNC-012](Functionality-012--Work-Path-History.md) owns path containment policy.
One FileNameFilter/DirectoryScanner pipeline serves startup, filtering and root
changes. No interpreter/renderer dependency. File opening reuses Application's
P053 target routing; filtering does not execute targets.

## 7. Verification

Historical P8: WorkPathTest covered matching/history/scanning; WorkPathGuiTest
covered native input, startup, filtering and root changes. SidebarGuiTest and
WheelGuiTest passed. Evidence: [P8](../../../docs/evidence/P8.md).

P054 adds PathWorkspaceGuiTest for native editing/paste/Enter/Escape/clipboard,
filter combinations and Refresh. Acceptance is recorded in
[P054](../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md).

## 8. Status, risks and change impact

Implemented in P8; large/slow filesystems can take time. Progress remains visible
and obsolete work is canceled. Tree containment is not an OS sandbox.
P27 found a lost wakeup in stop/join: the predicate now changes under the same mutex
as the condition-variable wait. WorkPathTest includes 100 rapid start/stops and
backpressure. P054 changes presentation/context restoration, not scanner ownership.

P054 local acceptance: [workspace evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md)
records native interaction/visual checks, focused ASan/UBSan checks and the final
source/binary manifest. Earlier phase placement descriptions retain their dated
scope. Status remains Implemented; this is not blanket physical-display verification.

P056 header/filter acceptance: [evidence](../../../sprints/Sprint-008--Sidebar-Header/evidence/P056.md).
