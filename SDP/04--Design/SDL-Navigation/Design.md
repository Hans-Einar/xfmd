# SDL navigation refinement — Session 0003

Status: N2 is installed; N3/N4 native/projection/source implementation is delivered
in the development candidate. See [current disposition](Viewpoint-Disposition.md)
and N4-001 for tested scope and explicit remaining publication/projection work.
Installed gh remains unchanged.
Dated implementation checkpoints below retain their original scope. Owner #028 is the outcome.
No authored SDL/SDUI models are edited under the existing #022 hold.

## Outcome and current evidence

Default tree: **SDL → discovered System/source → viewpoint → detail**. Show a
viewpoint once in its source context. Remove phase and abstraction presentation
wrappers from that route; retain their meaning in producer facts and diagnostics.
Keep producer System/source grouping and invalid-source entries, even if a source
is an old fixture. Hiding fixtures is a separate discovery policy, not this cleanup.
Do not hardcode VP01–VP11 or derive identity from labels/path fragments.

The inspected installed discovery snapshot has 33 canonical viewpoint nodes across
three valid models, plus repeated references through phase/abstraction groups.
SDPTool/tree.go creates those wrappers and references. SdpPanel::add resolves them,
then appends reference/depth-limit leaves when the ancestry guard fires. It also
uses the resolved object's target, although a reference may carry its own focused
viewpoint target. This can replace the intended viewpoint with generic VP11.
These are source/JSON findings, not a new native regression result.

Alternatives:

| Approach | Decision and reason |
| --- | --- |
| Change only producer tree immediately | Desirable eventual default upstream, but ties useful native cleanup to an installed producer upgrade |
| Normalize known presentation wrappers in the consumer | Selected bounded first increment; consumes node kinds/IDs, preserves semantics/actions, supports current and future producer trees |
| Reconstruct viewpoints from SDL in XFMD | Rejected: duplicate language/projection ownership and unnecessary coupling |
| Remove every reference or globally deduplicate objects | Rejected: loses valid cross-links, per-viewpoint context and independent Systems |

## Display projection and responsibilities

SdpDisplay is an application-owned projection over an immutable SdpReply.
It prepares **only requested children**;
SdpPanel owns FOX row creation. SdpProtocol remains wire validation, SdpSources
source association, and SdpNavigation process/document ownership.

A projected row needs distinct origin node ID, resolved node ID, stable occurrence
key, label/state/diagnostic, source association, effective typed action and ancestry.
A stable occurrence key incorporates displayed parent context and origin identity;
a canonical viewpoint uses source context + resolved viewpoint ID so alternative
A-level routes collapse. Canonical IDs alone must not merge different occurrences
whose actions differ. This is a presentation record, not another source registry.

Algorithm for a source/System's requested child list:

1. Follow graph edges supplied by discovery. Treat healthy `phase`/`abstraction`
   nodes beneath SDL source scope as transparent wrappers. Collect reachable
   viewpoint rows in producer encounter order; deduplicate resolved viewpoint ID
   only within that source context. Do not flatten KanBan, Sessions or SDUI trees.
2. Preserve unfamiliar group kinds and any wrapper diagnostic/status requiring
   attention. A diagnostic is exposed as diagnostic text/status, not thrown away
   with the wrapper. Preserve existing directly supplied viewpoint children.
3. Resolve references with visited IDs. Prefer the origin reference's explicit
   target when present; use the canonical target only when absent. Preserve the
   origin's open target for KanBan Ref cards. Conflicting source associations
   disable View source rather than choosing one. Do not synthesize URIs.
4. Separate action availability from further expansion. A valid reference back to
   an ancestor can remain a clickable terminal cross-link using its own target;
   do not recursively copy its subtree. Drop only inert cycle/depth placeholders.
   For a genuine depth truncation with unseen content, keep any valid action and
   report the incomplete expansion in the status tooltip. Do not claim all content
   is shown. Invalid model/tool diagnostics remain visible.
5. Expansion uses projected children and independent ancestry. Refresh restoration
   searches display identities/routes, not hidden A-level paths. Preserve source,
   tab, expansion and selected occurrence; fall back to surviving source/viewpoint
   when the target disappeared. Cancel queued activations before replacing rows.

Existing global row/protocol limits remain #025 obligations; N2 must not silently
turn them into invisible omissions. Count displayed rows, not skipped duplicates;
show a visible truncation status where the existing guard prevents rendering.
Removing the guard without replacing its resource behavior is not part of N2.

## Activation versus expansion

| Interaction | Final behavior |
| --- | --- |
| Expansion box / keyboard Right on a branch | Expand/collapse only; no producer generation |
| Viewpoint label activation / Enter | Generate its explicit aggregate overview target |
| Detail label activation / Enter | Execute that row's original typed detail target |
| Valid reference leaf | Open its own contextual target, without recursive subtree copying |
| Inert cycle marker | Not displayed |
| Missing overview on older producer | Interim N2: keep expand behavior with a clear unavailable-overview status; never call broad selection as an alleged overview |

Check native hit testing: NavigationTree currently queues action on clicked rows.
Tests must distinguish expansion-box clicks from label clicks; patch only the
relevant SDP/native interaction if FOX notification behavior conflates them.
Keyboard Left/Right must retain normal tree traversal; Enter selects overview.

N2 is a useful tree-only delivery, not completion of the owner's overview request.
The complete activation behavior becomes available with N3's producer contract.

## Overview producer contract — N3d selection

N3d adopts an optional `overviewTarget` to a `kind: viewpoint` discovery node, using the
existing typed-target fields and validation rules. Absence means not available;
a broad existing target is not evidence of an aggregate overview. Consumers that
ignore the new field keep current behavior. The operation remains `select`
with an explicit producer-owned diagram/query identifier, model and revision;
XFMD passes the returned URI verbatim. The producer defines and validates its
identifier and scope, not XFMD string manipulation.

SDL owns each overview projection from a complete validated source snapshot.
SDPTool advertises `overviewTarget` only when the selected projection exists, and
returns a generated Markdown bundle with the overview, scope/mode, detail links,
source references and genuine missing-model facts. Discovery must not generate
all detail outputs. Selection generates the requested overview only. Large-model
handling must reconcile #025's fixed count/file/byte limits before claiming complete
coverage; one giant unsupported graph is not an acceptable workaround.

N3d adopts this optional field in the working producer for VP01; the installed
gh extension is unchanged and VP07 is not yet advertised. Contract tests must cover old
and new producers/consumers, malformed targets, foreign project and stale revision.
The adopted shape is recorded in the producer Contract.md before code.

## Viewpoint disposition matrix

Below are proposed aggregate views within the owner-selected direction. Only
VP01/VP07 outcomes and the general need for meaningful other views were explicitly
requested; these other notations are design recommendations, not fabricated approval.
Use existing catalog titles from discovery. Do not assign A-levels to every row.

| Viewpoint | Overview question and proposed presentation | Detail / model evidence and limits |
| --- | --- | --- |
| VP01 | Who needs which behavior? Proper use-case diagram of all actors/use cases in selected scope; supporting functionality realization shown in a clearly separate companion section | Use-case detail + related explicitly linked scenarios; `pursues`, `supports`, `contributes-to`. Do not invent UML include/extend relations or System boundary |
| VP02 | What is the full logical architecture? Containment diagram covering declared System/container/unit hierarchy | Selected subsystem decomposition; `contains`, not deployment/allocation inferred from ownership |
| VP03 | Who owns/provides responsibilities and capabilities? Architecture annotated with responsibilities; readable legend | `owns`, `realizes`, `provides`; capability and feature stay distinct |
| VP04 | Which declared interfaces collaborate? Interface/provider/consumer graph | Contract/interface detail; consumption does not prove a provider, transport or call order |
| VP05 | What depends on what in this mode? Dependency graph with explicit mode scope | Focused dependencies and mode selection; never union incompatible modes or infer inheritance |
| VP06 | How is work related and delivered? Activity/dependency graph plus status table | Work item detail; `refines`, `addresses`, `delivers`, `depends-on`; no calendar dates inferred |
| VP07 | Where do features apply across the complete architecture? Architecture base with feature overlays/legend and mode scope | Feature realization slice from `contributes-to`, `owns`, `allocated-to`; show unallocated contributions explicitly, not invented placement |
| VP08 | What channels/scenarios are declared? Channel/participant topology and scenario index | Existing producer sequence diagrams from ordered validated scenario steps; no all-scenarios synthetic timeline |
| VP09 | What data contracts/storage relate? Data ownership/contract graph | Dataset/datagram/database/field/variant detail; ER notation only when relational semantics are modeled |
| VP10 | How is a datagram encoded? Encoding inventory and selected packet layouts | Validated bit positions/closed contracts; no packet geometry for missing encodings |
| VP11 | What is known, traced or missing? Source/provenance coverage and model-gap summary with linked relation views | Declaration/fact/source detail, not a single unreadable graph of every fact |

VP01: the current generator makes per-use-case and per-feature flowcharts, not an
aggregate use-case diagram. #020 must establish proper native use-case support
across interpretation, typed model, preview and PDF before acceptance.

Sequence reuse: channels.go already emits VP08 sequenceDiagram from validated
scenario steps. Link only scenarios that the source explicitly associates with
the selected use case; no same-name heuristic. Missing association is a model gap.
Pre/postconditions require explicit semantic facts or a documented deterministic
rule with supporting source IDs. Generic properties are not automatically conditions.
Represent absent evidence as not specified; do not infer behavior from prose labels.

VP07: the current generator produces per-feature/per-mode graphs and reports
UNSPECIFIED_ALLOCATION. Aggregate architecture/overlay composition is new producer
work. Preserve the original reported libavoid fixture in #020 until a real fix is
verified; new styling cannot serve as evidence that the routing defect is repaired.

## Source references and retained invariants

#021 owns the revision-aware source-span link contract and exact editor selection;
link it, do not claim line highlighting from existing View source. A projection
must retain declaration/relation provenance and source revisions even before the
consumer can highlight ranges. Missing provenance is explicit, never a guessed line.

Keep dirty-buffer transactions, cancellation/stale-result checks, native history,
dynamic tabs, invalid source diagnostics, runtime storage and bundle lifetimes.
No fixed viewpoint list, copied SDL parser, background generation on expansion,
or runtime launch when viewing diagrams. SDUI #027 and toolbar #029 remain later.

## Verification contract

Projection fixtures: duplicate wrappers, direct/unknown groups, two models sharing
labels, cyclic actionable/nonactionable references, deep finite data, source conflicts,
invalid diagnostics and differing origin/canonical targets. Assert semantic output
and target identity, not implementation container layout. Native tests verify all
click/keyboard distinctions, refresh/history/source actions, real producer labels,
no generation on expansion and explicit status for unavailable overviews.

Producer fixtures cover overview completeness against source facts, scope/mode,
25+ use cases, isolated actors/features, empty data, real declared sequences,
missing conditions/associations and preserved source spans. Native/PDF acceptance
must inspect actual intended notation, not just successful generation. #020 routing
reproduction and #021 exact-source tests remain separate evidence dependencies.

## N2 implementation checkpoint — 2026-10-07

SdpDisplay implements child IDs and resolved row data in separate helpers;
SdpPanel/SdpItem assign stable display-parent occurrence keys. Native tests verify
context-specific target dispatch, deduplication, source menus, expansion-box versus
activation, refresh, real generated views and retained runtime behavior. See
[N2-001](../../05--Implementation/SDL-Navigation/evidence/N2-001/README.md) for
88/88 tests and installed 0.6 identity. Producer overviewTarget remains a proposal;
no N3/N4 implementation or model-source conformance is implied.

## N3b dependency adoption — Session0005 T002, 2026-10-08

Adopt the tested fork commit through an exact repository-contained source archive,
recorded with commit and SHA-256 in `cmake/mermaid-source.json`. The fork continues
to own routing; bootstrap extracts unmodified source only after checksum and
Libavoid verification. Retain existing mismatch refusal and remote archive support.
This local source transport allows a reproducible normal build without publishing
an unrequested upstream branch or relying on another checkout. A .deps patch or
absolute local dependency would violate the existing ownership/reproduction rule.
The complete archive preserves licenses and upstream source identity, at the cost
of repository size. This is a build-input choice, not a new product behavior.

A two-point diagonal Libavoid no-route result remains invalid geometry, classified
as NoSpace to enable the existing single fresh-placement retry. Frozen positions,
deadlines, malformed multi-segment rejection and explicit engine selection remain.
Verify the original 12-node/18-edge VP07 fixture through the normal parser, measured
layout, native display and PDF. Keep nested-group and producer overview gaps open.

## N3c native notation — Session0005 T003, 2026-10-08

The [use-case profile](UseCase-Profile.md) selects the additive family23 contract,
strict parser subset and native glyph/routing responsibilities. It is the consumer
notation prerequisite for VP01; it does not supply producer aggregate targets.
The [producer handoff](Producer-Handoff.md) records the next concrete contract and
projection work, including the existing explicit Scenario `exercises` relation.
Authored model sources remain held; no unsupported relation or condition is inferred.

## N3d execution refinement — Session0005 T004

Select VP01 as the first complete vertical overview. The producer now adopts
optional overviewTarget and SDL query `projection=overview` (rather than a reserved
diagram ID that could collide with authored names). XFMD validates select/project/
model/revision/URI fields and uses the canonical viewpoint's overview target for
label/Enter activation. Expanding is still read-only. Without that field, show
unavailable-overview status even for an empty viewpoint; never call broad selection.
VP07 retains fallback until its separate projection is implemented. Preserve
original contextual detail targets and old producer compatibility. Upstream
work unit: SDP/05--Implementation/SDL/ViewpointNavigation/Plan.md in SDP-vNow.

## N3e selection — Session0005 T005, 2026-10-08

Deliver VP07 as a complete logical architecture base (all declared System,
container and unit nodes, including isolated nodes), overlaid with every Feature
and its contributing Functionality. Preserve actual contains, owns and
contributes-to edges, with distinct feature/functionality shapes and relation
styles. Do not fabricate placement from ownership. The default overview is
explicitly logical and omits allocation edges; discovery adds one lazy overview
selection per declared mode. A selected mode includes only its allocated-to
facts and explicitly reports unallocated contributions. Never union modes.
Existing feature detail targets remain usable. This uses the N3d typed target;
no XFMD parser or hardcoded viewpoint list is introduced.

A single union of all allocations is rejected because modes may be incompatible.
Generating all mode diagrams on activation is rejected in favor of on-demand
selection. Full structural base means decomposition, not every unrelated
interface/data/scenario relation. All declaration/relation provenance remains.
Verify isolation, multiple modes, unallocated/isolated features, empty models,
old VP01 behavior, actual discovery-to-native selection and real XFMD PDF.
The current branch/shared-baseline policy from N3d remains; no release/install,
model edit or broad commit. Exact source links remain N4.

N3e native-profile refinement: use rounded Feature nodes and typed rectangular
architecture/Functionality nodes. Hexagon/stadium syntax was rejected by the
existing Flowchart 1 parser during native acceptance; do not expand the consumer
profile merely for styling. Preserve the failed candidate evidence. The native
128-node bound also applies; oversized complete graphs fail without truncation.

### N3e overlay refinement after real-model acceptance

The literal realization graph (57 nodes / 137 edges) exceeded the native capture
budget on the real XFMD model. Replace Functionality nodes/edges in the overview
with deterministic Feature keys (F01...) annotated on their actual logical owners
and, for a selected mode only, on explicit allocation containers. The complete
structural contains graph remains. Unowned/unrealized Features remain standalone
rounded nodes. The legend maps every key to its declared Feature; a companion
table retains all contributing functions, owner/allocation and exact proof IDs.
Diagram Elements retain annotation provenance (Feature, owner/container, role,
source-fact IDs), and SourceFacts includes these proofs. This is a traceable
projection, not invented direct SDL relationships. No synthetic edges are added.
Detailed feature selections retain their explicit graphs. The initial literal
candidate and timeout are retained; all-mode union remains rejected.

N3e verification disposition: retain the annotation projection. A temporary
presentation group for standalone nodes was rejected after native/PDF inspection;
its PDF pagination worsened. Full-model fit-to-width labels remain small, so large
view reading stays with KB020/028. This does not change projection completeness.

## N3h reading refinement — Session0005 T013

[Full-size diagram reading](N3h-Reading.md) selects an opt-in continuous viewport
policy using existing zoom and scrolling. Fit remains default, A4/PDF unchanged.
This does not close producer detail/source or large-overview publication gaps.

## N4 current delivery

[Source/detail contract](N4-Source-and-Details.md) is implemented.
[Viewpoint disposition](Viewpoint-Disposition.md) supersedes earlier pending-status
projections without rewriting their historical evidence.
