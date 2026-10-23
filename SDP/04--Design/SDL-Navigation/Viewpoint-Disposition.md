# Viewpoint delivery and residual work — 2026-10-21

Session0005 T021 / PLAN-XFMD-0009. This is an implementation disposition, not
owner acceptance of every proposed aggregate. All VP01–VP11 now have selected development overview candidates; integration,
publication and retained acceptance work remain separate. Original source inspection:
SDL/go/viewpoint/{catalog,structure,channels,data,documents,query,overview,
feature_overview,usecase_detail}.go in the N4 producer candidate, extended by the
N5–N13 overview/context modules and per-increment evidence below.

| Viewpoint | Delivered selected view | Aggregate disposition and next work |
| --- | --- | --- |
| VP01 | Complete actor/use-case overview; focused actor/ellipse diagram with separate realization; explicit exercises-linked sequences and unspecified-condition statement | Selected overview implemented. Legacy eager language export still uses generic diagrams; KB-SDP-032 owns aligning that separate export, not an XFMD parser change |
| VP02 | N5 complete System/Container/Unit overview plus contains and source-linked structural register; roots/parent details preserved | Implemented candidate cdf4437 under Session0007, full producer/native/PDF tests. Awaiting separate integration/release; no deployment inference |
| VP03 | N6 complete architecture with responsibility/contribution/provider annotations and source-linked registers; original detail graphs preserved | Implemented candidate 63da7ae under Session0007 with full producer/native/PDF tests. Awaiting integration/release; no feature or allocation inference |
| VP04 | N7 complete Interface inventory, explicit dashed consumption graph and source register; ordinary table preserved | Implemented candidate 85d62f2 under Session0007, full producer/native/PDF checks. No provider, transport or runtime inference; awaits integration/release |
| VP05 | N8 mode-index and isolated named-mode dependency graph/register; all capabilities retained, empty cases explicit; original details preserved | Implemented candidate 3b3d935 under Session0007, full producer/native/PDF tests. No inherited/union dependencies; awaits integration/release |
| VP06 | N9 complete Activities/refinement, separate prerequisites, linked status/coverage/contribution/scenario registers | Implemented candidate 76fc87f under Session0007; full producer/native/PDF checks. Source status is not evidence; no schedule or completion inference. Awaits integration/release |
| VP07 | Complete logical architecture with feature annotations; explicit mode-specific allocation overlay; existing focused feature targets | Selected overview implemented. Native Wrap has full-size reading. Large A4 overview printing remains KB020/028; small overview/detail vector PDF verified |
| VP08 | N10 channel/mode/scenario index and explicit per-mode channel diagrams/registers; original validated sequences retained | Implemented candidate d677d70 with producer/native/PDF evidence. Complete semantic partition avoids union routing failure; no merged timeline or mode inheritance. Awaits integration/release |
| VP09 | N11 Dataset neighborhoods plus complete data/storage/contract/variant/field/membership/projection registers; original details retained | Implemented candidate 6b80b40; full producer/native/PDF/source checks. Explicit persistence and ternary projection without SQL/runtime inference; awaits integration/release |
| VP10 | N12 complete Encoding/Variant coverage, separate validated packets and source-linked field layouts; original details retained | Implemented candidate f1c3342: full producer/native/PDF/source checks; N14 fork 49bb0ee corrects narrow-label clipping and adjacent bit numbers with native/PDF verification. Awaiting integration/release |
| VP11 | N13 snapshot counts, explicit properties, all reported cross-viewpoint gaps and complete linked declaration/fact registries | Implemented candidate 486b29d; producer/native/exact source/PDF evidence. Source assertions and bounded gap detection are not implementation coverage. Awaiting integration/release |

All saved-source selection pages now carry checked source bindings; rendered fact
IDs and declaration references link to original UTF-8 byte spans. Diagram shapes
are not themselves clickable source hit targets. Language-only output without a
filesystem-aware binding keeps text provenance. Missing overviewTarget remains an
explicit unavailable capability, not a request to generate every detail.

## Session completion versus broader card completion

The selected native implementation is complete: tree cleanup, advertised VP01/VP07
overviews, required notation/routing, continuous reading, focused scenarios and
exact source selection. N4-001 supplies integrated producer/native/PDF evidence.
Session0005 closes this implementation round with the following explicit handoffs;
it does not mark PLAN-XFMD-0009 or its broader cards complete and does not waive
previously recorded acceptance gaps.

| Remaining obligation | Successor / status | Concrete next action |
| --- | --- | --- |
| Readable large-overview A4 publication | KB-XFMD-020 and KB-XFMD-028, PLAN-XFMD-0009 N3 remains active | Select producer partitioning versus native tiled/large-page publication before implementation; verify actual real-model print readability. Current fit-to-page PDF is not a passed large-print gate |
| General Mermaid compatibility and nested SDUI containment overlap | KB-XFMD-020 active | Use retained exact fixture and measured native/PDF reproduction; no claim that this Session fixes all grammar/families |
| Remaining aggregate proposals and eager-export notation | KB-SDP-032 active in shared producer checkout | Use the matrix above to choose bounded projection increments; do not advertise unimplemented overviews |
| Removal of all remaining fixed resource guards | KB-XFMD-025 active | 24/48 diagram-count guards removed; file/byte, node/edge and discovery limits remain explicit, never silently truncate |
| Producer main integration and released availability | KB-SDP-032 integration handoff | Integrate main-based successor commits, preserving shared board history; validate exact released discovery/select contract |
| Native release/install and owner visual acceptance | Existing release workflow, not selected in T021 | Build is ready for inspection; installed 0.8 remains unchanged |
| Independent review | Existing card/plan review boundary | This same-context implementation/testing is not independent approval |

No production .design/.sdui update, ProjectGovernance mutation or new Actions API
consumer is included. Session0006/KB030 remains held for its released API.

## Session0007 T001 update

N3j/N4 is integrated on SDP main as 5ca0e90. VP02 N5 is committed and pushed as
cdf4437 on sdp/xfmd-vp02-overview, with complete inventory, source and native
evidence in N5-001. No release/install. VP03 remains the next proposed increment.

## Session0007 T002 update

VP03 N6 is implemented and pushed as 63da7ae, stacked on VP02 cdf4437. N6-001
verifies complete inventories, source facts and native behavior. No merge/release
or install. VP04 interface overview is the next proposed increment.

## Session0007 T003 update

VP04 N7 is implemented/pushed as 85d62f2, stacked on N6. N7-001 verifies all
interfaces/consumption proofs, original table and native rendering. Existing
Right-on-leaf activation is retained; expansion-only applies to branches.
No merge/release/install. VP05 mode-specific dependencies is proposed next.

## Session0007 T004 update

VP05 N8 is implemented/pushed as 3b3d935, stacked on N7. Mode-index plus explicit
mode targets preserve isolation and original detail/VP07 behavior. N8-001 verifies
producer/native/PDF and actual empty NormalDesktop. No merge/release/install.
VP06 activities/delivery planning is proposed next.

## Session0007 T005 — VP06 candidate

N9 76fc87f on sdp/xfmd-vp06-overview stacks on N8 3b3d935. Complete
activity refinement and separate prerequisites retain isolated activities, with
source-linked status, responsibility, Feature and Scenario registers. Status is
source-claimed; missing addresses does not imply missing native implementation.
Full SDL/SDPTool suites, vet and 12 affected consumer tests pass. Native/PDF checks
verify both graphs and all statuses. Actual XFMD has no Activities: its explicit
empty plan and all 28 uncovered Functionality links are verified. VP08 channels/
scenario overview is proposed next; VP07 is already delivered. Broader plan/cards
remain active; N5–N9 await integration. No merge/release/install, authored model or
native product change, PG mutation, independent approval or large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N9-001/README.md

## Session0007 T006 — VP08 candidate

N10 d677d70 on sdp/xfmd-vp08-overview stacks on N9 76fc87f. VP08 has an
index and explicit mode targets, complete per-channel participation diagrams and
source-linked channel/binding/scenario registers. Original scenario sequences
remain on demand. The initial union failed Libavoid's routing budget; final
channel partitions retain all 60 unique nodes, 123 grouped edges and 202 uses
facts across 40 diagrams. All 22 scenarios and node names survive PDF; 850 source
spans are in bounds. Full producer suites/vet and 12 native consumer tests pass.

Actual model tree/select succeeds. Whole-project discovery hits the existing
20000-node/32 MiB guard on N9 and N10; KB025 retains that obligation. KB020 retains
the failed union renderer fixture. No generic large-print acceptance, independent
approval, native product/authored model change, merge/release/install or PG change.
Extended Session round complete; broader plan/cards remain active. N5–N10 await
integration. VP09 contract/storage overview is next proposed; VP07 already exists.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N10-001/README.md

## Session0007 T007 — VP09 candidate

N11 6b80b40 on sdp/xfmd-vp09-overview stacks on d677d70. Complete data/storage/
contract inventories and Dataset neighborhoods retain source proofs and original
detail selections. Registers distinguish explicit persistence, source-claimed
completeness, optional fields and ternary projections without SQL/runtime inference.
Full producer suites/vet and 12 consumer tests pass. Actual XFMD's three Unit-held
Datasets render as 3 diagrams/9 nodes/6 edges; all 44 Contracts and 14 Fields survive
PDF, with 358 checked source spans. Persistent stores, Datagram/projection behavior
and unused shapes are verified by the native fixture. Contract count columns were
combined after PDF inspection; final artifacts were reverified.

Extended Session round complete; broader plan/cards stay active. N5–N11 await
integration. VP10 encodings/packets is next proposed. Existing discovery/routing
limits remain KB025/KB020. No merge/release/install, native product/authored model
change, PG mutation, independent approval or general large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N11-001/README.md

## Session0007 T008 — VP10 candidate

N12 f1c3342 on sdp/xfmd-vp10-overview stacks on 6b80b40. Complete Encoding/
Variant coverage and separate validated packet diagrams retain exact source links,
byte/bit order and ordered field positions/types/owners. No serialization or layout
is inferred. Original detail selection is unchanged. Full producer suites/vet and
12 consumer tests pass. Native fixture verifies two 29-bit packets and both orders;
34 source spans and all six placement statements check exactly. Actual XFMD has
no Encodings/Variants; its empty page is verified natively and in PDF.

Visual inspection reproduces clipped narrow-field labels and crowded adjacent bit
numbers in overview and original detail. KB020 retains this renderer follow-up;
new source registers remain complete. PDF text retention is not visual acceptance.
Initial native-test synchronization failure was corrected by waiting for the new
document/render token; final reruns pass. Evidence: XFMD N12-001.

Bounded projection round complete; broader plan/cards and renderer/discovery
residuals remain active. N5–N12 await integration. VP11 properties/traceability/gaps
is next proposed. No merge/release/install, native product/authored model change,
PG mutation, independent approval or general visual/large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N12-001/README.md

## Session0007 T009 — VP11 candidate and catalog handoff

N13 486b29d on sdp/xfmd-vp11-overview stacks on f1c3342. Complete properties,
snapshot counts, existing cross-viewpoint gaps and declaration/fact registries
retain exact source links and file/line context. Counts/properties are source
assertions, not implementation evidence; no new gap rules or inferred defaults.
Original VP11 selection remains unchanged. Full producer suites/vet and 12 consumer
tests pass, including composed-source freshness and native exact property selection.

Actual XFMD: 312 declarations, 1034 facts, 195 explicit properties, one reported
NO_ENCODINGS gap and 1346 exact source spans. Every fact sentence and declaration
name survives the 88-page PDF. Native fixture has 47 declarations/87 facts with
cross-viewpoint gap context. Inspected native Preview/source selection and sampled
PDF pages; no independent or general large-print acceptance. N13-001 retains inputs,
checks and artifacts. Source-link test restores Preview explicitly after Editor.

All catalog overviews now have selected development candidates. N5–N13 await
integration; N3j/N4 remains on main. Next: reconcile the stacked integration
candidate and prioritize retained KB020 acceptance/renderer work. KB025 discovery
guard and KB032 legacy eager-export alignment also remain open. Broader plan/cards
remain active. No merge/release/install, native product/authored model change or
PG mutation. Shared producer receives only this existing-card/Integration handoff.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N13-001/README.md

## Session0007 T010 — N14 and integration review

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

## Session0007 T011 — deferred discovery producer

N15a candidate e194721 implements opt-in compact discovery with all sources
retained and revision-bound per-model expansion, shared by CLI and JSON actions.
Full SDPTool tests/vet, producer document validation and twelve affected native
compatibility checks pass. The actual checkout retains 63 sources in compact
discovery; XFMD expansion returns all eleven overview targets. The aggregate
fixture expands 22,236 nodes across twelve models without building one combined
graph. Default eager behavior remains compatible. See N15-001 evidence.

Next: N15b native capability negotiation, expansion/atomic tree merge and stale
response handling. Installed XFMD still uses eager discovery. Source parsing,
remaining resource guards, integration and independent review are not completed
by N15a. KB025 and the broader plan remain active; no authored models changed.

## T012 delivery — N15b / XFMD 0.9

Owner-authorized local installation is complete. N15b negotiates producer support,
loads source graphs on expansion, validates/merges atomically and preserves dirty
buffers, source actions and refresh state. Full final CTest passes 94/94 (47 GUI).
The actual project loads all eleven XFMD viewpoints and generates native VP02.
Installed discovery retains 63 sources; the installed GUI is observed invoking
its private SDPTool with lazy-sdl, without an environment override.

XFMD 0.9 and standalone/private SDPTool 2.2.0-dev at e194721 are installed with
per-file checksum verification and predecessor backups. Mermaid and SDUI/Fyne
installed smoke checks pass. gh-sdp's signed 2.2.0 default and project receipt are
unchanged. This is a shared-dirty development delivery, not a public release or
main integration. Source archive, candidate/package hashes, screenshots, logs,
checklist and installation receipt are in N15b-001.

Remaining KB025: source scanning/parsing costs, filesystem/byte/per-model guards,
GUI row/depth guards and demand-driven non-SDL inventories. Broader KB020/028/032
acceptance, integration, legacy export alignment and independent review remain.
No authored SDL/SDUI edits or unrelated shared-tree staging. Cards/plan retain
active scope; local installation does not close those residual obligations.
