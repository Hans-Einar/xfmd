# N7-001 — complete interface consumption overview

Session0007 T003 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
85d62f24115285984cacc952919c1cf99988857f on sdp/xfmd-vp04-overview
stacks on N6 63da7ae; main remains 5ca0e90. No merge/release/install.

## Outcome and proof

VP04 shows every Interface and every explicit consumes relationship/consumer.
Isolated interfaces remain visible. Dashed arrows express design-time consumption,
never inferred providers, transport, calls or necessity. Source-linked register
and the unchanged Interface use table retain proofs. No authored model or native
product code changes. The existing generic discovery/select path is sufficient.

- sdl-tests.log: full module passes; many-to-many/Container consumers, isolated
  and empty interfaces, 171 nodes/169 edges without truncation, exact proofs,
  excluded relations, deterministic output, old table parity and filter rejection.
- sdptool-tests.log and vet.log: full module suites/vet pass; composed source
  provenance/freshness and discovery/Actions compatibility covered.
- ctest.log: 12/12 affected native/consumer tests pass, including six GUI tests.
- native-final.log: actual native VP04 leaf Right/Enter activation, dashed edges,
  isolated interface, earlier viewpoints and vector PDF export pass.
- reading-test.log: full-size/pan/200% zoom/resize on actual generated view passes.
- actual-view, target.json and actual-result.json: real XFMD has 14 interfaces,
  12 consumers and 24 edges. artifact-checks.json proves all 26 node names are
  retained in PDF and validates every generated source span against the snapshot.

Initial native/CTest logs retain a failed test assumption: VP04 has no diagram
children, while Right-on-leaf already activates in NavigationTree. Inspection
confirmed the existing contract; corrected tests assert the leaf condition and
verify both Right and Enter, preserving prior branch-only expansion checks.
No native behavior or acceptance requirement was weakened.

Visual inspection: native/vp04-native.png, native/vp04-pdf-page1.png,
native/vp04-actual.png and reading/full-size-detail.png. Dashed dependencies and
isolated interfaces render correctly. Disconnected packing makes the full actual
fit-to-window diagram small; full-size reading works. Existing large-overview
layout/print readability remains open, not claimed solved.

preflight.json/precommit.json record current main and no PG/coordination path
overlap, including dirty work. candidate.json and environment.txt record tested
inputs; SdpOverviewGuiTest.cpp.txt retains the changed native test. N4-001's
archive remains the native product baseline. Same-context checks are not
independent review or owner visual acceptance. VP05 is proposed next.
