# Preview SDL design files through SDPTool

| Field | Value |
| --- | --- |
| id | KB-XFMD-015 |
| project | XFMD |
| CardState | backlog |
| type | Proposal |
| created | 2026-09-25T09:35:14Z |
| source | Owner conversation 2026-09-25: direct .design rendering before full native navigation |
| next_review | Select a bounded saved-file preview with KB-SDP-017, before full sidebar implementation |

## Owner need and existing foundation

Opening a supported SDL `.design` file should show diagrams of its content in the
preview pane, with the original source available for editing. The owner suggests
prioritizing this ahead of the full SDP navigation sidebar and using sdptool to
translate design content for presentation.

Current source inspection: FileOpenPolicy classifies a readable .design as plain
text. DocumentViews can already invoke a registered SDL tool and display generated
Markdown/resources, but does not bind an editable SDL source buffer to its preview.
The Go SDL CLI already parses structural .design files and projects model facts
into Mermaid and document bundles. With mmdr it produces SVG, including SDL-specific
Go symbol drawing for flowcharts using Rust layout. No Python frontend or new Rust
SDL parser is needed. The producer is KB-SDP-017:

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/active/%23017--Proposal--sdptool-and-project-navigation.md

Current pipeline and producer proposal:

file:///home/warloc/git/SDP-vNow/Toolkit/SDPTool/Navigation-and-Design-Preview.md

## Recommended bounded delivery, not adopted product requirements

1. Add a native document/preview adapter for a supported saved structural SDL
   file, using host-registered sdptool and the shared producer contract. A valid
   standalone file should work without requiring a full SDP project/sidebar.
2. Display a compact model-derived overview and relevant diagrams; allow viewpoint
   selection through supported producer targets. Avoid a giant graph or all detail
   diagrams on every update. Define supported profiles and large-file behavior.
3. Retain the .design path/source buffer, dirty state, undo and save destination;
   generated Markdown/SVG is derived preview, never the replacement editable file.
4. Add unsaved-buffer preview as a separate milestone using source snapshots and
   revision IDs; current SDL CLI reads saved files. Debounce/cancel, reject stale
   responses, show source-positioned diagnostics and preserve the last valid view.

Use the complete generated Markdown/SVG package to preserve current SDL-specific
symbols. Mermaid-only presentation can be an explicit mode but need not reproduce
all those shapes. Do not duplicate SDL parsing/projection or interpret model code
inside XFMD. Preview must not execute callbacks/domain logic. Resolve relative
resources from an explicit source base, with temporary-bundle lifetime owned by
its preview session.

## Ownership and related work

XFMD owns source/preview pairing, file-type routing, native UI and refresh/error
behavior. SDP-vNow owns the sdptool producer operation and existing SDL service
reuse. Native implementation happens in XFMD under its own requirements → blueprint
→ plumbing → code → verification process; this is only a backlog capture.

[KB-XFMD-014](%23014--Proposal--SDP-sidebar-and-generated-navigation.md) owns the
separate native SDP tree and project discovery integration. It may reuse this
adapter; direct preview need not wait for all its subtabs. Neither card turns XFMD
into an SDUI widget runtime or adopts SDP as XFMD's development process.

## Acceptance when selected

Supported saved .design opens with source and generated preview; a changed source
updates diagrams, not only textual tables. Invalid source shows position/diagnostic
and preserves the last valid preview. Missing tools and unsupported profiles fail
clearly. Editing/saving cannot overwrite source with Markdown. Test two windows,
rapid changes/stale results, paths with spaces, bounded large output and resource
cleanup. Add unsaved-buffer tests only when that milestone is implemented; do not
claim them from a disk-based preview test. Preserve existing Markdown/text workflows.

## Worklog

Recorded 2026-09-25T09:35:14Z, Codex, EVT-KB-XFMD-000048. Primary card created in backlog;
no XFMD code, blueprint or product requirement changed. The direct-preview adapter
and producer operation remain unimplemented.

Producer link updated 2026-09-25T10:16:50Z, Codex, EVT-KB-XFMD-000051: KB-SDP-017 is now
active. Its design uses SDP’s own numbered phases; XFMD keeps its existing process.

## SDP KanBan migration — 2026-09-25

EVT-KB-XFMD-000055: relocated the board and added visible CardState (backlog).
Existing lifecycle, IDs, decisions and evidence remain unchanged. The owner now
authorizes XFMD's SDP process area; earlier instructions to remain note-only
are historical. [Migration evidence](../../../Maintenance/SDP1/Plan-and-Evidence.md).
