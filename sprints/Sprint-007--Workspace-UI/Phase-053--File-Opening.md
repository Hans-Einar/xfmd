# Phase 053 — File and folder opening

Branch `sprint/007/phase/053-file-opening`, base `6703a58`. Collaborative mode.
KB-XFMD-006/007; UR-001/005/009/012/013/024/028, SR-005/006/011.
Reuse FTR-003/005 and FUNC-002/005/010/012/014; no new feature/service object.

## Milestones

| Milestone | Deliverable | Evidence |
| --- | --- | --- |
| M1 | Unified chooser and routing, updated requirements/plumbing and native checks | Complete: [identified evidence](evidence/P053.md); commit is the introducing P053 milestone |

## Selected behavior and planned plumbing

Application owns opening policy; XfmdWindow only builds UI. OpenPathDialog adapts
FOX selection, including a current-folder button. Application::openDialogPath
preflights the proposed root then opens through the existing document transaction;
it changes root only after successful internal opening. Folder selection leaves
the active document untouched. External handoff leaves both root and document.

FileOpenPolicy classifies regular local targets. Markdown/text suffixes use the
internal loader and report invalid input; HTML/HTM use the configured browser.
Other files are classified by bounded UTF-8/control-byte validation: plain text
opens in editor mode; binary or oversized unknown files use OS defaults. This
preserves the 8 MiB internal limit without blocking ordinary binary handoff.
Empty files are valid text. A literal `.md` extension is required for Markdown
interpretation; arbitrary plain text does not become Markdown from its contents.

Application::openTarget is shared by tree, dialog and later typed paths.
Application::followLink resolves from its source document, then uses the same
routing policy. FOX link callbacks carry captured Ctrl state; deferred tree
activation stores the modifier with the copied path. Navigator links retain
their own document consumer for normal internal navigation.

ExternalBrowser handles validated HTTP(S) URLs and explicit existing local HTML
paths, launches argv without a shell, and reports asynchronous exit failures.
Ctrl+click uses xdg-open even with another browser configured. DesktopFileOpener
continues handling local OS association. Do not execute targets as programs.

## Acceptance

Native dialog selection (file, selected folder, current folder, cancel); menu and
Ctrl+O dispatch; root/document/dirty/MRU retention on cancellation and load failure;
directory named notes.md; Unicode/spaces; text/HTML/binary and Ctrl routes from
real tree and link clicks; configured versus OS browser; launcher argument
integrity and failed launches. The second Open icon is added and verified with
the sidebar phase; KB-XFMD-006 cannot close before that integration check.

Preserve byte-for-byte save behavior, ordinary navigation anchors and existing
input/endpoint contracts. Run focused tests at phase completion and record actual
source identity, commands, results and native screenshot here.

## Phase outcome — 2026-09-24

M1 is complete locally. The final corrected build passed 67/67 non-Rust CTests;
the unchanged Rust tests passed in the initial full run. Native screenshots exposed
and then verified the correction of an off-dialog footer/hidden forwarding-button
mistake. The stricter tests now require visible controls. See the evidence and
source/binary manifest for exact build identity and test provenance.

KB-XFMD-007 is complete. KB-XFMD-006 remains active for its second Open icon and
P054 integration check. Continue on the selected workspace-layout phase without
merging or installing. No overall goal completion is claimed.
