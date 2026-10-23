# N10-001 — channel topology and scenario index

Session0007 T006 (2026-10-20–21) / PLAN-XFMD-0009 / KB028 / producer KB032.
Candidate d677d70f4bc839f6684e964f94a625c821795940 on sdp/xfmd-vp08-overview stacks on
N9 76fc87f. Pushed; main remains 5ca0e90. No merge/release/install.

## Delivery and selected presentation

VP08 top selection is a channel/mode/scenario index. Explicit mode rows generate
one diagram per Channel, retaining idle channels and every participant/binding.
Sender/receiver roles give edge direction; multiple message bindings for the same
role share an edge with complete proof IDs. Registers retain every binding and
source link. No merged scenario timeline, runtime behavior or mode inheritance.
Original individual scenario sequence targets are unchanged and tested natively.

The initial single-mode union fails native routing: Libavoid: BudgetExceeded,
60 unique nodes/123 grouped edges. initial-union/, native-diagnostic.log and
native/actual-render-failure.* retain this failure. Design was revised before
final code to partition by actual Channels, not a count threshold. Final output
has 40 diagrams, 119 node occurrences representing the same 60 source identities,
123 edges and all 202 uses proofs. Each diagram has at most 5 nodes/5 edges.
No content is dropped. General union routing remains KB020, not an N10 fix.

## Verification

- sdl-tests.log: full `go -C SDL/go test ./...` passes. Mode isolation, grouped
  messages and complete edge proofs, directions, idle/empty scope, 172-channel
  inventory, deterministic output, filter rejection and unchanged sequences.
- sdptool-tests.log/vet.log: full `go -C SDPTool test ./...` and vet pass.
  Canonical discovery/Actions and composed-source links/freshness are verified.
- ctest.log: `ctest --test-dir build-ui -R 'Sdp.*Test|SdlSourceGuiTest'
  --output-on-failure -j4` passes 12/12, including six GUI tests.
- native-final.log: extended SdpOverviewGuiTest passes with final producer binary.
  It tests VP08 index expansion versus selection, Offline/Online/Quiet mode pages,
  two-channel fixture partitions, source links and original Example sequence.
  Existing VP01–VP07 checks remain. Actual-model run opens actual-mode/entry.md,
  requires 40 native scenes, 119 node occurrences and 123 edges, and exports PDF.
- artifact-checks.json/check-artifacts.py: final partitions preserve all original
  nodes/edges/202 uses proofs. PDF retains all 60 node names and 22 scenarios;
  850 unique source spans are in bounds. Scenario cells wrap: bbox first-column
  extraction reconstructs identifiers, while plain text interleaves columns.
- Visual inspection: native/vp08-actual.png, actual PDF pages 1 and 38, and
  fixture vp08-Online-page2.png. Long identifiers wrap in the 50-page actual PDF.
  This does not establish general large-print/fit-to-page acceptance.

Reproduction: configure build-ui with SDP_TOOL_EXECUTABLE=/tmp/xfmd-n10-sdptool,
build SdpOverviewGuiTest, then run through tools/run_with_xvfb.py. Set
XFMD_SDP_TOOL to that binary and XFMD_UI_EVIDENCE to native/. For actual coverage,
set XFMD_OVERVIEW_ACTUAL_SOURCE to actual-mode/entry.md,
XFMD_OVERVIEW_EXPECTED_SCENES=40, XFMD_OVERVIEW_EXPECTED_TOTAL_NODES=119,
XFMD_OVERVIEW_EXPECTED_TOTAL_EDGES=123, XFMD_OVERVIEW_ACTUAL_PREFIX=vp08 and
XFMD_OVERVIEW_EXPECTED_TEXT='Participation register'. No per-scene size override.

## Scope limitations and handoff

Whole-project `discover --json` fails the existing combined 20000-node/32 MiB
inventory guard in this growing checkout. baseline-discovery-errors.log reproduces
it with N9 too. Explicit model tree/select succeeds: actual-tree.json/target.json
retain the advertised model/revision and generated outputs. No claim that root
discovery works; KB025 owns source inventory/lazy discovery/resource handling.
Do not silently exclude evidence models or claim this limit is newly fixed.

scope-note.md records initial fixture-ordering/compile corrections. No validation
was relaxed. The existing incomplete-participation message is translated in its
source. Native workflow was split into focused test fixture/check headers to keep
files readable; product code and authored models are unchanged. candidate.json
archives code, binary and all native test file hashes; N4-001 is native product
baseline. preflight/precommit record no relevant PG/coordination overlap.
Same-context checks are not independent approval. VP09 contracts/storage is next.
