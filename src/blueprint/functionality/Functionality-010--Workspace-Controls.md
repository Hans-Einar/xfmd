---
id: FUNC-010
kind: Functionality
audience: User
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-038, UR-032, UR-033, UR-028, UR-029, UR-025, UR-026, UR-021, UR-011, UR-001, UR-006, UR-007, SR-002, SR-008, SR-013, UR-015, UR-017, UR-018, UR-019, UR-020, SR-019
uses: FUNC-020, FUNC-001, FUNC-002, FUNC-005, FUNC-007, FUNC-012, FUNC-013, FUNC-014, FUNC-015, FUNC-018, FUNC-019
---

# Functionality-010: Workspace, commands and sidebar

## 1. Purpose and scope

Provide thin UI plumbing for the window, splitters, view modes, sidebar and commands.
This is one workspace responsibility, not a feature per button. XfmdWindow must not
parse documents or perform file transactions.

## 2. Requirements and acceptance

Requirements are listed in metadata and defined in the [requirements](../../../xfmd_requirements.md).
Original acceptance: AT-025, AT-001, AT-006, AT-007, AT-012, AT-018, AT-023. Chapter 7 distinguishes later
acceptance amendments from historical evidence.

## 3. Contracts and ownership

XfmdWindow::buildUi builds the window. CommandRouter::dispatch/update handles
actions and enabled state. ViewModeController::setMode/toggleSidebar owns view and
whole WorkspacePanel visibility. SidebarWidget emits copied file paths after native
dispatch. WorkspacePanel owns root/filter/history controls; FTR-005 owns the workflow.

P10–P13 delegated preferences, export and display profiles to dedicated services.
FoxWindowMode owns fullscreen/restore; XfmdWindow builds widgets only. Sidebar and
WorkPathList share scrolling preferences. Menu actions do not change dirty/undo.

P053: OpenPathDialog adapts the FOX mixed file/folder selector and adds Open current
folder. Application::openDialogPath preflights a proposed root, opens through the
existing document transaction, then changes root after successful internal opening.
Application::openTarget shares routing between tree, dialog, recent files and links;
plain text selects editor mode. A later typed-path control will reuse it.

## 4. Behavior, state and failures

Preview is the default; Ctrl+1/2/3 selects modes and F10 toggles the sidebar. Single
click or Enter on a real file activates it. Double-clicking the root broadens the
work root without changing documents. Directories, including names ending `.md`,
remain tree navigation. ID_TREE_EVENT starts at FXTreeList::ID_LAST to prevent
SEL_COMMAND collisions with FOX's ID_HIDE. Tree/document operations retain sidebar
visibility; only explicit sidebar actions change it. View changes retain session/undo.

FoxWheelScrollBar is shared by both axes, retaining fractional wheel deltas and
using FOX timers and changed/command notifications. Constructors replace bars before
create(); widgets own the adapters.

P053, 2026-09-24, supersedes `.md`/`.txt`-only routing. All Open entries use the same
file/folder chooser. Folder selection changes only work root. Failed loading or
canceled dirty prompts retain root/MRU; external handoff retains document and root.
Ctrl is stored with deferred tree activation and carried by host link callbacks.
Known text types retain internal validation errors; bounded unknown UTF-8 text opens
literally in editor mode. HTML uses the configured browser; binary uses OS defaults.
Ctrl always requests OS association for eligible file/web links.

P054 implementation: the separate path/filter row, compact Files/Index and recent
layout, active-tab Refresh and theme popup follow the [phase specification](../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md).
Older toolbar/header placement descriptions are historical; persistence and document
contracts remain. Native acceptance is recorded in the P054 evidence.

P055 implemented zoom behavior follows the [phase plan](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md).
Shared manual scale applies in Wrap/A4; explicit A4 width/height fit uses visible
viewports. Application owns state; host transforms and editor fonts consume it.
Native zoom acceptance is recorded in [P055 evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md); previous evidence retains its dated scope.

P056 places Markdown/Up/Open beside Refresh in the tab header, hiding those three
controls on Index. One oval Markdown icon replaces both type text toggles.
WorkspacePanel owns the checked/filter state; UiButton and ButtonPainter own the
pill presentation, IconCatalog owns the M/down-arrow/box glyph. Implemented changes
follow [P056](../../../sprints/Sprint-008--Sidebar-Header/Phase-056--Sidebar-Header.md).

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `XfmdWindow constructor` | `XfmdWindow::buildUi` | `src/application/ui/XfmdWindow.cpp` | FOX app → window | Parent ownership of widgets | Implemented |
| 2 | `FOX command` | `CommandRouter::dispatch` | `src/application/commands/CommandRouter.cpp` | Command → Application::execute | Check enabled state | Implemented |
| 3 | `Application::execute` | `ViewModeController::setMode` | `src/application/ui/ViewModeController.cpp` | Mode → visible panes | Preserve document | Implemented |
| 4 | `FOX SEL_CLICKED / ID_TREE_EVENT` | `SidebarWidget::onOpen` | `src/application/ui/SidebarWidget.cpp` | File/Ctrl → copied request | Directories navigate; defer opening until release finishes | Implemented |
| 5 | `Application::openTarget internal route` | `Application::open` | `src/application/Application.cpp` | Path → document transaction | Shared dirty policy | Implemented |
| 6 | `SidebarWidget constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → wheel adapter | Parent ownership; before create | Implemented |
| 7 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/remainder → target/timer | Clamp and retain fractional remainder | Implemented |
| 8 | `XfmdWindow::buildUi` | `WorkspacePanel::WorkspacePanel` | `src/application/ui/WorkspacePanel.cpp` | Filter/tree/history → panel | Parent ownership | Implemented |
| 9 | `WorkPathList constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | History → shared wheel adapter | Parent ownership; before create | Implemented |
| 10 | `Edit Preferences` | `PreferencesService::begin` | `src/application/preferences/PreferencesService.h` | Profile → draft | Cancel discards | Implemented |
| 11 | `View Full Screen` | `FoxWindowMode::requestFullscreen` | `src/application/adapters/FoxWindowMode.cpp` | Boolean → WM request | Confirm actual WM state | Implemented |
| 12 | `File Export PDF` | `ExportCoordinator::start` | `src/application/export/ExportCoordinator.cpp` | Snapshot → job | No markSaved call | Implemented |
| 13 | `UiFactory::button` | `UiButton::UiButton` | `src/application/ui/controls/UiButton.cpp` | Role/target/selector → FOX button | Native input; painter owns drawing | Implemented |
| 14 | `Application::applyAppearance` | `UiContext::apply` | `src/application/ui/style/UiStyling.cpp` | Profile → live controls | Preserve document fonts | Implemented |
| 15 | `SidebarWidget::onActivate callback` | `Application::openTarget` | `src/application/ApplicationOpening.cpp` | Local path/Ctrl → consumer | External opening retains document | Implemented |
| 16 | `Application::openTarget` | `DesktopFileOpener::open` | `src/application/adapters/DesktopFileOpener.cpp` | Regular file → xdg-open argv | Startup and asynchronous failures reported | Implemented |
| 17 | `SidebarWidget/NavigationTree::onKey` | `activatesTreeItem` | `src/application/ui/TreeActivation.h` | Key/modifiers/leaf → activation | Retain branch expansion | Implemented |
| 18 | `FOX layout` | `CompactToolbar::layout` | `src/application/ui/controls/CompactToolbar.cpp` | Width → compact rows | Wrap groups at narrow widths | Implemented |
| 19 | `Application::execute` | `EditorWidget::setViewProfile` | `src/application/ui/EditorPresentation.cpp` | Layout/zoom → text width | Preserve text/selection/anchor | Implemented |
| 20 | `Application documents.opened/saved` | `RecentFilesPanel::remember` | `src/application/ui/RecentFilesPanel.cpp` | Successful path → MRU/registry | Max 32; no failed/canceled visit | Implemented |
| 21 | `FOX list activation` | `RecentFilesPanel::onActivate` | `src/application/ui/RecentFilesPanel.cpp` | Absolute path → injected Application::openTarget | Deferred dispatch; shared document errors/dirty policy | Implemented |
| 22 | `Application::execute Open` | `OpenPathDialog::OpenPathDialog` | `src/application/ui/OpenPathDialog.cpp` | Work root → mixed chooser | Cancel returns no selection | Implemented |
| 23 | `accepted Open dialog` | `Application::openDialogPath` | `src/application/ApplicationOpening.cpp` | Selection → document/root/handoff | Validation/dirty failure preserves root | Implemented |
| 24 | `Application::openTarget` | `FileOpenPolicy::classify` | `src/application/io/FileOpenPolicy.cpp` | Regular path → Markdown/Text/Browser/Desktop | Bounded read; propagate I/O errors | Implemented |
| 25 | XfmdWindow::buildUi | `DocumentPathField::DocumentPathField` | `src/application/ui/controls/DocumentPathField.cpp` | committed path/edit/filter → field | no document I/O in widget | Implemented |
| 26 | path submit callback | `Application::openTypedPath` | `src/application/ApplicationWorkspace.cpp` | work-root-relative input → existing target route | report invalid target; retain dirty/document | Implemented |
| 27 | Refresh button | `WorkspacePanel::onRefresh` | `src/application/ui/WorkspacePanel.cpp` | active tab → file/index refresh | preserve document/root/filter | Implemented |
| 28 | Files refresh | `SidebarWidget::refresh` | `src/application/ui/SidebarWidget.cpp` | current tree context → rescan/restore | omit disappeared entries | Implemented |
| 29 | ThemeButton alternate gesture | `ReadingColorPopup::showAt` | `src/application/ui/controls/ReadingColorPopup.cpp` | anchor → existing palette sliders | Escape/outside closes; no theme toggle | Implemented |
| 55 | commands / normalized input | `DocumentZoom::step` | `src/application/zoom/DocumentZoom.cpp` | signed steps → shared bounded scale | leave fit; cancel scroll; retain document | Implemented |
| 56 | viewport/profile changes | `DocumentZoom::refresh` | `src/application/zoom/DocumentZoom.cpp` | geometry → editor/host scale and controls | guard reentrancy; preserve anchor | Implemented |
| 57 | Markdown button activation | `WorkspacePanel::onMarkdown` | `src/application/ui/WorkspacePanel.cpp` | toggle checked state → deferred name/type filter | document/root unchanged | Implemented |
| 58 | Files/Index selection | `WorkspacePanel::onTab` | `src/application/ui/WorkspacePanel.cpp` | active page → Files-only control visibility | retain filter; Refresh remains | Implemented |

## 6. Reuse and dependencies

[FUNC-001](Functionality-001--Document-Session.md),
[FUNC-002](Functionality-002--Local-File-Storage.md),
[FUNC-005](Functionality-005--FOX-Presentation-Host.md),
[FUNC-007](Functionality-007--Preview-Pipeline.md),
[FUNC-012](Functionality-012--Work-Path-History.md) and
[FUNC-013](Functionality-013--Filtered-File-Tree.md).
UC-001/003/004 use this functionality directly; navigation/synchronization consume
the controls without owning them. New commands delegate to the appropriate service,
not implementations in a growing window switch. Other consumed functionality IDs
are retained in metadata for the existing preferences, index, scroll and UI layers.

## 7. Verification

WorkspaceTest uses isolated Xvfb for editor/preview, F10, filters, undo and dirty
close. Historical evidence: [P2](../../../docs/evidence/P2.md),
[sidebar/root clicks](../../../docs/evidence/sidebar-tree.md),
[wheel gestures/bounds](../../../docs/evidence/wheel-scrolling.md).
P7 evaluates aggregate coverage; Implemented is not automatically Verified.
The P10–P13 extension listed AT-029, AT-031, AT-032, AT-033, AT-034, AT-039 for subsequent evidence,
not as claims established by P2.

P15: Files/Index tabs; IndexPanel composes two NavigationTree controls with the same
scroll profile. Single click opens, Enter activates, arrows select; root double-click
retains broader-root navigation. See FUNC-020 and AT-040.

P053: FileOpenPolicyTest, OpenPathDialogGuiTest and FileRoutingGuiTest cover selected
opening/routing behavior with native events; existing document/navigation/input
regressions are retained. Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Implemented originally in P2, with later revisions tracked by phase. The 1.2
[design revision](../../../softwareDesign.md) introduced contracts that earlier
baseline evidence did not establish. Update contracts, calls, consumers and tests
together and retain explicit ownership.

P17–P19 extended UR-025, UR-026 through application/ui/style and controls, preserving
document workflows and FOX input. AT-045, AT-046, AT-047 received separate evidence.
P18-M1: AppearanceGuiTest covered both toolbar theme directions, persistence,
checked view modes and a narrow toolbar with large control fonts; UiControlsTest
separated selected/pressed state and exercised native mouse/Space.
P18-M2: PreviewControls used CommandRouter for Wrap/A4/zoom. Zoom appeared only in
A4 with sufficient width. Files had root/filter/Refresh; Index/References used
PanelHeader. Row height used FOX item measurements, not custom click coordinates.
Native IndexGuiTest and SidebarGuiTest passed. Evidence:
[P17–P19](../../../docs/evidence/P17-P19.md), [UI layer](../../../docs/design/fox-ui-layer.md).
Status remained Implemented; physical experience/other DPI were not automatically verified.

P20: UR-028, UR-029, AT-048, AT-049 and revised AT-046. The original suffix routing used
Application::open for supported files and DesktopFileOpener otherwise; P053
supersedes that classification. Both trees shared Enter/Space and Right-on-leaf
policy. ViewModeController retained split fractions across single-pane transitions;
Editor/Split/Preview used distinct layout icons. P20-M2 reproduced a preview below
100 px on Editor → Split, then retained valid fractions/restored both widths,
including reopening a collapsed pane. AppearanceGuiTest covered repeated switches,
resize, proportions and icon order. Evidence: [P20](../../../docs/evidence/P20.md).

P22: compact shared toolbar; UR-032, UR-033, UR-034, AT-052, AT-053, CompactWorkspaceTest and
regressions. Evidence: [P22](../../../docs/evidence/P22.md).

P24: UR-038/AT-058. RecentFilesPanel owns a bounded MRU list adapter and RecentFiles
registry Path0–Path31. WorkspacePanel originally placed it below folder history.
No separate file-opening service was added. Missing files remain until activation,
which reports the normal error. Absolute paths are identity; labels show filename
then folder with WorkPathHistory::displayPath home abbreviation.
Evidence: [P24](../../../docs/evidence/P24.md).

P053 changes the chooser/routing. Sidebar placement, path/filter row and toolbar
rearrangement are selected P054 work, not yet delivered by this phase. The second
Open icon remains required before closing KB-XFMD-006.

P054 local acceptance: [workspace evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md)
records native interaction/visual checks, focused ASan/UBSan checks and the final
source/binary manifest. Earlier phase placement descriptions retain their dated
scope. Status remains Implemented; this is not blanket physical-display verification.

P056 header/filter acceptance: [evidence](../../../sprints/Sprint-008--Sidebar-Header/evidence/P056.md).
