# N6 — responsibilities and capabilities across the architecture

Selected by owner continuation 2026-10-19, Session0007 T002 / PLAN-XFMD-0009,
KB028 and producer KB032. Extend the completed VP02 round with VP03; preserve
its completion evidence. Producer branch sdp/xfmd-vp03-overview is stacked on
cdf4437, with a scoped milestone commit. Neither VP02 nor VP03 is implicitly
merged/released by this continuation. Production model hold remains.

## Semantics and presentation

Keep the complete System/Container/Unit hierarchy and contains edges. Show all
Functionality responsibilities using stable-in-snapshot sorted R01... keys on
immediate owners, and all Capabilities through sorted C01... keys and a legend.
Annotate an owner with a capability contribution only through both owns and
realizes facts; annotate provides only from an explicit provides fact. These are
presentation summaries, not new direct SDL relations. Provides never implies a
realization and ownership never implies a provider. Realizes means a contribution,
not proof of complete or verified capability realization. Features and runtime
allocation belong to other viewpoints and must not appear as such in this view.

A capability with neither mapped realization nor provider remains a standalone
rounded node. All capabilities remain in the linked register, including provider-
only, contributor-only and unconnected cases. Every Functionality is listed with
its immediate owner and actual contributions, even when it realizes no capability.
All structural declarations remain visible, including isolated components. Empty
inventories and absent providers/contributors are explicit rather than invented.

Use the proven VP07 annotation approach instead of one union graph of all
responsibility/capability edges; the latter has already failed useful large-model
rendering. Unlike VP07, keys and labels distinguish responsibility, contribution
and offered capability. Do not change VP07 or reuse feature semantics. Keep exact
proof IDs on each annotation and links to original declarations/facts in tables.
No count-only cap or silent truncation. Full-size native reading remains available;
large A4 publication remains a separate open obligation.

## Plumbing and acceptance

SDL responsibilityOverview owns projection and responsibilityContext owns the
register. SDPTool uses existing HasOverview/overviewTarget/select/sourceLinks.
XFMD uses its generic overview path; no native semantic parser or wire change.
Reject mode/focus/relation/diagram/traversal filters for the complete view. Preserve
all existing per-capability contribution/provider detail targets unchanged.

Verify exact complete structure, responsibility/capability inventories and proof
sets, derived versus explicit annotations, no-provider/no-contributor cases,
empty/large/composed sources, original detail parity, unsupported filters, Actions
and discovery selection, source freshness and actual native/PDF behavior. The
real XFMD model currently declares responsibilities but no capabilities: its
preview must say so and must not relabel Features as Capabilities.
