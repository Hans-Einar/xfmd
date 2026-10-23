# N6-001 — VP03 responsibilities and capabilities

Session0007 T002, PLAN-XFMD-0009, KB028 and producer KB032. Producer candidate
63da7ae1ecc31793a2c82f78415481cf7d24eacc on sdp/xfmd-vp03-overview,
stacked on VP02 cdf4437. Main remains 5ca0e90; no merge/release/install.

## Outcome

Complete architecture annotated with R responsibility keys and separate C
contribution/provider keys. Every functionality and capability has a source-linked
register entry. Only explicit owns/realizes/provides paths inform annotations;
isolated structural declarations and unplaced capabilities remain. Features and
allocations are not substituted. Existing VP03 detail identities are preserved.
No authored production model or native product code change.

## Verification

- sdl-tests.log: full SDL module passes, including semantic/proof/empty/large
  inventories, original-detail parity and filter rejection.
- sdptool-tests.log and vet.log: full producer suites/vet pass, including composed
  source links, stale dependency rejection, discovery and Actions compatibility.
- ctest.log: all 12 affected native/consumer tests pass (six GUI tests).
- native-final.log: extended real XFMD discovery/Enter/expansion, VP01/02/07
  preservation, VP03 scene and vector PDF export pass.
- reading-test.log: actual-model full-size/pan/200% zoom/resize checks pass.
- actual-view and actual-result.json: real XFMD snapshot yields 23 structural
  nodes, 19 contains edges and 28 responsibilities; no capabilities are declared.
- artifact-checks.json: all structural PDF names and all responsibility keys
  retained; 154 source spans are within the exact retained source bytes.

Visual inspection: native/vp03-native.png, native/vp03-pdf-page1.png,
reading/full-size-detail.png and native/vp03-actual-page4.png. Long identifiers
wrap in PDF table cells; content remains present. The first raw PDF string check
failed on those wrapped identifiers, so the evidence distinguishes direct string
matches from visual inspection. It does not claim the existing large-A4-print
readability obligation is solved.

preflight.json/precommit.json retain concurrent-work checks: no overlapping
PG/coordination paths, including current dirty work. candidate.json records
exact code/binary/test identities; SdpOverviewGuiTest.cpp.txt preserves the consumer
test. The N4 source archive remains the unchanged native product baseline.

Same-context verification is not independent review or owner visual acceptance.
Shared XFMD dirty work and shared producer product files remain preserved. VP04
is the next proposed increment; the broader plan/cards stay active.
