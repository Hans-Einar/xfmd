# N12-001 — Encoding inventory and explicit packet layouts

Session0007 T008 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
f1c334218c00cb6ca20f8dce19880cb65536cea4 on sdp/xfmd-vp10-overview stacks on N11 6b80b40.
Pushed separately. Main remains 5ca0e90; no merge/release/install.

## Delivered behavior

VP10 advertises complete on-demand overviewTarget with one independent validated
packet per Encoding, source-linked Encoding/field registers, Datagram family and
Variant coverage. Registers include declared byte/bit order, closed Contract,
header/variant owner, exact bit range/width and field type/presence. Logical
Variants without Encoding have explicit gaps. No concatenated layout, inferred
serialization or optional/padding behavior. Legacy detail diagrams remain unchanged.
No native product change or authored model edit; test fixture/helpers extended.

## Verification evidence

- sdl-tests.log: full `go -C SDL/go test ./...` passes. Tests cover both orders,
  shared header, signed and bytes fields crossing byte boundaries, unencoded
  optional Variant, 32 packets without truncation, empty inventory, invalid layout
  rejection, source/proof completeness, narrowing-filter rejection, determinism
  and preserved detail selection. Bundle tests verify exact source placement links.
- sdptool-tests.log/vet.log: full `go -C SDPTool test ./...` and vet pass. Existing
  composed-source/freshness and Actions tests now include VP10 overview targets.
- ctest.log: `ctest --test-dir build-ui -R 'Sdp.*Test|SdlSourceGuiTest'
  --output-on-failure -j4` passes 12/12, including six GUI tests.
- native-final.log: SdpOverviewGuiTest verifies branch-only expansion versus
  overview selection, two 29-bit packets, source-linked registers, original single
  packet detail and native/PDF export. Earlier viewpoint workflows remain tested.
- actual-tree.json/target.json/actual-result.json retain the advertised target and
  selected source revision. Actual XFMD has no Encodings or Variants; its explicit
  empty page is verified natively and in PDF, without invented packets.
- check-artifacts.py/artifact-checks.json: 34 fixture source spans in bounds; all
  six placements match exact statements; original detail emits identical packet
  syntax. PDF text retains all field names. This is not visual label acceptance.

Initial GUI failure: the test accepted the prior overview as the detail because
both contained PacketBig's title. The final test waits for changed document path
and current render token. native-initial.log/ctest-initial.log retain that failure;
final reruns pass. No product behavior was changed to accommodate the test.

## Visual findings and limits

Inspected native overview/detail, fixture PDF pages 1/2 and actual empty PDF.
The 1-bit PacketFlag label is visibly clipped and adjacent 12/13 boundary labels
are crowded. Reproduced on the original detail; producer syntax/positions agree.
KB020 now records the reproducer and screen/PDF follow-up. Full field registers
remain readable (long identifiers wrap), but do not close diagram readability or
general large-print acceptance. Retained inspected page images are supplementary
to the actual native shell/PDF artifacts. No independent approval is claimed.

Existing whole-root discovery guard remains KB025; explicit model tree/select
succeeds. Union routing residual remains KB020. N5–N12 await selected integration;
VP11 properties/traceability/model gaps is next proposed.

## Reproduction and identity

Configure build-ui with SDP_TOOL_EXECUTABLE=/tmp/xfmd-n12-sdptool and build
SdpOverviewGuiTest. Run through tools/run_with_xvfb.py with XFMD_SDP_TOOL set to
that binary, XFMD_UI_EVIDENCE to native/, XFMD_OVERVIEW_ACTUAL_SOURCE to
actual-view/entry.md, XFMD_OVERVIEW_ACTUAL_PREFIX=vp10,
XFMD_OVERVIEW_EXPECTED_SCENES=0 and XFMD_OVERVIEW_EXPECTED_TEXT='No Encodings are declared'.

candidate.json and archived native test files identify tested inputs; N4-001 holds
the native product baseline. preflight/precommit record no relevant PG/coordination
overlap, exact heads and scope boundary. final-git.json records publication identity.
Shared producer receives KB032/Integration handoff only; shared histories unchanged.
