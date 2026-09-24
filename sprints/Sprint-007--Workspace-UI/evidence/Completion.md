# Sprint 007 — Six-card completion audit

Audit date: 2026-09-24. Scope is the owner's selected KB-XFMD-006–011 goal.
Each original card retains its own outcome; there is no consolidation or new
product scope. The later KB-XFMD-012 process Ref and KB-XFMD-003 SDUI ownership
remain backlog work. No BoxUI implementation is imported.

## Owner requirements against delivered behavior

| Card | Delivered behavior and concrete verification | Evidence |
| --- | --- | --- |
| 006 | File menu, Ctrl+O and both Open icons share the FOX file/folder chooser. Successful internal file opening updates root to its parent; selected/current folders update root only. Native tests include Unicode, a directory named notes.md, dirty Cancel, missing target and state retention. | [P053](P053.md), [P054](P054.md): OpenPathDialogGuiTest |
| 007 | Unknown-suffix UTF-8 text opens literally in the editor; Markdown retains view mode. HTML and HTTP(S) use the browser. Ctrl activation in preview, index and tree uses OS association. Binary uses OS handoff. FileOpenPolicy and native routing tests cover precedence, relative bases, byte-preserving save, canceled opens and argv/error handling. | [P053](P053.md): FileOpenPolicyTest, FileRoutingGuiTest, ExternalBrowserTest |
| 008 | Full-width row below toolbar displays the committed absolute path. Left-click/F6 edits; typing filters names automatically; Enter addresses an exact absolute/work-root-relative target. Right-click copies committed path, including during editing. Duplicate sidebar search is removed. Native tests cover paste, Unicode, literal wildcards, Escape/blur, clear, missing paths, dirty Cancel and narrow widths. | [P054](P054.md): PathWorkspaceGuiTest |
| 009 | Redundant folder label is removed; Files/Index share a row with active-tab Refresh. Type filters, immediate-parent Up and shared Open are above the tree. Independent recent folders/files use lower tabs. Native tests check root boundary, tree expansion/selection/scroll restoration, dirty-buffer index refresh, reference invalidation, MRU persistence and cancellation. | [P054](P054.md): PathWorkspaceGuiTest, WorkPathGuiTest, RecentFilesGuiTest |
| 010 | Shared editor/preview manual zoom in Wrap and A4: Ctrl wheel/plus/minus/keypad, current percentage, five presets and toolbar width/height fit. View submenu has presets and in/out shortcuts without fit. Native 96/144-DPI tests cover all view/layout combinations, resize/sidebar fit, bounds/fractional input, anchors, selection, dirty/undo and saved bytes. Actual exports retain every page and word box across four zoom modes. | [P055](P055.md): DocumentZoomGuiTest, DocumentZoomDpiTest, DocumentZoomPdfTest |
| 011 | Sidebar is first beside menus; grouped view controls precede Back/Forward; Theme is rightmost. Existing reading-color controls live in its alternate-gesture popup. Ordinary toggle, keyboard popup/Escape, native pointer dragging/outside dismissal, palette transactions and teardown are checked. | [P054](P054.md): ThemePopupGuiTest, ReadingColorsGuiTest, CompactWorkspaceTest |

The audit reread the owner-direction sections and current composition/routing/zoom
calls. Existing per-phase evidence identifies the checks, artifacts and source
hashes; historical failed runs remain recorded. P055's complete 73-test regression
run covers all registered tests, including the pinned real SDL integration and
P053/P054 behavior. Subsequent visual acceptance is identified separately in P055.
Blueprints remain Implemented; this bounded goal does not claim every historical
acceptance criterion in every blueprint is globally Verified.

## Decisions and limits

Routine interaction decisions are recorded in the sprint README and phase plans:
exact Enter versus incremental filtering, external handoff preserving work root,
shared session-local zoom, explicit A4 fit and Wrap reflow. These are implementation
decisions within the selected work, not retroactive owner quotations.

Native acceptance uses isolated Xvfb and temporary homes. The 144-DPI run sets
FOX's screen resolution; it is not physical monitor/touchpad acceptance. External
launchers are controlled stubs, so local desktop associations are not certified.
At very narrow enlarged Wrap widths, single glyphs and fixed nested indentation
can still overflow; code/table overflow semantics are retained. A4 height-fit can
require horizontal scrolling because it fits height only. PDF geometry is fixed.
UTF-8 detection retains the existing 8 MiB internal-edit limit.

No merge or installation is part of this delivery. The combined sprint PR must
preserve the three phase commits and its dependency on the pre-UI checkpoint;
SDL and historical BoxUI PRs remain separate.
