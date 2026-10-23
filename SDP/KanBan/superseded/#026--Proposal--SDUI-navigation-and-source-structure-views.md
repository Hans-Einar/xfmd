# SDUI navigation, runnable entries and source-structure views

| Field | Value |
| --- | --- |
| id | KB-XFMD-026 |
| project | XFMD |
| type | Proposal |
| CardState | superseded |
| created | 2026-10-06T01:17:23.175880+00:00 |
| source | Owner Session 0002 T006, 2026-10-06 |
| next_review | Next SDP navigation work selection; coordinate producer and consumer changes |
| tags | SDUI, Fyne, source, Mermaid, navigation |

## Owner outcome

Provide an SDUI tab listing runnable SDUI systems. Investigate hosting the Fyne
application inside XFMD. If that is not practical, show a schematic diagram tree
of SDUI source in Markdown: explain how the code is assembled, not a rendering
of the UI. Determine a recognizable diagram vocabulary supported by XFMD.

## Existing implementation and the actual gap

The discovery contract already emits a kind=tab SDUI root, validated source nodes
and named frame children. XFMD already builds dynamic tabs from that contract and
calls sdui-preview for frame selections. Extend this existing tab; do not add a
competing hardcoded Files/Index peer or a second SDUI source registry.

SDPTool/sdui.go emits source/profile validation state and preview targets, not a
runnable-system declaration. A parsed frame is not proof that its callbacks or
SDL runtime are bound. The sdui-fyne host creates its own Fyne window, supports
explicit local prototype bindings, and leaves symbolic SDL callbacks unbound.

SDUI already has presentation.Markdown and CLI --format markdown. It produces a
layout dump and static labels/content from normalized instances; it is not the
requested source-analysis diagram exporter. Reuse the frontend; extend the
presentation package with a distinct structural output mode rather than create
a second parser or confuse the two outputs.

## Proposed interaction and producer contract

- Preserve producer-defined dynamic SDUI tab and stable source/model identities.
  Group by declared System/entry when the producer actually supplies that fact;
  otherwise label source + frame honestly. Do not invent System names from paths.
- Each entry exposes source profile/revision, entry frame, compatible host/runtime
  availability and binding readiness. Distinguish runnable UI prototype,
  application-connected runtime, invalid source and unavailable host, with reasons.
  Exact capability/launch schema is a producer design task, not implemented today.
- Default activation opens a structural source overview. Explicit actions can
  include View structure, View source and Run in Fyne. Launch only on the user's
  action; the overview never executes callbacks or starts applications.
- Prefer a separate Fyne window initially. Native embedding remains an optional
  isolated feasibility study, not a prerequisite for useful source navigation.
  Tie launch lifetime/error reporting to an explicit policy; opening a new
  Markdown document must not silently terminate a running prototype.
- Return ordinary Markdown plus Mermaid fences and source/revision links through
  the established generated-view contract. Generate details when selected;
  follow #025 rather than adding another fixed total-entry cap.

## Structural projection design

See [analysis and illustrative vocabulary](../evidence/2026-10-06-sdui-navigation/README.md).
Recommended primary view: top-down flowchart containment tree, with consistent
labels for System/source, definition/frame, region, row, widget and Markdown.
Represent reuse/binding edges separately from ownership. Pair it with a simple
block diagram per frame for header/body/footer and horizontal row composition.
Use text labels and a legend; recognition must not depend on custom CSS/colors.

Project both source declarations and normalized instances. Preserve declaration
identity, each instance path, ordered rows, region roles, overrides/layout rules,
module/object/member references and source spans. Do not collapse a reused frame
into one node when the view claims to show instances. Do not omit hidden nodes in
a source view simply because the existing static GUI exporter filters visibility.

Ownership: SDUI parser/normalizer and source-structure projector → SDPTool discovery/
view-generation facade → XFMD generic tabs, Markdown and existing Mermaid pipeline.
XFMD must not parse SDUI grammar. Existing AST spans are a starting point; verify
source provenance survives normalization/reuse before promising source navigation.

## Acceptance and unresolved choices

- Fixtures: multiple frame definitions, reused frames, nested rows/groups, regions,
  hidden nodes, layout overrides, Markdown literals and unresolved SDL bindings.
- Distinguish declaration and instance views and prove no lost relationships;
  deterministic node IDs/order, escaped labels and explicit missing provenance.
- Native XFMD evidence for every emitted diagram pattern, light/dark, zoom and
  PDF; source/entry navigation must identify the correct source revision.
- Host absent/unsupported profile/unbound callbacks must not be labelled fully
  runnable. Invalid source stays discoverable with diagnostics.
- Embedding trial, if selected: X11 focus/tab traversal, resize/DPI, clipping,
  shortcuts, clipboard/IME, modal windows, close/crash/restart and event loops.
  No Wayland/cross-platform claim from an X11 reparenting demonstration.
- Exact producer capabilities, launch action and source-span link format remain
  to be agreed. Markdown heading links are implemented; arbitrary source-line
  URI dispatch is not implied by that feature.

## Overlap and authority

[#003](%23003--Question--SDUI-widget-ownership.md) retains its separate question
about SDUI widget ownership. This request does not replace FOX or resurrect BoxUI.
[#014](../active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md) and
[#021](../active/%23021--Change--Dynamic-SDP-tabs-labels-and-source-opening.md)
provide existing navigation; #019 concerns shared rendering, #020 grammar fidelity,
#015 SDL .design preview. No card is superseded by this narrower SDUI source view.

[Session 0002](../../Sessions/session-%230002--Standalone-Mermaid-files.md), T006.
Owner requests investigation and backlog capture; no host embedding, new compiler
implementation or UI change has been performed. Model-source hold remains intact.

## T007 owner refinement — deferred until #025

The owner wants a more expressive/recognizable diagram than a flat block diagram.
Revisit grouped hierarchical composition and region containers; the block sketch
is an unaccepted optional example, not the selected default. Finish runtime and
on-demand fixes in the current Session before opening a new Session for this card.

## Session 0003 consolidation — 2026-10-06

Fully carried forward to [KB-XFMD-027](../active/%23027--Proposal--SDUI-preview-and-launch-workflow.md),
including unresolved ownership, embedding, provenance and readiness questions.
KBO-XFMD-000001 records this merge. Superseded is not implementation completion.
