# N3i-001 — Isolated producer overview integration candidate

XFMD Session0005 T016 / PLAN-XFMD-0009 / KB-SDP-032, 2026-10-18.

## Delivery

Earlier N3d/N3e producer work is now a local scoped commit:
`cc7b52f84f05fdef63efc1a305023a12347026ad` on `sdp/xfmd-viewpoint-overviews`, based
on `48c919f`. Worktree: /home/warloc/git/SDP-xfmd-viewpoint-overviews. Its clean
committed candidate contains 18 files: overview/query/document/tree code and tests,
contract/requirements and the bounded producer work unit/evidence. It excludes
concurrent SDUI/widgets/governance/MCP work and their module changes.

The existing shared producer product files were preserved byte-for-byte. A
concurrent Session0010 documentation commit advanced its HEAD to 0918098 during
verification; it did not change the captured overview inputs. The shared index
was never staged/restored by this workstream. Closeout appends the handoff to
KB032, its work unit and shared histories. No merge, push, release, installation,
authored-model change or independent approval. XFMD product code is unchanged
from N3h; its broader Session remains active.

## Verification

- `go test ./viewpoint ./documents` in isolated SDL/go: PASS ([log](sdl-tests.log)).
- `go test .` in isolated SDPTool: PASS, including all root-package tests without
  the historical SDUI exclusion ([log](sdptool-tests-composed.log)).
- Added composed-model regression proves overview inclusion of the System and
  included goals/structure, sources.json provenance, changed included-file
  rejection and preservation of earlier output. Its initial fixture appended a
  declaration after statements, which SDL correctly rejected; the corrected
  fixture places declarations before statements ([initial log](sdptool-tests-composed-initial.log)).
- Actual binary build: `go build -o /tmp/xfmd-n3i-sdptool ./cmd/sdptool`.
- `XFMD_SDP_TOOL=/tmp/xfmd-n3i-sdptool` with native SdpOverviewGuiTest: PASS
  ([log](native-test.log)). Enter and pointer activation select the explicit
  overview; expansion does not generate; both VP07 modes preserve scope. Captured
  native/PDF artifacts are in native/. Both native screenshots were inspected.
  PDF text retains VP01 actors/goals and the selected Online VP07 labels.
- Actual XFMD source discovery/select advertises and generates VP01 (12 nodes,
  11 associations), VP07 logical and NormalDesktop (23 nodes/19 structural edges
  each, 113/137 source facts). See [results](live-projections.json), compressed
  discovery and the vp01/vp07-logical/vp07-mode bundles. Uses the returned targets,
  not consumer-invented URIs or manually authored diagrams.
- Isolated management/lineage validation and whitespace checks pass. Shared
  and consumer closeout validation is recorded in document-checks.log.

[candidate.json](candidate.json) identifies base, exact relevant source/module
hashes, tested producer binary and native test binary. Product code did not change
after the binary was built. The test-only composed-source addition was then run;
contract/work-unit documentation was consolidated before commit.
[integration-commit.json](integration-commit.json) records final Git identity.

## Historical state and integration

The committed parent contains a later ledger history but not the uncommitted
KB032 transitions. A trial importing just those older transitions failed the
strictly increasing event-ID validation ([log](management-import-initial.log)).
It was removed from the isolated candidate. Existing shared ledger bytes were
never changed, and no duplicate transitions were invented. Integrate that complete
shared history through its existing workstream; do not treat the isolated board's
older backlog snapshot as a new decision about KB032.

The authoritative shared-checkout handoff names the commit and explains how to
reconcile retained dirty copies (including the separate SDUI contract additions):
file:///home/warloc/git/SDP-vNow/SDP/05--Implementation/SDL/ViewpointNavigation/Integration.md

Read-only version check still reports installed SDPTool 2.2.0/ef8741c. This turn
verifies the candidate, not a released extension. Merge/release checks remain;
KB030 action discovery stays separate. Focused use cases/scenarios/conditions,
exact source links, readable publication and other viewpoint dispositions remain
unfinished KB032/Session0005 scope.
