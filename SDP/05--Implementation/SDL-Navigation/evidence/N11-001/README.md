# N11-001 — data definitions, storage and contracts

Session0007 T007 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
6b80b40fb269e29a03dc9761c4facb8473a54f3e on sdp/xfmd-vp09-overview stacks on N10 d677d70.
Pushed separately. Main remains 5ca0e90; no merge/release/install.

## Delivery and evidence

VP09 combines Dataset neighborhoods with complete data/Database/Contract/Variant/
Field and shape membership registers. Projection rows preserve all three endpoints
and explicit ownership proof. Database-backed persistence is distinct from Unit-
held data; no SQL/ER, runtime transformation or packet layout is inferred. Original
Contract/dataset details are unchanged and remain on-demand tree selections.

- sdl-tests.log: full `go -C SDL/go test ./...` passes, including exact neighborhood
  relations/proofs, persistent versus Unit-held data, shared contracts, Datagram
  origin, unused Database/open Contract, optional Fields, ternary projection,
  empty/170-contract inventory, deterministic output, filter rejection/detail parity.
- sdptool-tests.log/vet.log: full `go -C SDPTool test ./...` and vet pass; canonical
  discovery/Actions and composed-source links/freshness are verified.
- ctest.log: `ctest --test-dir build-ui -R 'Sdp.*Test|SdlSourceGuiTest'
  --output-on-failure -j4` passes 12/12, including six GUI tests.
- native-final.log: SdpOverviewGuiTest verifies branch expansion versus overview
  selection, fixture data registers and two diagrams (9 node occurrences/7 edges),
  original Contract detail selection, source links and PDF. Earlier viewpoints
  remain covered. Actual source renders 3 diagrams with 3 nodes/2 edges each.
- actual-tree.json/target.json/actual-result.json retain the advertised target and
  selected checked source revision. Real XFMD has 3 Unit-held Datasets, 44 Contracts,
  14 Fields and no Databases, Datagrams, Variants or projections. Output states
  these absences without inferring unimplemented native behavior.
- check-artifacts.py/artifact-checks.json: all inventory and diagram names remain
  in PDF; 358 distinct source spans are in bounds. Bbox column reconstruction
  verifies wrapped identifiers without treating adjacent columns as one name.

Visual inspection: native/vp09-actual.png, actual PDF page 1, fixture PDF page 2.
Three count columns were combined after initial PDF inspection to give contract
names more room; final producer/native artifacts and tests were regenerated.
General large-print/readability acceptance remains open; no independent approval.

Reproduction: configure build-ui with SDP_TOOL_EXECUTABLE=/tmp/xfmd-n11-sdptool,
build SdpOverviewGuiTest, then use tools/run_with_xvfb.py. Set XFMD_SDP_TOOL to that
binary, XFMD_UI_EVIDENCE to native/, XFMD_OVERVIEW_ACTUAL_SOURCE to actual-view/entry.md,
XFMD_OVERVIEW_ACTUAL_PREFIX=vp09, XFMD_OVERVIEW_EXPECTED_SCENES=3,
XFMD_OVERVIEW_EXPECTED_NODES=3, XFMD_OVERVIEW_EXPECTED_EDGES=2, and
XFMD_OVERVIEW_EXPECTED_TEXT='No Databases are declared'.

candidate.json and archived native test files identify exact inputs. N4-001 retains
native product baseline; no native product or authored model changes in N11.
preflight/precommit record no relevant PG/coordination overlap. Existing whole-root
discovery limit and union routing residuals remain KB025/KB020 (N10 evidence);
explicit model tree/select succeeds. N5–N11 await integration. VP10 is next proposed.
