# N15 — deferred SDL model-tree discovery

Selected 2026-10-21, Session0007 T011, KB-XFMD-025, PLAN-XFMD-0009; producer
KB-SDP-017 navigation responsibility and existing ViewpointNavigation work unit.

## Analysis and decision

Navigation currently calls ModelTree for every discovered SDL source and merges
all graphs into one response. The combined 20,000-node/32-MiB limit fails the whole
project even when each model is navigable separately. XFMD additionally bounds
JSON graph sizes and materialized rows. Removing one constant or filtering evidence
models would not satisfy the owner's on-demand and source-visibility requirements.

Keep all ordinary sources, including XM-M2 and generated evidence. Add opt-in
`discover --navigation lazy-sdl` and the same optional `navigation` parameter on
JSON action `project.discover`. Empty/`eager` retains the exact legacy payload.
Only `lazy-sdl` uses a compact SDL source list: validated sources retain their
model ID, revision and tree target, advertise `childrenDeferred: true`, and carry
no model graph children. Invalid/unsupported/context-required sources remain
visible with diagnostics and no deferred target. Other tabs remain unchanged.
The top-level `navigationMode: lazy-sdl` explicitly identifies this result.

Add `tree --model ID --revision HASH --model-only` and JSON action
`project.model-tree` with required model/revision. Return only the selected SDL
Tree, rooted at `sdl`, with model identity/revision and complete original node,
reference, detail and overview targets. Reject empty selectors and stale revisions,
including changed composed inputs. Share one service across CLI and JSON; do not
create an alternative source registry or mutate project files. Existing `tree`
and eager `discover` behavior stays compatible.

## Bounded delivery and consumer handoff

N15a delivers/tests this producer contract against both synthetic aggregates over
20,000 nodes and the real XFMD project. N15b is the required XFMD successor:
request compact discovery with explicit old-producer compatibility, fetch a model
on expansion, validate project/model/revision and node namespace, merge atomically,
retain existing source links and document state, invalidate on refresh/project
switch, and serialize/cancel stale requests. An unloaded source needs a visible
expansion affordance; an empty model is different from a deferred one. Do not
fall back to eager loading on arbitrary errors.

N15a alone does not fix the installed XFMD sidebar. Filesystem/source-count and
byte guards, per-model tree limits, GUI row/depth guards and lazy KanBan/SDUI/file
inventories remain separate KB025 obligations. No claim of unrestricted scale,
source parsing deferred to expansion, or complete KB025 closure. Source parsing
still occurs during discovery to retain validation/revision semantics.

## Verification

Prove legacy byte/payload parity, all source identities/states retained, deferred
markers only on valid models, no model graph construction in compact navigation,
full selected-tree parity, stale refusal, CLI/JSON equivalence, advertised action
schema, composed-input freshness and explicit invalid argument failures. Use the
real growing XFMD checkout as service-level evidence, not a native GUI claim.
No authored SDL/SDUI changes, main merge, release/install or independent approval.

## N15b selected native design — T012

Probe the read-only actions catalogue before discovery. Use lazy-sdl only when
both the discovery enum and project.model-tree action are advertised. An explicit
legacy arguments diagnostic permits eager discovery; transport/JSON failures do
not. Serialize expansion requests behind the existing job; refresh/project changes
cancel pending work. Deferred source nodes show an expansion handle; failed requests
remain retryable. Merge a complete tree only for the matching project/model/revision,
with one SDL root, its model namespace and validated targets. Other tabs, current
document and source links remain unchanged. Refresh discards expanded graphs and
restores expanded rows by requesting current revisions. Never treat unloaded as empty.

Bundle the tested SDPTool beside the existing private SDUI tools; explicit
XFMD_SDP_TOOL still wins, then the installed private executable, then gh sdp.
This is local application distribution, not a signed SDP process upgrade or a
public gh-sdp release. Keep installed project process files unchanged.
