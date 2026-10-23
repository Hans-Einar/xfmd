# N9-001 — activities and delivery plan

Session0007 T005 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
76fc87f2bc41cf03026aa1b0620249086157ef3b on sdp/xfmd-vp06-overview stacks on N8 3b3d935.
Pushed separately; main remains 5ca0e90. No merge/release/install.

## Delivery and evidence

VP06 provides all Activities and explicit refinement, plus a separate prerequisite
diagram when depends-on facts exist. Source-linked registers show explicit status,
addresses with owners, delivers and illustrative Scenarios. Status is a source
assertion, not verification evidence. Missing addresses is model coverage, not
missing native implementation. Original detail/implementation output is preserved.

- sdl-tests.log: full `go -C SDL/go test ./...` passes. Exact facts/directions,
  all statuses, isolation, scenario/ownership links, empty and 170-activity scope,
  filter rejection, deterministic output and original detail parity pass.
- sdptool-tests.log and vet.log: full `go -C SDPTool test ./...` and
  `go -C SDPTool vet ./...` pass. Discovery/Actions, composed source links and
  stale-source refusal pass.
- ctest.log: `ctest --test-dir build-ui -R 'Sdp.*Test|SdlSourceGuiTest'
  --output-on-failure -j4` passes 12/12, including six GUI tests.
- native-final.log: SdpOverviewGuiTest with SDP_TOOL_EXECUTABLE configured to
  /tmp/xfmd-n9-sdptool passes. VP06 branch expansion versus Enter selection,
  two scenes (4 nodes/2 refinement edges; 4 nodes/1 dashed prerequisite edge),
  isolated activity, status/source links and PDF pass. Earlier viewpoints remain.
- discovery.json, target.json, actual-result.json and actual-view retain the
  actual XFMD selection. No Activities, no diagrams, 28 unaddressed Functionality.
  Native/PDF checks verify explicit empty state and all 28 names/source spans.
- artifact-checks.json records fixture statuses/scene counts and actual PDF/source
  assertions. candidate.json and SdpOverviewGuiTest.cpp.txt retain exact inputs.

The initial full SDL failure is preserved in initial-sdl-tests.log. The Scenario
fixture was corrected with the required mode, completeness and explicit event
step; product validation was not relaxed. Final full suite passes.

Visual inspection: native/vp06-native.png and vp06-pdf-page1.png/page2.png show
the separate graphs and readable source/status tables. Actual XFMD has no activity
graph; no positive real-model graph or new large-layout/print acceptance is claimed.
Same-context verification is not independent approval. N4-001 retains the unchanged
native product baseline. preflight/precommit.json record no PG/coordination overlap.
VP08 channels/scenario overview is next proposed; VP07 is already delivered.
