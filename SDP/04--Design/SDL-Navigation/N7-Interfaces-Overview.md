# N7 — complete interface consumption overview

Owner continuation 2026-10-19, Session0007 T003, PLAN-XFMD-0009 / KB028 / producer
KB032. Stack sdp/xfmd-vp04-overview on N6 63da7ae with a scoped milestone commit.
Preserve production model hold, shared dirty XFMD/SDP-vNow and PG ownership.
No merge, release or install is selected.

## Selected projection

SDL consumes means Unit (including Container) is designed to use an Interface.
It proves neither a provider, mandatory availability, a runtime call, transport
nor order. Provides is Unit-to-Capability, not Interface provision. Requires is
mode-scoped necessity and belongs to VP05. Never derive these from consumes.

Use one bipartite dependency diagram with every declared Interface and every
explicit consumes edge/consumer. Interfaces without consumers remain isolated
rounded nodes. Consumer rectangles retain their actual Unit/Container kind.
Dashed arrows are labeled consumes and point from consumer to interface. Units
without consumes are outside this projection (the complete structure is VP02);
no invented provider nodes, containment edges or runtime sequences. The actual
XFMD model has 14 interfaces and 24 consumption facts, making a direct relation
diagram a suitable first representation. Test actual native routing before
claiming delivery; broader high-density readability remains a separate obligation.

Keep the existing Interface use table unchanged for detail compatibility. Add an
interface register with exact declaration links, explicit consumers and source
facts, including no-consumer entries. Empty interfaces and absent consumption
are explicit. Avoid the redundant full architecture annotation approach used by
VP03/VP07: it would obscure direct consumer/interface relationships here.

## Ownership and acceptance

SDL interfaceOverview and InterfaceOverviewContext own projection/context.
SDPTool advertises it through HasOverview/overviewTarget, with existing sourceLinks
and freshness. XFMD uses its generic path; no schema or native parser change.
Reject mode/focus/relation/diagram/traversal filters, preserve ordinary VP04 table
selection, and do not add count-only caps or silently truncate.

Verify many-to-many consumption, Container consumers, unconsumed interfaces,
empty inventories, proofs, excluded provides/requires/contains, deterministic
large inventories, unchanged detail table, composed source links/stale inputs,
discovery/Actions, full producer modules and native Enter/expansion/render/PDF.
Inspect the actual XFMD model through the new producer and full-size native
reading. Same-context checks are not independent approval.

### Native keyboard inspection

VP04 has no existing diagram children and is a leaf. Existing NavigationTree /
TreeActivation deliberately activates leaves with Right, Space or Enter; branch
Right only expands. Preserve that behavior, not a new viewpoint-specific keyboard
rule. The initial native test incorrectly assumed VP04 was a branch and failed;
correct its fixture-specific expectation, assert the leaf condition and verify
Right and Enter both generate the exact overview. Existing branch-expansion
checks for VP01/02/03 remain. No native product behavior change is needed.
