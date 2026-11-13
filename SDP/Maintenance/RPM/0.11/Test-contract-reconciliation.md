# Real-producer test contract reconciliation

Activity0004 enables all real shared-engine native checks in the clean RPM build.
The first complete run on source e18f521 with canonical engine dab8a03 found five
stale optional test assumptions left from the pre-Activity0003 producer. That run
failed; it produced no qualified RPM. Tests remain enabled.

The owner-selected Activity0003 contract gives the producer ownership of opaque
primary targets, reading chapters, grouping and independent expansion targets.
XFMD must forward those targets and preserve source/history/race behavior. Native
checks cannot require an older producer's diagram identifiers, URI queries or
page layout where the accepted producer contract changed them.

| Check | Old assumption / observed failure | Corrected proof |
| --- | --- | --- |
| SdpIntegrationTest | A deferred source's primary target is its expansion request; VP02 has a /diagram/VP02-roots child and always emits Mermaid | Use returned expansionTarget independently of primary target; select the actual VP02 target; assert declared Desktop content. Retain atomic expansion validation, malformed responses, catalog sources, stale revision rejection and standalone preview. |
| SdpNavigationGuiTest | Diagram child identifiers are selectable rows; invalid sources are direct children; missing-runtime guidance contains its old prefix | Select the producer's primary VP02 row; traverse producer-owned source groups; retain invalid-source View source and exact source association. Keep all source-menu dirty/cancel/containment/symlink, Back/Forward, late delivery, repeated selection, root switch, watch refresh and stale model checks. Delay both shared SDUI and SDL routes for race tests. |
| SdpOverviewGuiTest | All primary URIs contain projection=overview; old overview-specific registers and fixed diagram counts define native acceptance | Independently read producer catalog and compare native primary targets exactly for all 11 viewpoints. Follow producer-supplied chapter links unchanged (instrumented argv), render every supplied Mermaid diagram, inspect exact source spans and reuse retained pages through Back. Keep optional native/PDF artifact inspection. Producer semantic assertions and old geometry expectations remain the producer/rendering owners' responsibilities; this does not claim unchanged old page output. |
| SdlSourceGuiTest | Focused Browse is addressed by /diagram/VP01-Browse and old unspecified-precondition wording | Select producer-supplied Browse object in its use-case collection. Preserve linked Happy scenario versus unrelated scenario exclusion, two diagrams, exact UTF-8 composed-source spans, dirty cancellation, changed sibling rejection and symlink refusal. Use current explicit unspecified-behavior text. |
| SdpDeferredGuiTest | Standalone model sources are top-level; primary activation means retry expansion; all children are exactly11 viewpoints | Traverse producer groups without eagerly loading entries, count actual viewpoints separately from Source files, and retry through expansion. Preserve serialized two-model expansion, dirty-token retention, explicit refresh, stale response/retry, project-switch cancellation and actions-error fallback limits. |

Targeted development runs confirmed each correction against the unchanged native
product implementation. Final exact shared-engine/source results and the clean
RPM evidence are recorded in the Session/release handoff. Failed trial outputs
remain preserved and are not counted as passing qualification.
