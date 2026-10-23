# P34: Protected caption attachment

Requirement: SR-024. Owner: FUNC-024 / the renderer fork's caption stage.
DiagramPainter still consumes finished SVG; XFMD and `.deps` own no routing fixes.

A caption attaches to the nearest orthogonal segment with overlapping projection.
The open rectangle between the padded caption and that segment is protected
against other edges. Identity is the edge index, not source/target identity.
Placement, obstacle registration and final validation share this geometry.

The fork's `routed::attachment` owns the zone and obstacle extension.
`labels::place` prioritizes unambiguous attachment and tries up to four fixed
caption orders. `labels::obstacles` extends the caption obstacle towards its
owner, subtracting exactly Libavoid's global shape buffer so the owner's segment
remains usable. `labels::validate` checks actual final routes, while
`quality::measure` reports `attachment_intrusions`. Three routing transactions
and the final caption-only pass remain the limit. Failure returns NoSpace and
the visible diagnostic fallback; there is no hidden engine change. Invalid
port directions after obstacle routing are classified as NoSpace and can use
the existing single fresh-placement retry. Port validation is not relaxed.

Coincident routes on zone boundaries and minimum parallel separation remain
separate constraints. This does not promise to solve every parallel overlap.
Exact buffer subtraction matters: an extra tolerance gap previously admitted
a foreign connection into the protected zone; regression checks reject it.

## N3g: Preserve the nearest-owner rule during rerouting

Attachment exclusion alone allowed a competing route to approach the opposite
side of a caption. The existing ambiguity guard then rejected ChapterNavigation.
In addition to the attachment rectangle, register a caption exclusion polygon
with radius `max(0, ownerDistance - 3 - routerClearance)`. Distance is the same
Manhattan rectangle-to-route metric used by validation; chamfered corners avoid
the unnecessary corner blocking of a square expansion. The two overlapping
obstacles represent the union and have distinct IDs.

The owner can move in later routing passes, so this radius is a routing aid,
not proof of validity. Strict final validation, caption orders, transaction count,
placement retry and deadlines remain unchanged. Geometry stays in the fork.
See [N3g design](../../SDP/04--Design/SDL-Navigation/N3g-Routing.md) and its
exact measured/native/PDF acceptance. The local helper and geometry unit tests
remain together in labels.rs because they share private caption geometry; no
unrelated file split or new public API is required.

## Possible later port search

Libavoid receives positioned nodes and source/target connections with candidate
or fixed ports, then finds route points. The first pass selects ports; later
passes lock coordinates in edge-specific pin classes. Another edge cannot select
that class, but distinct pins can occupy the same physical point. A future port
search must therefore track physical occupancy and clearance explicitly.

A separate bounded search could try candidates, rerun Libavoid and accept only
strict overall improvement after attachment/collision checks. It would require
visited configurations, an attempt bound and the time budget. Shorter geometric
port distance alone proves neither a shorter route nor a better overall result.
Stability means a local result, not a global optimum. This is not implemented.
