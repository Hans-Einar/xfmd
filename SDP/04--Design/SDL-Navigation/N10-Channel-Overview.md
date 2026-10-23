# N10 — communication topology and scenario index

Session0007 T006 / PLAN-XFMD-0009 / KB-XFMD-028 / KB-SDP-032. Owner continues
with VP08. BranchPolicy: sdp/xfmd-vp08-overview stacked on N9 76fc87f;
CommitPolicy: scoped N10 milestone, phase push permitted, no merge/release/install.
Production models and shared product/history edits remain preserved.

## Selected behavior

VP08 top selection is an index: all declared Channels with explicit Contracts,
all Modes with participation/scenario counts, and all Scenarios with declared Mode,
completeness and step count. No combined mode graph or all-scenarios timeline.
Named-mode overview targets reuse the existing generic discovery/select contract.
A selected Mode shows every declared Channel (including idle channels) and its
explicit participants. Sender arrows point participant to channel; receiver arrows
point channel to participant. Group repeated message bindings by participant,
channel and role to keep topology readable; preserve every contributing uses fact
in diagram source proofs and per-edge elements, with every message binding in the
source-linked participation register. Edge Fact identifies the first supporting
fact, not a replacement for the complete proof list. Only this mode's uses are used.

Channel contracts are global declaration facts; scenario rows and derived
MessageSets are isolated to the selected mode. Modes have no implied inheritance.
No delivery, transport, runtime activity, scheduling, pre/postconditions or execution
success is inferred from static bindings. Missing participants/scenarios are explicit
model absences. Channel topology is not a sequence diagram or an inferred direct
participant-to-participant connection. Existing per-Scenario sequence targets remain
unchanged and open on demand from the tree; the index links declarations to source.

## Ownership and compatibility

SDL viewpoint projection/context owns aggregation and source semantics. SDPTool
advertises canonical VP08 overview/mode targets. Existing XFMD discovery/select,
source handling and native flowchart renderer consume these without product changes.
Keep all channels including those with no uses; empty model/mode output is explicit.
Reject unrelated focus/diagram/relation/depth/direction filters. No fixed inventory
count cap or silent truncation. Preserve detail sequence identities and proof sets.

## Verification boundary

Test multi-message grouped edges, both roles/directions, disjoint modes and quiet
mode, channels without participants, all scenarios/step counts, proof fidelity,
empty and large inventories, deterministic selection and unchanged sequence details.
Verify canonical discovery/Actions, composed source links and stale refusal, full
producer suites and native expansion/selection/mode switching/source/PDF workflows.
Generate and render actual XFMD communication topology, recording exact sizes and
layout limitations. Same-context tests are not independent approval; large-print
acceptance remains separate. VP09 contract/storage overview is next proposed.

## Real-model refinement before final implementation

The initial one-graph-per-mode projection fails in native XFMD on the real model:
60 nodes/123 grouped edges reaches Libavoid: BudgetExceeded. Preserve the failed
graph, native diagnostic and screenshot under N10-001. Select deterministic
per-channel diagrams within each requested mode instead of a single union graph.
Every channel is retained, even idle ones; each neighborhood has all explicit
participants and role edges, with exact grouped source proofs. Participants may
repeat across channel diagrams; this is a partition of topology, not new entities
or omitted cross-channel relationships. No sequence timeline or content cap is
introduced. The mode page still has complete participation/channel/scenario
registers. This makes the selected workflow usable without an unrelated routing
algorithm change or raising a resource budget. Native proof must cover every
partition and all original participant/channel identities; large-print acceptance
remains separate. Keep the original failed union as a renderer/KB020 residual.
