# N8 — dependencies selected by mode

Owner continuation after N7 selects VP05, Session0007 T004 / PLAN-XFMD-0009 /
KB028 / producer KB032. No evidence of partially executed N8 product work after
the interrupted turn. BranchPolicy: sdp/xfmd-vp05-overview stacked on 85d62f2;
CommitPolicy: scoped milestone. No merge/release/install or production model edit.

## Semantics and interaction

Capability requires Interface in Mode states a necessary condition in that mode;
it does not prove sufficiency, provider availability, consumption, transport or
runtime execution. Modes never inherit dependencies. The producer stores these
Dependency facts in Views.Facts, not the general Relations map; filter checked
facts directly instead of changing unrelated extraction behavior.

The VP05 top target shows a mode index with declared modes and dependency counts,
plus exact mode declaration links. No dependency graph or combined fact table is
generated before a mode is selected. Explicitly explain selecting a mode under
VP05; no default mode or union of modes. Reuse existing overview-mode typed rows
for every declared mode, including modes with zero dependencies. No URI/schema
change; mode stays part of target identity and freshness.

For a named mode, show all declared Capabilities (including those with no modeled
requirement in that mode), only the required Interfaces, and all explicit requires
facts for that exact mode. Dashed requires arrows retain source fact IDs. A linked
capability register distinguishes no declared requirement from independence or
sufficiency. A selected mode with no capabilities has explicit text and no diagram.
Unselected interfaces belong to VP04, not this view. The actual XFMD model has
NormalDesktop but no Capabilities/requires; do not invent them for presentation.

A merged all-modes graph is rejected because it loses the required scope. A
mode-selector index plus per-mode graph fits existing generic consumer plumbing.
Keep original VP05 per-mode detail identities unchanged; also retain VP07 logical
and mode overlays. Test fixtures must address mode rows by viewpoint-qualified
IDs now that two viewpoints expose them.

## Plumbing and acceptance

SDL modeOverview / ModeOverviewContext own projection and index/register; Query
allows named modes for VP05 as for VP07, while other overview filters stay rejected.
SDPTool tree advertises the index and mode rows using existing targets. XFMD uses
generic selection; no native semantic parser or product code change.

Verify index with no cross-mode facts, exact selected proofs/inventories, shared
interfaces/different requirements, zero requirements, no modes/capabilities,
unknown mode/filter rejection, large inventories, existing details, source links
and stale composed dependencies, Actions/discovery and VP07 compatibility. Run
full producer modules and native selection of two modes plus index/empty mode,
PDF and exact counts; check real XFMD empty NormalDesktop result. No new fixed
count-only cap. Existing native resource/large-print limits stay explicit.
