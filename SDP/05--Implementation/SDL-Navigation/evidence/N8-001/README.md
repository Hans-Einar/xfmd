# N8-001 — mode-isolated VP05 dependencies

Session0007 T004 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
3b3d935287ce69c5533aed8a9b1a557f217fe5a5 on sdp/xfmd-vp05-overview
stacks on N7 85d62f2. Main remains 5ca0e90; no merge/release/install.

## Delivery and evidence

VP05 top selection returns a mode index with counts/source links and no combined
dependency graph. Named-mode rows generate all Capabilities and only that mode's
explicit requires facts/Interfaces, with a source-linked register. No inheritance,
sufficiency, provider, consumer or runtime-flow inference. Empty states are explicit;
original detail views and VP07 overlays remain. No native product/model change.

- sdl-tests.log: full module passes, including exact scope/proofs, index with no
  combined facts, all modes/capabilities, shared port/different requirements, empty
  cases, unknown modes/filters, detail parity and 171-node/170-edge inventory.
- sdptool-tests.log and vet.log: full suites/vet pass, including discovery/Actions,
  viewpoint-qualified mode identity, composed source links and stale refusal.
- ctest.log: 12/12 affected consumer tests pass, including six GUI tests.
- native-final.log: index/branch expansion, Offline/Online/Quiet selections, exact
  target/scene counts, dashed edges, source links and native/PDF export pass.
  Earlier VP01/02/03/04/07 checks remain.
- actual-index and actual-normal: real XFMD's NormalDesktop has zero capability
  or dependency declarations. Index and selected empty page generate no diagram;
  native/PDF empty-state assertion passes.
- artifact-checks.json: positive-mode labels remain in PDF, source spans are within
  the checked snapshot, and actual empty-state text remains in PDF.

Visual inspection: native/vp05-Online.png and native/vp05-pdf-page1.png.
No new large-model reading/print claim is made: real XFMD has no positive VP05
graph, and existing layout/print/resource obligations remain.

preflight.json/precommit.json retain main/PG/coordination identities and no
overlapping committed/dirty paths. candidate.json and SdpOverviewGuiTest.cpp.txt
retain tested code/binary/test inputs; N4-001 remains native product baseline.
Same-context verification is not independent approval. VP06 is next proposed.
