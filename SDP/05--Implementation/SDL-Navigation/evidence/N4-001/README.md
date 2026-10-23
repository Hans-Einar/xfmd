# N4-001 — focused details and exact SDL source navigation

Session0005 T021, 2026-10-19 Europe/Oslo. Producer commit
fde2239f0cd8202f6221394d2a3dae84828edb10 follows N3j ecd9dcd on the isolated
main-based successor. XFMD remains the shared dirty candidate at HEAD 41fa4948;
source archive/hash manifest records actual bytes, not a claim of a clean commit.

## Behavior verified

- Native actor/use-case detail plus only its explicitly exercises-linked scenario
  sequence; supporting Feature/Functionality realization remains text/table.
- Missing pre/postcondition semantics are stated; unrelated scenarios/use cases
  and their gap diagnostics are excluded from the focused page.
- Producer sourceLinks bind original per-file paths, SHA-256 and byte-identical
  snapshots. Model-wide revision remains separate from file revision.
- Native source activation opens the correct included .design file and selects
  exact UTF-8 bytes. Single-file 0.5 and composed 0.6 sources are supported.
- Invalid ID/range/multibyte boundary, changed dependency, internal symlink and
  dirty cancel are exercised. History retains context. No language parser in XFMD.
- 50 explicitly selected diagrams succeed after removing fixed 24/48 count caps;
  real file/byte and diagram geometry limits remain explicit.

## Commands and results

| Evidence | Result |
| --- | --- |
| go -C SDL/go test ./... | PASS, sdl-final.log, complete module |
| go -C SDPTool test ./... | PASS, sdptool-final.log, complete module without exclusions |
| go -C SDPTool vet ./... | PASS, sdptool-vet.log |
| go -C SDPTool build -o /tmp/xfmd-n4-sdptool ./cmd/sdptool | PASS, binary hash in candidate.json |
| cmake --build build-ui -j4 | PASS, build.log |
| ctest --test-dir build-ui --output-on-failure -j4 | 92/93 initially; one blueprint heading-format failure, ctest.log |
| ctest --test-dir build-ui --rerun-failed --output-on-failure | PASS, ctest-recheck.log; corrected the heading, no test waived |
| Final producer: ctest -R 'Sdp.*Test\|SdlSourceGuiTest' | 12/12 PASS, ctest-sdp-final.log |
| Final producer: native SdlSourceGuiTest and SdpOverviewGuiTest with evidence output | PASS, native-final.log and overview-final.log |
| Blueprint structure/symbols and pure-layer checks | PASS, document-checks.log |
| Producer ProjectManagement schema/history/placement validation | PASS, 66 cards / 45 records / 604 events |
| Main and PG merge-tree forecasts | Both exit 0, merge-forecast.json; no actual merge |

Native commands use tools/run_with_xvfb.py, XFMD_SDP_TOOL=/tmp/xfmd-n4-sdptool
and XFMD_UI_EVIDENCE pointing at native/ or overview/. The full suite's checks
are green after the sole document correction; after the last producer-only gap
filter, full producer suites and all 12 SDP/native consumer checks were repeated.
Unchanged C++ product code did not require another full build.

Inspected focused-native.png, source-native.png and rasterized PDF first page:
actor/ellipse notation, original source path and highlighted pursuit statement
are visible. PDF text includes Browse, Reader, Happy, Caller, Handler and missing
condition statements; no Mermaid error. Small VP01/VP07 overview native/PDF
workflow passes. This does not prove large A4 overview print readability.

Development trials caught noncanonical/missing-contract fixture data and a nil
class-view declaration model; these were corrected before final full suites.
The expanded realization fixture initially lacked required Functionality ownership;
its valid owner was added rather than relaxing the validator. The first separate
overview evidence run failed because its output directory did not exist; creating
that directory passed unchanged product/test code. Initial and final native logs
are retained. A pre-paint source screenshot was replaced after adding settleNative
before capture; selection assertions already checked the actual editor state.

## Boundaries and disposition

[Viewpoint disposition](../../../../04--Design/SDL-Navigation/Viewpoint-Disposition.md)
records every VP01–VP11 outcome and named residual owners. The Session completes
its selected implementation round with explicit handoffs; broader cards/plan
remain active for large-print readability, remaining aggregate proposals,
Mermaid compatibility, other resource guards and integration/release.
No independent approval, production .design/.sdui edit, main merge, push,
installation or release. Installed XFMD0.8 and gh sdp are unchanged.

The final PG head is 3e407d6; main is e8760d3. No overlapping changed paths at
precommit preflight and both committed merge forecasts are clean. This excludes
future/unknown remote changes and is not a combined-runtime verification claim.
Shared producer board/ledger reconciliation is explicitly separate from the clean
code branch. Original cc7b52f ancestry remains historical, not the merge source.
