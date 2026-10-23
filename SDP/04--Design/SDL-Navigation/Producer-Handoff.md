# Producer handoff after native use-case support

## Current delivery boundary — 2026-10-18, Session0005 T017

KB-SDP-032 is broader than the N3i overview delivery. Only that bounded delivery
is implementation-complete in the tested candidate; the card is not waiting solely
for a merge. The isolated producer commit is `cc7b52f`, with reconciliation and
verification instructions in SDP-vNow's
`SDP/05--Implementation/SDL/ViewpointNavigation/Integration.md`.

| Outcome | Producer remaining work | XFMD position |
| --- | --- | --- |
| VP01/VP07 overview activation | Integrate the tested candidate with concurrent changes/history; verify merged candidate, release and upgrade | Typed target parsing and native activation already implemented and tested against the candidate; verify the eventual released producer |
| Focused use-case views | Replace generic focused projection with proper use-case notation; attach only explicitly linked ordered scenarios; report unsupported/absent conditions honestly | Native use-case/sequence rendering is available; integrated output acceptance remains |
| Exact SDL source links | Define and emit revision-aware per-file span links, including composed sources and stale handling; sources.json alone is insufficient | Extend the existing source navigation workflow under KB021 after the producer contract is selected |
| Other viewpoint overviews/details | Resolve the proposed disposition matrix and implement selected projections, with source-backed completeness evidence | Consume advertised targets; do not implement SDL semantics in the client |
| Readability and acceptance | Coordinate source-supported outputs and record actual owner feedback | Full-size Wrap reading is implemented; readable overview publication/PDF and remaining layout cases stay open |

Source inspection at T017 confirms `structure.go::goals` still calls the generic
diagram projection, `sourceProvenance` emits a registry, and XFMD's SdpProtocol
and SdpPanel already consume `overviewTarget`. This is a status clarification,
not a new product change or a rerun of the N3i tests. The next dependent source
navigation implementation needs the producer span/link contract first. KB030's
action API remains separate.

## Historical handoff and checkpoints

Session0005 T003, 2026-10-08. This refines KB-SDP-032 coordination; it does not
activate that upstream card or claim a producer contract has been adopted.

## Observed source boundary

SDL/go/viewpoint/structure.go still generates per-use-case and per-feature
relations with generic diagrams. SDPTool/tree.go has no overviewTarget. Current
source supports `Scenario exercises UseCase` (SDL/go/parser/vocabulary.go and
SDL/docs/profiles/SDL-Channel-Scenario-Profile.md), and query.go accepts that verb.
That provides a concrete fact for future scenario linkage; source inspection does
not establish that VP01 already includes those sequences. Preconditions and
postconditions remain unspecified unless the model has explicit supported facts.

## Producer implementation slice to select next

1. Adopt the optional overviewTarget proposal in SDPTool/Contract.md and its Go
   Node type. Use the existing target operation/project/model/uri/revision fields;
   bind it to an explicit producer-owned aggregate projection. Missing means no
   overview. Keep expansion read-only and detail targets unchanged. Do not advertise
   an overview until that projection is implemented and validated.
2. SDL VP01 builds the full actor/use-case inventory for the selected source scope,
   including isolated declarations. Emit the [XFMD profile](UseCase-Profile.md).
   Map `pursues` to an association, with an explicit legend. Do not turn supports
   or contributes-to into UML include/extend. Put functionality realization into a
   companion section. Preserve stable mapping from generated IDs to original IDs.
   Do not invent System boundaries from filenames or selected folders.
3. Link declared `exercises` scenarios to their existing ordered VP08 projections;
   report missing relationships/conditions as unspecified. Preserve provenance.
4. VP07 composes complete declared architecture with feature contributions and
   allocations, scoped by mode. Preserve unallocated functions as visible facts.
   This is independent of the corrected VP07 routing failure and must have its own
   source-fact completeness tests.
5. XFMD consumes only the advertised typed target, preserving project/revision
   checks, cancellation/history and no generation during expansion. Cross-project
   schema selection must precede code; no guessed URI syntax in the consumer.

Acceptance: old/new producer compatibility; invalid/foreign/stale targets; 25+
use cases; isolated actors/use cases/features; missing ownership/allocation;
multiple independent sources with duplicate display labels; explicitly linked
scenarios; no fabricated conditions; native/PDF review of generated output.
Existing rendering resource limits are a separate declared boundary; an oversized
aggregate must not be silently truncated or claimed complete.

Upstream code remains untouched in this increment. The next execution needs its
own selected producer plan under that repository's process, followed by the XFMD
consumer changes and real-tool acceptance. Native notation alone does not close
KB028 or the Session goal.

## T004 update — 2026-10-08

N3d adopts overviewTarget with projection=overview and delivers VP01's aggregate
actor/use-case diagram plus realization table. The installed gh extension remains
unchanged. The next selected producer step is VP07 with explicit mode handling;
focused proper use-case/sequence/condition views and exact-source provenance
remain open. Prior T003 observations above describe the pre-N3d source boundary.

## T005 update — 2026-10-08

VP07 now advertises a logical overview and separate named-mode diagram targets.
The annotation projection keeps all structural nodes and proven feature paths;
exact source links, use-case details and large-overview reading remain. See N3e-001
in the external Session's Implementation evidence. Installed gh is unchanged.

## T013 recovery — 2026-10-10

Read-only inspection now finds installed SDPTool 2.2.0, revision
`ef8741c433c7fde67e689f94a74e7afdf938f24f`. Actual project discovery advertises
zero overviewTarget fields. The local producer checkout still contains N3d/N3e
implementations, so do not confuse a working candidate with an installed contract.
N3h is consumer-only continuous diagram reading and can proceed independently.
Evidence summary is in XFMD N3h-001/producer-inspection.json.

The current producer also has design-core/0.6 source composition. Before N4,
recover `SDL/go/documents/bundle.go::sourceProvenance` and `sources.json`
(`sdl-source-provenance/1`), alongside legacy 0.5 facts/declarations. This registry
is not itself a clickable, revision-checked exact-source protocol. Specify source
identity, per-file revision, byte-span units, multi-span facts and stale/dirty
source behavior before extending the existing SDUI-only source-link handler.
Never treat the composed model revision as an individual file hash.

Focused VP01 still uses generic `v.diagram` in `viewpoint/structure.go`; existing
`DiagramMarkdown` prints fact IDs as plain text. Proper focused use-case notation,
explicit `exercises`-linked VP08 sequences, unspecified-condition handling and
source-link transport remain producer work under KB-SDP-032. The installed JSON
action API required by KB030 is a separate dependency; these findings do not
resume Session0006 or prove PLAN-SDP-0024 shipped. Preserve the concurrent producer
work in query.go/documents rather than assuming this checkout is an isolated unit.

## T021 current handoff — N4

Focused selected VP01 now uses actor/ellipse notation, explicit exercises-linked
sequences, separate realization and unspecified conditions. Optional sourceLinks
carry checked per-file snapshots and exact source spans into XFMD; no parser is
duplicated. Fixed 24/48 diagram-count caps are removed. See N4-001 and the
[viewpoint disposition](Viewpoint-Disposition.md). Main integration, release,
shared board-history reconciliation and remaining aggregate proposals stay open.
