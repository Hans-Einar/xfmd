# Phase 054 — Workspace layout and interactions

Branch `sprint/007/phase/054-workspace-layout`, base `8a8ee4d`. Autonomous phase:
implement the composed workspace then build/test at its boundary; corrective
builds follow findings. KB-XFMD-008/009/011 and remaining KB-XFMD-006 integration.
Reuse FUNC-010/012/013/014/020 and FTR-005. No SDUI prerequisite.

## Milestones

| Milestone | Outcome | Evidence |
| --- | --- | --- |
| M1 | Path/filter row, compact sidebar, grouped toolbar and reading-color popup | Implemented and locally accepted; [evidence](evidence/P054.md) |

## Selected interactions and ownership

DocumentPathField is a FOX adapter under application/ui/controls. At rest it shows
the committed full path (or an explicit unsaved placeholder); dirty indication is
separate. Left-click or keyboard entry starts editing. Typing applies the existing
filename filter (basename for a path-shaped entry) with the existing type toggles.
Enter submits the exact existing absolute/work-root-relative target; literal `*`/`?`
filenames take precedence over wildcard interpretation. It never opens a partial
match. Directories change root; typed files use P053 routing and retain root.
Successful submission restores the pre-edit name filter and resting path; failed
submission retains editable input with feedback. Escape restores the pre-edit
filter/path; focus loss commits the filter and restores path display. A clear-filter
button removes the name pattern. Right-click copies only the committed path, even
while editing. Relative base and active filter appear in the field's tooltip.
The row remains present with Sidebar hidden or Index selected; it does not silently
switch tabs. XfmdWindow constructs the row, ApplicationWorkspace coordinates paths.

WorkspacePanel composes a Files/Index tab bar plus active-tab Refresh and a switcher.
The redundant folder heading and sidebar text search disappear. Above the retained
tree root, type toggles sit with immediate-parent Up (disabled at `/`) and Open.
Open dispatches the existing command. The lower splitter area holds Folders/Files
recent tabs with independent existing 32-entry persistence. Files Refresh preserves
root/filter and restores available expanded paths, selection and scroll. Index
Refresh cancels pending reference results and rebuilds from the current accepted
buffer, or requests its pending model; it never reloads the document from disk.

Toolbar order: menus, Sidebar, Open/Save, spaced Editor/Split/Preview group,
Back/Forward, Wrap/A4/current zoom, flexible space, Theme at the right edge.
Whole groups wrap at narrow widths; the independent path row remains below.
Theme left-click/Space retains its command. Right-click or Shift+F10/Menu opens a
ReadingColorPopup containing existing PreviewColorControls; Escape/outside click
closes it and slider dragging remains usable. Reuse live palette, commit/rollback
and separate Light/Dark persistence. PDF palette, document/undo and anchors remain
unchanged. P055 replaces the existing zoom controls; this phase reserves their group.

## File map and acceptance

New application-owned adapters: DocumentPathField.{h,cpp}, ReadingColorPopup.{h,cpp},
ThemeButton.{h,cpp}; ApplicationWorkspace.cpp owns path/workspace coordination.
WorkspacePanel, SidebarWidget, RecentFilesPanel, XfmdToolbar and CompactToolbar
retain their existing responsibilities. IconCatalog gains a native Up glyph.
No interpreter/renderer dependency change or generic utility owner.

Run native path editing/clipboard/filter/dirty-error tests, Files/Index Refresh,
root boundary, recent tabs/restart/missing entries, sidebar Open, toolbar order and
geometry, popup pointer/key/drag/outside dismissal, palette persistence and unchanged
buffer/selection/PDF regressions. Capture wide/narrow workspace and open popup;
inspect actual pixels and visible hit targets, not merely direct hidden-widget calls.
Record actual source/binary identity and only close cards with matching evidence.

## Phase outcome — 2026-09-24

M1 is complete locally. All 70 registered checks are covered by the full run plus
corrected document-link/native acceptance checks. Three focused ASan/UBSan checks
pass, with the final screenshot-wait test separately rebuilt and checked. Five
native screenshots were inspected. Evidence records failed intermediate findings,
corrections, source/binary identity and physical-display limits.

KB-XFMD-006/008/009/011 are completed with append-only ledger events. P053's #007
remains complete; P055/#010 is the remaining selected subgoal. The concurrent #012
process Ref is preserved separately; only its external-checkout link formatting
was repaired to pass repository checks. No merge, installation or overall goal
completion is claimed.
