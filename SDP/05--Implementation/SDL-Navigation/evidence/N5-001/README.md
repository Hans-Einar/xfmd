# N5-001 — main integration and VP02 overview

Session0007 T001, 2026-10-19 Europe/Oslo. Owner authorizes merging existing work
and continuing to the next viewpoint. N3j/N4 merge 5ca0e90 is pushed to SDP main;
its tree exactly equals previously tested fde2239. Targeted integration tests pass.
See integration.json and integration-preflight.json. XFMD's shared uncommitted
product baseline is preserved, not included in that producer merge.

## VP02 candidate and evidence

Producer commit cdf4437 on sdp/xfmd-vp02-overview implements all structural
System/Container/Unit declarations and contains relations as one overview, with
isolated roots and an exact-source structural register. Other relation families
are excluded. Empty models are explicit, unsupported overview filters rejected,
old detail identities preserved. No new count-only cap. A 170-node inventory is
verified for complete producer generation, not native rendering at that size.

- Full SDL module suite: PASS, sdl-tests.log.
- Full SDPTool module suite: PASS, sdptool-tests.log, no exclusions.
- go vet ./... in SDPTool: PASS, vet.log.
- Native test build: PASS, native-build.log.
- All 12 affected CTests: PASS, ctest.log, including six GUI checks.
- Extended SdpOverviewGuiTest: PASS, native-final.log. Expansion is inert; Enter
  opens VP02; VP01/VP07 targets and both modes remain valid. Small fixture and
  actual XFMD output render/export successfully, with exact scene node/edge counts.
- Actual discovery and typed selection: discovery.json, actual-result.json and
  actual-view/. XFMD model revision e2a2f39e... has 23 nodes and 19 contains edges;
  all 23 original labels occur in the vector PDF text.
- Actual-model full-size/pan/zoom reading: PASS, reading-test.log and reading/.
- Small native/PDF and actual-model fit/full-size screenshots inspected. The
  actual A4 full graph remains small; existing large-print obligation stays open.
- Producer project-management validator: PASS, producer-docs.log. Consumer
  blueprint structure/symbol checks and diff checks pass, document-checks.log.

Commands use /tmp/xfmd-n5-sdptool as XFMD_SDP_TOOL. Actual native evidence adds
XFMD_OVERVIEW_ACTUAL_SOURCE=actual-view/entry.md and expected node/edge counts
23/19. Existing DiagramReadingGuiTest consumes that entry through
XFMD_DIAGRAM_READING_SOURCE. Exact hashes and the consumer test snapshot are
recorded beside this file. C++ product code is unchanged from N4; only the native
integration test was extended, so the unchanged full consumer suite is not rerun.

Early test iterations found old overview-count assertions, an abstraction-reference
match instead of the canonical viewpoint, and noncanonical mutated test source.
The tests now require the new target in addition to existing targets and mutate
a canonically formatted valid included file. No product validation was relaxed.

## Integration and remaining work

N5 is committed/pushed on its own branch; it is not automatically merged under
the owner's instruction to merge the preceding delivery. No release/install,
production .design/.sdui change or independent approval is claimed. Installed
XFMD/gh remain unchanged. Prior source models and concurrent PG edits are preserved.
Precommit comparison found no overlapping paths with current PG dirty work or
coordination. Committed merge forecasts do not cover unknown future changes.

Next selected-viewpoint candidate is VP03 (responsibilities/capabilities). Existing
large-publication, broader renderer and resource-limit tasks retain their owners.
