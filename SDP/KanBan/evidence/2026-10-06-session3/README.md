# Session 0003 inspection — 2026-10-06

Owner capture and source/tool inspection, not product implementation or native
GUI acceptance. The VP07 .mmd fixture preserves the owner-supplied source.

Owner-observed error (not independently reproduced this turn):

```
Mermaid: Libavoid: Backend("non-orthogonal edge 11 [(477.0104064941406, 403.30827713012695), (913.8437194824219, 211.26335525512695)]")
```

Likely boundary to investigate: orthogonal edge routing/validation after parsing,
not enough evidence to blame SDL projection or to choose a renderer fallback.
Do not remove the edge or reject valid model relations to hide the failure.
Compare the exact diagram in the actual XFMD interpreter/contract/layout/PDF path,
then minimize while retaining the original fixture, inspect libavoid inputs and
returned routes, pinned dependency versions and measured labels. No fix claimed.

## Installed SDUI export

Commands run successfully against current XFMD project:

```
gh sdp . discover --json
gh sdp . sdui-preview --model desktop-d3d068302a4ec102 --entry EditorWorkspace --output /tmp/xfmd-session3-sdui-preview --json
```

Result schema sdptool/0.2, profile sdui/0.2; revision and original source are in
sdui-preview-result.json. sdui-text-preview.md is a byte copy of the generated
entry. It contains layout dump and content, labels explicitly static, no Mermaid
composition diagram or live callback execution. Output was written under /tmp for
this read-only producer probe; this does not change XFMD's runtime storage policy.

Source inspection: SDPTool/sdui.go calls SDUI/go/presentation.Markdown;
SDUI/go/presentation/markdown.go starts with Dump and renders static visible
content. SDPTool/tree.go makes repeated viewpoint references under abstraction
levels; XFMD SdpPanel::add emits the reference/depth-limit marker at cycles/depth.
These facts identify producer/consumer ownership, not native test results.

## Proper use-case notation

The official Mermaid documentation inspected on this date identifies
usecase-beta as a diagram family in 12.0.0+, including actors, system boundaries
and UML relations: https://mermaid.js.org/syntax/usecase.html
XFMD's explicit semantic dispatch does not currently list usecase-beta. Do not
infer native Rust support from JavaScript documentation or assume a dependency
upgrade alone establishes typed-model, layout and PDF compatibility. #020 owns
that check and selected support; #028 owns model-derived viewpoint semantics.

## Scope reconciliation

Only local #003/#026 merge into #027. Upstream KB-SDUI-003 capability pilot and
KB-SDUI-004 text fidelity remain owned there; new KB-SDUI-005 owns combined views
and readiness facade. Local #015 direct source/preview, #017 model maintenance,
#019 shared rendering, #020 compatibility and #021 source navigation remain separate.
Upstream KB-SDP-032 receives actual owner viewpoint feedback. No model sources,
application code, existing release evidence or concurrent staged work is rewritten.

## Record validation

XFMD board schema/replay/placement/index: PASS, 29 cards and 113 card events.
Upstream ledger schema/replay/lineage: PASS, 361 card events; both changed cards
match their latest event paths. New Session/card file links and git diff --check
in both repositories: PASS. Checks establish document integrity, not product
behavior, installed runtime readiness or independent design approval.
