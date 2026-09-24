# Sprint 007 — Workspace UI

The owner selected all six UI backlog Changes as a persistent chat goal on
2026-09-24. Complete each subgoal with requirements, plumbing, implementation,
native checks and screenshots. SDUI widget ownership and optional SDUI modeling
are excluded. No merge or installation is implied.

| Phase | Cards / outcome | Branch | Status |
| --- | --- | --- | --- |
| [053](Phase-053--File-Opening.md) | KB-XFMD-006/007: unified file/folder opening and target routing | sprint/007/phase/053-file-opening | Locally complete; #006 sidebar integration remains P054 |
| 054 | KB-XFMD-008/009/011: path/filter row, compact sidebar and toolbar/color popup | sprint/007/phase/054-workspace-layout | Selected, queued |
| 055 | KB-XFMD-010: shared zoom and controls | sprint/007/phase/055-document-zoom | Selected, queued |

One combined sprint PR after phase acceptance. One commit per milestone, with
phase branches preserving history. Base `6703a58` is the explicit checkpoint of
previously uncommitted reconciliation, tooling, SDL verification and captured UI
notes. It does not retroactively change those reports' tested source identities.

## Selected interaction defaults

- Open dialog accepts a selected file/folder and offers Open current folder.
  Successful internal dialog file opening moves the work root to its parent;
  canceled/failed loading does not. External handoff preserves root/document.
- UTF-8 plain text uses the editor regardless of suffix; Markdown keeps its view
  mode. HTML uses the configured browser. Ctrl+click uses OS defaults. No shell
  interpretation. Existing 8 MiB internal-edit limit remains.
- Path/filter row: typing filters names; Enter opens an exact existing target,
  including a literal filename containing wildcard characters. Directories change
  work root; typed files retain it. No implicit partial-match opening. Escape
  restores the previous filter and the committed document path. Right-click copies
  only the committed path. Relative typed paths use work root; links use document
  directory. No mode toggle.
- Sidebar Up uses the immediate parent, stopping at `/`; retain the existing
  double-click-root shortcut. Keep separate 32-entry MRUs and resizable geometry.
  Refresh Index rebuilds from the current accepted buffer and clears reference
  caches; it never reloads/discards edits.
- Zoom is shared by editor/preview, session-local, 25–300% with ten-percentage-point
  wheel/key steps and presets 25/50/100/200/300. Manual changes leave fit mode.
  A4 fit uses one page's width or height. Wrap manual zoom reflows content; fit
  modes are only available for A4, with a clear explanation. PDF geometry is fixed.
- Theme popup reuses existing sliders/persistence, stays usable during dragging,
  closes on Escape/outside click and has keyboard access. Left-click still toggles.

These are implementation decisions within the selected cards, not prior owner
quotes. Amend them if native verification reveals a better fit to the stated needs.
