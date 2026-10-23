# Audit and improve Mermaid compatibility across XFMD's interpretation and rendering pipeline

| Field | Value |
| --- | --- |
| id | KB-XFMD-020 |
| project | XFMD |
| type | Proposal |
| CardState | backlog |
| created | 2026-09-29T23:48:33.074200+00:00 |
| source | Owner request 2026-09-30; SDP Session Gantt failure and earlier gitGraph/init report |
| next_review | Before the next Mermaid compatibility or rendering delivery |
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
