# XM-M3 bounded modeling pilot — 2026-09-29

Parent: MAINT-XFMD-0004. Work: PLAN-XFMD-0001–0005 / KB-XFMD-017.

## Delivered scope

- [Requirement reconciliation](../../../../02--Requirements/XFMD-Modeling/Coverage.md)
  assigns all 71 table-defined UR/SR identities to concerns, records the important
  later amendments and maps all source families. This does not close all acceptance
  or function-call review.
- [Architecture decisions](../../../../03--Architecture/XFMD-Modeling/Decisions.md)
  select supported, bounded inputs; no unsupported System/import declarations or
  new parser/host are introduced.
- [Workflow contract](../../../../04--Design/XFMD-Modeling/Navigation-Workflow.md)
  describes NAV-01–12, including dirty admission, generation, replacement and leases.
- [Model sources](../../../../SDL/XFMD/README.md) supply a structural SDL pilot
  mapped to 14 functionality and three feature identities, and three static SDUI
  shell states. Twenty remaining blueprints have no SDL definition yet.

## Executed checks

At XFMD HEAD `41fa494` plus the recorded uncommitted artifacts:

```sh
python3 SDP/05--Implementation/XFMD-Modeling/render_pilot.py --sdl /tmp/xfmd-modeling-0xt1ad54/bin/sdl --sdui /tmp/xfmd-modeling-0xt1ad54/bin/sdui
python3 SDP/02--Requirements/XFMD-Modeling/build_inventory.py
python3 SDP/05--Implementation/XFMD-Modeling/verify_pilot.py
```

Results: SDL check/canonical format/AST and VP01/VP02/VP07 generation pass;
all three SDUI entries export AST, dump and SVG. The identity check accounts for
71 UR/SR IDs and 17 mapped blueprint IDs. Source/output hashes match. A second
generation into `/tmp/xfmd-pilot-reproduction` reproduced **all 52 output hashes**.
The generator verifies supplied binary hashes against XM-M2 before attributing
them to the pinned producer commit. No build/install/producer mutation is performed.

The first coverage check caught missing UR-029; it was added to keyboard/sidebar
responsibilities. Initial SDUI authoring attempts exposed definite-size requirements
and a too-wide text header; the authored composition was corrected within the
supported layout contract. These were model-authoring corrections, not tool fixes.

All three SVG states were rasterized with `rsvg-convert` and visually inspected:
[Files clean](FilesClean.png), [dirty delivery rejected](FilesDirtyRejected.png),
[Index clean](IndexClean.png). Files controls are absent on Index; the dirty state
shows Path * and the actual delivery-rejection message. Favorites have grouped
sample entries and no Add/Remove buttons. Refresh uses R as a documented glyph
fallback; prototypes do not claim native widget/style fidelity.

The existing SDL/SDUI gap baseline remains open. Visual inspection added
GAP-XFMD-SDUI-011; source tracing added GAP-XFMD-EVID-002. The register now has
20 entries. Generated SDL views remain tool output, not hand-edited diagrams.

## Candidate and limits

Generated-source/tool/output provenance:
[provenance.json](../../views/navigation-pilot/provenance.json).
Additional plan/model/evidence hashes: [candidate-files.json](candidate-files.json).
XM-M2's old candidate manifest remains historical; it is not a claim that the
now-expanded inventory/plans still have their old hashes.

No native application behavior test or independent review ran. The SDL model
does not encode ordered behavior, and SDUI has no live tree/editor/callbacks.
The static sample omits some full-shell controls; this is representative workflow
modeling, not a pixel-equivalent FOX replacement. Full System/source-set support,
remaining requirements/blueprints and acceptance evidence remain outstanding.

This session has read-only Git metadata. Changes remain uncommitted; no milestone
commit, push, merge, release or #017 completion is claimed. Existing concurrent
installation/backlog edits and prior ledger bytes are preserved.

## Repository checks at this checkpoint

- KanBan replay/schema/index: PASS, 19 cards, 82 card events, nine management records.
- Blueprint structure/matrix/local links: PASS, 37 blueprints, 71 UR/SR definitions.
- Blueprint symbol check: PASS, 285 implemented callees; call semantics not inferred.
- Whitespace check: PASS. The ledger prefix captured before this checkpoint is unchanged.
- Product sources are unchanged. Inventory generation remains deterministic on the current inputs.

The initial repository-link check ran before the new candidate hash manifest had
been written and reported that missing target; the final check includes it.
