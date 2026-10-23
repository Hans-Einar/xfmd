# Session0005 N3a — initial diagram and producer analysis

Observed 2026-10-07/08 Europe/Oslo. This is the first bounded #020 study/correction
under PLAN-XFMD-0009, not the full Mermaid compatibility matrix or N3/N4 delivery.
Authored SDL/SDUI sources and the installed XFMD0.7 binary remain unchanged.

## Producer boundary

Live `gh sdp . discover --json` reports sdptool/0.2 and 33 viewpoint nodes, none
with overviewTarget. producer-contract.json retains the relevant records and
snapshot hash. Current SDPTool/tree.go Node has Target, but no distinct overview
field or exact source-range target. SDL/go/viewpoint/structure.go goals() emits
per-use-case/per-feature diagrams; that is not a complete VP01/VP07 aggregate.
VP08 scenario sequences cannot be attached to use cases without model associations.
The previously proposed aggregate/provenance contract remains unimplemented;
no guessed bulk-select URI or source-line heuristic is selected in XFMD.

## Reproductions and responsibility matrix

| Input | Actual XFMD boundary | Disposition |
| --- | --- | --- |
| Original VP07 ConfigurableScrolling | Parser accepts 12 nodes/18 edges. Layout reproduces the owner's exact non-orthogonal edge11 coordinates. Native preview has no diagram scene; PDF preserves the error/source. | Routing/placement investigation in the pinned Mermaid fork; not invalid SDL or dropped model facts |
| Nested SDUI containment | Parser accepts 5 nodes/8 groups; native preview/PDF render, but three successive group levels have identical rectangles. | Upstream hierarchy/geometry gap. Current SubgraphTree uses strict node-set size to find parents, so equal-membership nesting is not represented by that relation. Explicit hierarchy/provenance needs design; do not infer a successful layout from parser success |
| Gantt axisFormat/todayMarker report | Stops at `Unsupported Mermaid statement: axisFormat Slot %d` | XFMD profile/parser gap. This does not establish separate todayMarker support; no stripping or claim of a fix |
| gitGraph init/header/tag report | Stops at `Mermaid init directives are not supported` | XFMD profile rejection. Header/tag capability must be tested independently before selecting any init handling |
| Mindmap quoted shape labels | Valid shaped label parsed, then unused unshaped fallback eagerly calls text(line), which rejects the syntax's quotes | Bounded XFMD interpreter defect, selected and corrected with lazy fallback; preserves label/shape/ID/hierarchy and existing text restrictions |
| Proper use-case overview | No use-case family in current native semantic dispatch; producer currently emits flowcharts | Requires selected producer semantics and a supported native model/parser/layout path; no advertised support from documentation alone |

The native before screenshots are actual application output under isolated Xvfb.
VP07 shows source fallback; nested groups visibly overlap. PDF text confirms the
VP07 routing diagnostic. Native mindmap after image has the expected circle,
rectangle and rounded nodes, Unicode labels and retained parent-child structure;
its PDF contains all four labels. Test status/identity are recorded in README.md.

## VP07 correction hypothesis — isolated fork work

Worktree /home/warloc/git/mermaid-rs-renderer-xfmd-diagrams starts at the exact
XFMD dependency pin 589517a7107ab95ee7dfa5ae88c2f339c0a284dc, branch
fix/xfmd-vp07-routing. No hidden .deps patch or source pin change is selected.
Libavoid vendor connector.cpp creates a two-point route when no path is found.
The current validation reports a generic Backend non-orthogonal error; fresh
flowchart compute retries only NoSpace. Semantic layout already handles a broader
failure category. Test a narrow NoSpace classification for the two-point diagonal
sentinel, keeping invalid geometry rejected, frozen-position calls failing and
the existing single spacing retry/deadline. Other malformed routes stay errors.
This is a hypothesis until the captured-metrics regression and native path pass;
source inspection does not establish that retry is sufficient for this diagram.

## Next bounded work

Finish the fork reproduction/retry assessment, then select exact source/pin
integration if it meets all routing guarantees. Preserve the independent nested
hierarchy gap. Reconcile producer KB-SDP-032 before adding overview activation;
proper use-case and feature-overlay semantics, source spans, sequences/conditions
and the remaining viewpoint dispositions are still N3/N4 work. XM-M2 fixtures
remain in ordinary discovery per owner T005/T006; no exclusion policy added.

## Candidate result — 2026-10-08

The proposed retry classification passed13 fork tests and actual XFMD native/PDF
checks with exact measured labels. See README.md for the local commit and images.
The normal dependency pin is unchanged; this is a tested integration candidate.
Mindmap quoted-label correction is in the normal working build with22 parser
tests and12 affected CTests passing. Nested groups and producer overview contracts
remain open. No source-model changes or installed upgrade performed.
