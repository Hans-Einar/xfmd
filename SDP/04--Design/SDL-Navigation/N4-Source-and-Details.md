# N4 — focused use cases and exact source navigation

Selected 2026-10-19, Session0005 T021 under PLAN-XFMD-0009, UR-046/SR-029,
FUNC-032 and KB021/028. The production .design/.sdui hold remains unchanged.

## Focused use cases

VP01 use-case diagram selections retain their identifiers but emit the native
usecase-beta actor/ellipse profile. Keep realization outside UML notation.
Only explicit Scenario exercises UseCase links attach existing ordered VP08
sequence diagrams to a selected use case. Do not derive sequence order from
static relationships or treat generic properties as pre/postconditions.
Missing associations and unsupported conditions are labelled not specified.
A diagram's declaration nodes count as focus evidence even with no relation facts.

## Source contract and ownership

SDL owns spans and reference presentation; SDPTool binds checked source identities
to actual original files at saved-source generation. Add optional sourceLinks to
sdptool/0.2 select/preview results. Each entry has id, absolute path, bundle-relative
snapshot and per-file SHA-256 revision. URI: sdl-source://ID/START/END. START/END are
zero-based UTF-8 byte offsets, end-exclusive, into exact original source bytes.
IDs are unique within the returned bundle, not persistent model identifiers.
Composed-model revision remains distinct from each file revision. Multiple proof
facts are separate links; one ID must never collapse different source spans.

SDPTool copies every checked source file into source-inputs inside its private
bundle and verifies snapshot hashes plus complete source-graph freshness before
publication. SDL presentation uses links only when this binding exists; pure
language output without a filesystem binding retains explicit text provenance.
The bundle remains subject to existing explicit byte/file budgets.

XFMD accepts links only from retained generated-page context in the current
project. It validates unique identities, .design paths under the selected SDP
area, safe relative snapshot paths inside the owned bundle, bounded UTF-8 byte
ranges and current bytes against every source snapshot. Refuse stale graphs,
missing sources, symlinks and unrelated/foreign contexts. Open through the normal
dirty-buffer transaction, recheck after opening, then select the exact bytes.
No model reparsing, guessed filenames or PG decision/lifecycle logic in XFMD.
Older results without sourceLinks retain existing whole-file View source.

Rejected alternatives: guessing lines from IDs loses provenance; treating a model
revision as a file digest breaks composition; direct arbitrary file URLs bypass
the retained snapshot/context checks; duplicating the SDL parser in XFMD splits
language authority. Reuse the existing SDUI snapshot-comparison approach with
an explicit multi-file result contract.

## Acceptance

Single/composed source fixtures, non-ASCII bytes before spans, multiple sources,
stale included files, malformed paths/IDs/ranges, original detail targets,
linked versus unrelated scenarios, missing conditions, source navigation with
dirty cancel and history. Actual native preview and PDF verify the produced
use-case/sequence notation; actual source selection verifies byte equality.
Full affected producer and consumer suites run on the final candidate. Other
viewpoints retain explicit supported/unavailable dispositions; no new aggregation
is advertised without a complete implemented projection.

## Recovered on-demand requirement

KB025 already rejects arbitrary diagram-count selection limits. Main still had
24 producer / 48 projection limits; N4 removes those fixed counts and verifies
50 explicit diagrams. Actual bundle file/byte budgets and individual diagram
geometry bounds still fail explicitly rather than truncate. This is not a claim
that all outstanding KB025 limits are resolved.

The focused conversion applies to selected previews (the XFMD workflow). Legacy
eager language exports retain their previous generic projection for compatibility;
upgrading that separate export is explicitly listed in the viewpoint disposition.
