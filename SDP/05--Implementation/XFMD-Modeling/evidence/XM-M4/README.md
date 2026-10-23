# XM-M4 — Whole-inventory SDL baseline and feasible SDUI

Date: 2026-09-30. Parent: [MAINT-XFMD-0004](../../../../Maintenance/MAINT-XFMD-0004.md).
Candidate: XFMD `41fa494` plus the source/model/document hashes recorded here.
Method: same-context source/blueprint inspection, actual SDL/SDUI tooling,
authority-based model checks, deterministic regeneration and static visual review.
No native XFMD/Fyne execution or independent review is claimed.

[Candidate file hashes](candidate-files.json) identify the current uncommitted
modeling artifacts. Earlier candidate manifests remain historical.

## Delivered scope

- Current authored input: [XfmdDesktop.design](../../../../SDL/XFMD/XfmdDesktop.design).
  308 declarations and 1,027 facts; all 10 existing Features, 27 Functionality
  blueprints and 10 current use cases mapped. Future UC-006 and BoxUI reservations
  remain excluded from implemented scope. Counts do not prove full semantics.
- 22 declared Channel/scenario examples, including NAV-01–12 and selected preview,
  resource, scroll, preference, PDF, index, diagram, external-open and fullscreen
  workflows. These are abstract design observations, not executable C++ or new
  wire payloads; [semantics and source boundaries](../../../../04--Design/XFMD-Modeling/Model-Semantics.md)
  identify what each example does and does not establish.
- Three bounded state records and named existing collaboration interfaces.
  They do not replace complete native types or enforce all conditions/lifetimes.
- Five SDUI concepts in [Desktop.sdui](../../../../SDL/XFMD/SDUI/XfmdDesktop/Desktop.sdui):
  split/editor/preview workspace, preferences and publication. Editor and viewer
  are labeled areas. Buttons are unbound; no file loading/editing/saving is claimed.
- [Generated view index](../../views/xfmd-model/sdl/index.md), including tool-owned
  Mermaid source and Markdown. No external Mermaid SVG renderer was invoked.

The earlier Navigation.design/Navigation.sdui pilot and all XM-M2/XM-M3 evidence
remain unchanged as historical fixtures. The current model uses one supported
input until a real System/source-set contract is selected. It is not a custom
concatenated or experimental System model.

## Evidence and reproduction

[Source manifest](source-manifest.json) pins inspected source/blueprint inputs
and producer Fyne/profile inspection. [Generation provenance](../../views/xfmd-model/provenance.json)
pins authored inputs, binaries, commands and 243 output hashes. The binaries
remain those built from tracked producer `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`
under XM-M2; the newer inspected producer commit does not relabel that build.

From the XFMD repository root, supply existing binaries matching the XM-M2 hashes:

```sh
python3 SDP/02--Requirements/XFMD-Modeling/build_inventory.py
python3 SDP/05--Implementation/XFMD-Modeling/render_model.py --sdl /path/to/sdl --sdui /path/to/sdui
python3 SDP/05--Implementation/XFMD-Modeling/verify_model.py
python3 SDP/05--Implementation/XFMD-Modeling/verify_pilot.py
python3 SDP/KanBan/verify_board.py
python3 tools/validate_blueprints.py
python3 tools/check_blueprint_symbols.py
git diff --check
```

`verify_model.py` compares real SDL AST facts with blueprint owners and Feature
uses, all current use-case identities, the 71 UR/SR mapping, declared scenario
steps and selected ordering invariants. It verifies current source/output hashes
and well-formed SVG. It does not parse SDL itself or establish native conformance.

[Reproduction](reproduction.json) records a fresh-directory generation with all
243 output hashes matching. Provenance command/output-directory paths intentionally
differ between runs. [Checks](checks.json) records the final validation commands.
[Ledger prefix](ledger-prefix.json) pins the bytes preceding this continuation's
seven progress events, including the concurrently created #020 event.

[Record checks](record-checks.json) confirm link resolution, deterministic inventory
and preservation of the ledger prefix. The initial blueprint link check ran before
checks.json was written; [initial results](checks-initial.json) retain that failure.
The affected check passed after its referenced report existed.

Authoring checks initially rejected messages without payload contracts. The final
model explicitly uses a design-observation context record, not fabricated runtime
signatures. SDUI layout initially rejected an indefinite input-row width and a
narrow sidebar. Explicit relative sizing fixed both. Long explanatory prose split
words under the existing SDUI wrapping limitation; concise region labels avoid
that in the final workspace compositions. No producer implementation was changed.

## Visual inspection

All five 1440×900 SVG exports were rasterized with rsvg-convert and inspected.
The final compositions show distinct source/preview regions, readable controls
and visible prototype disclaimers. They are conceptual views, not native screenshots
or pixel-fidelity checks. Existing typography/list fidelity findings remain open.

[Split](SplitWorkspace.png), [editor](EditorWorkspace.png),
[preview](PreviewWorkspace.png), [preferences](PreferencesConcept.png),
[publication](PublicationConcept.png).

## Remaining completion boundaries

All blueprint identities are structurally represented. Complete normative text,
all branch/interleaving/state predicates, exact native payloads and bindings are
not encoded. Full source/test correspondence and independent review remain open.
In particular, dedicated current native evidence for NAV-05 (Save then failed read)
and NAV-10 (invalid selection during valid generation) is not established here.
SDL-001/002/003 and relevant documentation/evidence findings retain those limits.

The latest owner instruction accepts SDUI placeholders; advanced widget/editor/
viewer parity is not a completion gate. A basic multiline input is recorded as
a producer proposal, with Fyne support and current SDUI host boundaries inspected.
This checkpoint does not close #017, implement #014/#015, install, commit or publish.
