# Audit and improve Mermaid compatibility across XFMD's interpretation and rendering pipeline

| Field | Value |
| --- | --- |
| id | KB-XFMD-020 |
| project | XFMD |
| type | Proposal |
| CardState | in-progress |
| created | 2026-09-29T23:48:33.074200+00:00 |
| source | Owner request 2026-09-30; SDP Session Gantt failure and earlier gitGraph/init report |
| next_review | N14 packet defect corrected in local candidate; retain large-print acceptance, VP08 union routing budget and broader compatibility study |
| tags | mermaid, compatibility, interpreter, renderer, diagrams, configuration |

## Owner need

Stop discovering missing Mermaid constructs one diagram at a time. Investigate
all diagram families and configuration/construct categories supported or advertised
by XFMD, identify where meaning is rejected or lost, and propose a bounded plan
for useful compatibility improvements. Include Gantt and earlier non-Gantt
failures. The owner expected raw source to be relayed to mermaid-rs-renderer;
explain and review the current responsibility boundary before selecting fixes.

This requests a compatibility study and implementation proposal, not unrestricted
execution of every Mermaid directive or an immediate rewrite. Full upstream parity
is not established or implicitly promised. Missing syntax should have an explicit
owner, disposition and meaningful acceptance evidence.

## Current pipeline — source evidence

Observed XFMD HEAD: 41fa4948c5377edd792ba3f503e6b92b0fd4717a. Inspection is not a
new native GUI test, and current working files must be rechecked at execution.

1. `src/interpreter/mermaid/rust/src/lib.rs` dispatches sequence and semantic
   families to XFMD parsers. Its remaining flowchart path uses a local profile
   inspector and upstream `parse_mermaid_strict`, then maps the graph.
2. `src/contracts/diagram/rust/` owns a typed model and wire validation; original
   Mermaid text is not passed through this model as executable renderer input.
3. `src/renderer/diagram/rust/src/lib.rs` reads that model, translates to upstream
   native diagram types and applies measured layout/rendering. XFMD supplies text
   metrics; library SVG is used in its presentation path.
4. `docs/design/mermaid-semantic-model.md` documents P38–P41's intentional split:
   semantic preservation, validated records, no reparsing across layout, measured
   labels and preview/PDF consistency. Recover applicable earlier decisions before
   changing it. The current architecture is not raw Mermaid pass-through.

Read `mermaid_coverage.md`, the record schema and existing fixtures/evidence.
FUNC-023 Mermaid Interpretation and corresponding layout/presentation blueprints
are affected. KB-XFMD-019 owns shared rendering/Kitty output and KB-XFMD-014/015
own SDP navigation/design preview; coordinate but do not absorb those scopes.

## Known reproductions and evidence limits

### Gantt configuration

```text
gantt
    title SDL expansion
    dateFormat YYYY-MM-DD
    axisFormat Slot %d
    todayMarker off
    section Proposed route
    Design :s1, 2000-01-01, 1d
    Implement :s2, after s1, 1d
```

Owner-observed error: `Unsupported Mermaid statement: axisFormat Slot %d`.
`semantic/planning.rs` accepts the exact dateFormat directive, title/section and
tasks; axisFormat and todayMarker each reach the unsupported-statement path.
Test the directives individually as well as together to avoid hiding the second
failure. The dates in this source are synthetic sequence slots, not a schedule.

The SDP session's standalone mmdr preview accepted the original source but showed
calendar ticks rather than the requested Slot labels. Acceptance without preserving
the option's visible meaning is not support. SDP removed both directives as a
local document workaround; that does not fix the compatibility gap.

### Earlier gitGraph/init case

Owner previously reported `Mermaid init directives are not supported` for:

```text
%%{init: {"gitGraph": {"showCommitLabel": false, "commitStep": 100}}}%%
gitGraph LR:
    commit id: "2cb49c0" tag: "old: 2cb49c0"
```

This is a historical owner reproduction, not a new GUI run. The exact message
exists in `src/interpreter/mermaid/rust/src/profile.rs`; semantic parsing also
rejects sources containing `%%{`. Check init handling, diagram header/direction
variants and each configuration option independently. Simply stripping init may
expose a separate unsupported header or silently lose requested behavior.

## Compatibility inventory — required study scope

Do not equate a recognized family name with complete support for that family.
Build a matrix by **family × construct/option × pipeline stage** against the
pinned upstream version actually used by XFMD. Inventory at least:

- Supported and unrecognized diagram families, header/direction variants and
  upstream version differences.
- Configuration: init/frontmatter, themes, fonts, spacing, axis/date/tick options,
  markers, display modes and family-specific configuration. Distinguish inert
  presentation settings from external links, callbacks or active content.
- Nodes/shapes, edge/arrow/relation kinds, labels/escaping/multiline text, groups,
  nesting, classes/styles and layout-relevant metadata.
- Family-specific semantics: sequence fragments/participants, state regions,
  class/ER/requirement relations, C4/architecture ports/boundaries, charts/axes,
  timeline/Gantt dates/dependencies/statuses, gitGraph branches/merges/tags and
  tree/list families. Consult the actual coverage register, not this example list
  as a claim that every item is missing.
- Rendering fidelity after successful parsing, including style/config lost by
  the contract/adapter, text measurement, labels and preview/PDF equivalence.

Each matrix row records source fixture, expected meaning, actual parser outcome,
model representation, adapter handling, upstream support and rendered result.
Use dispositions such as supported-and-verified, deliberately excluded,
XFMD-parser gap, contract/adapter gap, upstream-renderer gap, version mismatch
and unknown/unverified. Identify the appropriate owner for each.

## Architecture alternatives to assess

Compare retaining/extending typed profiles, sharing more of the upstream parser
and normalized model, and a deliberately bounded raw-render route where suitable.
Explain maintenance cost, semantic loss, text measurements, source diagnostics,
resource limits and preview/PDF behavior. The result should clarify whether XFMD
maintains a restricted Mermaid dialect and how users discover its limits.

Do not merely remove rejection checks: accepted input must retain its semantics
through the model and renderer. Do not silently ignore unsupported settings.
Preserve explicit resource/active-content policy and retain precise diagnostics.
If an upstream change is needed, create/link its owning work rather than hiding a
renderer fork inside XFMD. No new parser or renderer architecture is selected here.

## Deliverables and acceptance

1. A pinned, reproducible support matrix and gaps grouped by owner and cause,
   reusing the maintained coverage document and existing tests.
2. A disposition for the Gantt and gitGraph/init reports, with minimal positive
   and negative fixtures through the **actual XFMD pipeline**, plus independent
   upstream comparisons where useful.
3. A bounded Design/ImplementationPlan selecting high-value missing constructs;
   explicit exclusions and remaining work. Follow XFMD's requirements → blueprint
   → contracts/plumbing → code → verification workflow.
4. For implemented improvements: assert retained model/config values and inspect
   generated output in XFMD preview and PDF at relevant sizes. Standalone mmdr
   exit success is not enough. Cover failures, unsupported input and regressions.
5. Update current compatibility documentation and errors so users can distinguish
   XFMD profile restrictions from upstream renderer limitations.

No product implementation, full compatibility audit, native GUI/PDF verification
or architecture change has been performed by this card's preparation.

## External preparation record

Project SDP; Session prototype and KB-SDP-042. These are external references,
not resolvable dependencies for XFMD's board validator. The preparation source is
`SDP/02--Requirements/XFMD-Gaps/XFMD-Mermaid-Compatibility-Card-Draft.md` in the
SDP repository. Do not require another local checkout for the adopted card.

## Worklog

- 2026-09-29T23:48:33.074200+00:00 — EVT-KB-XFMD-000084: Registered owner-requested compatibility audit/plan proposal from external SDP handoff. Registration does not select product implementation; CardState backlog.

## Session 0003 owner feedback — 2026-10-06

Add the [original VP07 ConfigurableScrolling fixture](../evidence/2026-10-06-session3/VP07-ConfigurableScrolling.mmd)
and [exact reported error/inspection](../evidence/2026-10-06-session3/README.md)
to this compatibility study. The reported libavoid non-orthogonal edge is a
rendering failure, not yet a diagnosed parser or model error. Retain this exact
fixture while minimizing; require actual preview/PDF reproduction and a regression
for retained nodes, relations and labels before claiming a fix. Do not silently
switch routing engines, drop edges or hand-edit generated source as acceptance.

Owner also requires proper use-case notation for #028. Official Mermaid now
documents usecase-beta (12.0.0+). Assess and implement its needed semantic subset
through XFMD's native parser, typed contract and renderer; do not advertise support
based on the upstream JavaScript site alone. Preserve actors, use cases, boundaries
and genuine include/extend/generalization semantics; never invent those relations.
This is a scoped priority input to the existing study, not a claim of full parity.

[Session 0003](../../Sessions/session-%230003--SDL-navigation-and-SDUI.md) puts
navigation cleanup first, with renderer work needed before viewpoint acceptance.

## SDUI native inspection — Session 0004 T002, 2026-10-07

Nested independent flowchart groups exposed an ID-order mismatch in the native
inspection adapter. 0.7 fixes ID mapping and adds DiagramLayoutTest coverage.
A separate visual limitation remains: nested groups with identical membership
can overlap labels/boxes in the upstream containment layout. Reproduction:
`tests/fixtures/mermaid/sdui-nested-containment.mmd`. SDUI uses readable shallow
composition mindmaps with separate container details instead. Carry the nested
flowchart geometry investigation into the later SDL diagram Session; do not
claim this is fixed by the ID-order correction.

SDL diagram successor: [Session0005](../../Sessions/session-%230005--SDL-viewpoint-diagrams.md), created after SDUI0.7 delivery. Recover live producer contracts before N3/N4 execution; no new completion claim.

## Session0005 T001 execution

Owner selects the diagram round. Start PLAN-XFMD-0009 N3a with exact native
reproduction and producer-contract recovery; fix only a diagnosed bounded defect.
Preserve the staged pre-session card snapshot in Git; no index reset or broad
commit. Broader Mermaid compatibility and viewpoint work remain explicit.

## Session0005 T001 result — 2026-10-08

[N3a-001](../../05--Implementation/SDL-Navigation/evidence/N3a-001/README.md) records
exact native/PDF reproductions and live producer boundaries. Quoted mindmap labels
are fixed in XFMD with22 parser and12 affected CTest passes. VP07 has a verified
local fork correction (e79d8f9caf649995a5db5e54b0ea5d6b7fab8221),13 fork-test passes and an inspected C++ native/PDF
candidate retaining12 nodes/18 edges. It is not yet adopted into the normal source
pin or installed binary; do not call installed VP07 fixed. Nested containment,
Gantt/gitGraph options and broader matrix/use-case support remain open. One optional
label leader remains omitted with the existing diagnostic. Card stays in-progress.

## Session0005 T002 result — 2026-10-08

[N3b-001](../../05--Implementation/SDL-Navigation/evidence/N3b-001/README.md) adopts
the VP07 fork into the normal build using exact bundled source and commit/SHA-256.
13 affected CTests and inspected native/PDF pass; all 12 nodes/18 edges retained.
Bootstrap integrity/refusal and remote compatibility are tested. Installed0.7 is
unchanged; no public fork publication. Renderer adoption is complete, while card
scope stays in-progress for remaining notation/layout/compatibility work.

## Session0005 T003 result — 2026-10-08

[N3c-001](../../05--Implementation/SDL-Navigation/evidence/N3c-001/README.md)
delivers native use-case notation in the working Release candidate: actors,
ellipses/rectangles, system boundaries, associations and explicit UML relations.
Fourteen fork tests and 13 affected CTests pass with native/PDF inspection. Initial
detached actor edges were rejected and corrected before this result. Installed0.7
is unchanged. The card remains in-progress; aggregate VP01/VP07 and exact SDL
provenance remain producer work. See [handoff](../../04--Design/SDL-Navigation/Producer-Handoff.md).

## Session0005 T004 PDF discovery

N3d delivers the first VP01 overview through SDL projection, SDPTool overviewTarget
and native XFMD activation. Complete actor/use-case inventory includes isolated
items; functionality realization stays separate. Real XFMD source retains one
actor, 11 use cases and 11 pursues relations. Native inspection selected LR after
rejecting the unreadably wide TB fan-out. Query metadata stays in selection.json.
The tall real diagram also exposed a one-ULP paged-height overshoot; DiagramPlacement
now bounds the final height and has a synthetic plus real PDF regression.
Go viewpoint/documents and SDPTool facade tests pass. Native and affected consumer
checks pass, including Enter/click, expansion-only, older/empty producer fallback,
stale/foreign targets and PDF export. The first new GUI attempt lacked explicit
X11 focus; its failing log is preserved alongside corrected passing evidence.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3d-001/README.md). Installed0.7 unchanged. Remaining VP07/detail/provenance work stays in Session0005.

## Session0005 T005 — N3e

N3e implements lazy VP07 logical overview and named-mode targets. All declared
System/container/unit structure is retained. Feature keys annotate real owners and
selected-mode containers through proven Functionality paths; the legend and table
retain all Features, contributions and allocation gaps. No cross-mode union or
invented direct relation. Native Enter/selection exercises both mode targets.
Ten affected CTests pass (five GUI); SDL viewpoint/documents pass and the SDPTool
facade suite passes with one documented pre-existing SDUI test excluded. That test
also fails in an isolated copy with N3e edits removed: UIPreview uses Combined,
while the all-family test compares against Markdown. Do not claim full producer
suite success or change another workstream's acceptance in this increment.

The literal graph timed out; the adopted annotation projection renders the real
XFMD model (23 architecture nodes, 19 contains edges, 140 proof annotations and
137 facts) and exports an eight-page vector PDF. Small-fixture rendering is readable.
Real-model full-view labels remain small at fit-to-width / A4. Grouping disconnected
nodes was explored but not adopted: it improved native width while worsening PDF
pagination. Large-overview reading is still an acceptance gap owned by KB020/028;
no overall visual acceptance or Session completion is claimed.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3e-001/README.md). Remain active for the named successors.

## Owner feedback — Session0005 T007, 2026-10-09

Owner reports VP07 DocumentReferences detail failing with Libavoid NoSpace:
label attachment intrusion on edge 11. Reproduced through the same candidate
SDPTool target, normal native renderer and PDF probe: zero native diagram scenes;
PDF contains the exact error. This is a 12-node/15-edge detail graph, separate
from the newly added VP07 aggregate and its fit-to-width reading gap.

The strict renderer check in layout/routed/labels.rs rejects another edge crossing
the attachment zone between an edge and its label; the error is not an SDL mode
validation failure or invalid Mermaid declaration. One bounded expanded-spacing
retry already exists and is insufficient for this fixture. Root cause within label
placement/obstacle routing is not yet established. Preserve strict geometry,
source semantics, fixed candidate pin and existing original routing regression.
Next implementation boundary: reproduce with exact text metrics in the Mermaid
fork, correct attachment routing/placement, then test original and new fixtures
through native preview/PDF before dependency adoption. No guard removal or source
model rewrite is selected as a workaround.

NormalDesktop is the model's sole declared mode (XfmdDesktop.design line 180),
used by allocations, channel participation and scenarios. It denotes ordinary
standalone desktop execution; the name has no built-in runtime switch semantics.
Selecting it in VP07 filters modeled allocation facts, not the running app's mode.

[Reproduction](../../05--Implementation/SDL-Navigation/evidence/N3f-001/README.md). Cards remain active; no repair or approval claimed.

## N3f delivery — Session0005 T009

N3f repairs the reported DocumentReferences VP07 detail by using actual measured
caption extents to bound the existing single expanded-placement retry. Node/edge
semantics, strict attachment validation, four label orders, three route transactions,
engine selection and deadlines remain unchanged. Fork commit
04c6ea7146dab35ef5fca0c204e65763e52f99d7 is adopted through a checksum-pinned
repository archive; all 1,292 regular files match the clean fork and extracted input.

Fifteen affected fork integration tests and three strict attachment unit tests pass.
The normal Release build passes 14 selected CTests, including six native GUI tests.
Actual native/PDF inspection confirms the reported diagram renders all 12 nodes and
15 edges; its vector PDF has one page and all declaration labels are present.

A sweep of all ten existing XFMD VP07 feature details renders nine successfully.
ChapterNavigation still fails with an ambiguous label on edge7; the identical
failure reproduces in the pre-fix normal probe. This is an existing additional
case, not a passed gate or a regression introduced by the new spacing rule.
Prioritize that fixture next, then large-overview reading, focused use-case/scenario
projections and N4 exact source navigation. KB020/028 and Session0005 remain active.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3f-002/README.md). Owner confirms preserving NormalDesktop; no model edit.

## N3g delivery — Session0005 T010

ChapterNavigation caption ambiguity is repaired in the pinned renderer fork.
The ten-detail VP07 sweep now passes10/10; native/PDF inspection and all targeted
checks pass. [N3g-001](../../05--Implementation/SDL-Navigation/evidence/N3g-001/README.md)
contains exact evidence. Remain active for large-overview reading and the existing
viewpoint/detail/source successors. NormalDesktop and authored-model hold persist;
installed 0.7 is unchanged.

## Local 0.8 installation — Session0005 T011, 2026-10-10

The verified working candidate through N3g is installed as 0.8; full 91/91 CTest and
installed smoke checks pass. [R08-001](../../05--Implementation/SDL-Navigation/evidence/R08-001/README.md)
records the exact package and backup. Existing residual scope remains active;
installation does not close the card or claim independent/public acceptance.

## Owner pause — Session0005 T012, 2026-10-10

Session0005 is paused in favor of [Session0006](../../Sessions/session-%230006--Discoverable-SDPTool-actions.md) / KB030.
CardState in-progress → ready records the paused handoff; active placement is
retained. Resume only when the owner resumes the diagram Session. The 0.8 delivery
and remaining obligations are unchanged; no completion is implied.

## Session0005 T013 — resumed

Owner resumes diagram work while KB030 waits for its API release. N3h selects
opt-in full-size diagrams with existing horizontal scrolling/zoom in Wrap.
A4/PDF readability and remaining projection/source work stay open.

N3h outcome: full-size continuous diagram reading is built and verified (19/19
affected CTests, 11 GUI, plus actual VP07 native workflow). See
[N3h-001](../../05--Implementation/SDL-Navigation/evidence/N3h-001/README.md).
Default fit and A4/PDF remain; large-overview print readability and remaining
projection/source scope are not complete. Installed0.8 unchanged.

## Session0005 T021 — N4 implementation and residual handoff

[N4-001](../../05--Implementation/SDL-Navigation/evidence/N4-001/README.md)
delivers focused use cases/explicit scenarios and original composed-source spans.
Fixed 24/48 diagram-count caps are removed, with 50-diagram coverage. Producer
commit fde2239 follows ecd9dcd on the main-based successor; full producer/native
checks pass. Main/PG merge forecasts are clean; no merge/release/install.

[Current disposition](../../04--Design/SDL-Navigation/Viewpoint-Disposition.md)
records exactly what this card still owns. Session implementation closure does
not complete this broader card or waive large-print, compatibility, remaining
resource limits, integration or review obligations. CardState is unchanged.

## Session0007 T006 — VP08 all-channel routing budget

Initial VP08 NormalDesktop union (60 nodes/123 grouped edges) fails native rendering with `Mermaid: Libavoid: BudgetExceeded`. N10 delivers all channel neighborhoods separately, preserving the 60 unique nodes and 123 edges across 40 diagrams. The original union remains a useful renderer fixture; no general routing/budget fix or large-print acceptance is claimed.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N10-001/README.md

## Session0007 T008 — VP10 narrow packet labels

N12 native/PDF inspection exposes a renderer readability defect in existing
packet notation: the 1-bit PacketFlag field clips its long label, while the
adjacent 12/13 boundary numbers run together. A 29-bit fixture uses Flag 0:1,
Number 1:12 and Payload 13:16. Producer offsets and source references are correct;
overview and original detail emit identical packet syntax. The complete source-
linked field register supplies names, ranges and widths, but does not close
visual diagram acceptance. PDF text extraction retains names despite visible
clipping, so text retention alone must not be used as visual evidence.

Reproducer/evidence: N12-001/native/vp10-generated.md, vp10-source.design.txt,
vp10-native.png/pdf and vp10-detail.png/md under SDL-Navigation evidence.
Follow up in the packet renderer: accommodate narrow-field labels (external
legend/callouts or another layout preserving bit proportions) and separate
neighbor boundary labels. Verify screen and PDF at ordinary reading sizes,
including 1-bit fields, long names and adjacent multi-digit boundaries. No
renderer implementation or independent visual acceptance is claimed in N12.
Existing CardState and publication/large-print obligations remain unchanged.

## Session0007 T010 — N14 packet correction

N14 corrects the N12/KB020 packet renderer defect. The common measured bit
pitch preserves proportions, places endpoint numbers at their bit centers using
the actual SVG font, and separates adjacent multi-digit numbers. Overflow names
use numbered in-cell markers and full per-row legends with exact bit ranges.
Row-spanning fields retain the same ordinal. No producer source rewrite or ABI
change. Renderer commit 49bb0ee is locally committed and adopted via a checksum-
pinned archive; fork publication is separate.

Seven focused fork tests and 24 affected CTests pass (nine GUI tests), including
all existing diagram family/PDF and SDP overview/source/runtime regressions.
Native measured SVG, standalone native Preview and two one-page vector PDFs were
inspected; all field names are visible and the 12/13 PDF glyph boxes have a 5.55pt
gap. Original generated VP10 overview/detail workflow passes with full labels.
The development executable is built. Installed XFMD 0.8 and gh sdp are unchanged.

N5–N13 producer stack 486b29d has clean committed-tree merge forecasts against
main 5ca0e90 and PG 6dfde97. This does not establish independent semantic review
or authorize integration. Neither producer nor PG was changed. Broad KB020
large-print/VP08 routing acceptance, KB025 whole-project discovery/resource guards,
and KB032 eager-export alignment/integration remain open. Proposed next bounded
work: KB025 discovery of the growing project, starting with source/inventory policy.
No authored SDL/SDUI edits, merge, release, install or independent approval.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N14-001/README.md).
CardState remains in-progress; the N12 narrow-label/number defect is resolved in
the tested development candidate, not yet in installed 0.8.
