# N13-001 — properties, traceability and reported model gaps

Session0007 T009 / PLAN-XFMD-0009 / KB028 / producer KB032. Candidate
486b29df14d22e8fdc77ee5e5002d6ee22ba9abe on sdp/xfmd-vp11-overview stacks on N12 f1c3342.
Pushed separately. Main remains 5ca0e90; no merge/release/install.

## Delivery and verification

VP11 advertises complete overviewTarget with snapshot counts, declaration-kind/
fact-verb counts, explicit property assignments, existing cross-viewpoint gaps
and complete source-linked declaration/fact registries. Source file/line context
supports composed and legacy single-file models. Gap origin/code/subject/mode/
Feature are retained. No new gap rules, defaults, inferred implementation coverage
or combined diagram. Ordinary VP11 selection remains unchanged. Gap detection is
not an exhaustive audit; property assertions are not verification evidence.

- sdl-tests.log: full `go -C SDL/go test ./...` passes. Tests cover exact counts,
  every statement/declaration/gap, all property categories, ternary/step/placement
  arguments, mode/Feature gap context, isolated/empty/large registries (171
  declarations/340 facts), deterministic output, modifier refusal and legacy parity.
  Renderer-configured VP11 invokes no diagram renderer and emits no UML legend.
- sdptool-tests.log/vet.log: full `go -C SDPTool test ./...` and vet pass. Discovery/
  Actions advertise all 11 catalog overviews; mode rows remain separate. Composed
  source test checks every fact/declaration span in two files, original snapshots,
  path markup escaping and stale included-file refusal preserving previous output.
- ctest.log: `ctest --test-dir build-ui -R 'Sdp.*Test|SdlSourceGuiTest'
  --output-on-failure -j4` passes 12/12, including six GUI tests.
- native-final.log: SdpOverviewGuiTest verifies expansion versus overview activation,
  47 declarations/87 facts/29 properties, cross-viewpoint gaps and an actual property
  link opening the exact editor selection. Earlier viewpoint workflows remain covered.
- actual-tree.json/target.json/actual-result.json retain the advertised typed target,
  revision and selected output. Actual XFMD has 312 declarations, 1034 facts, 195
  properties and one reported NO_ENCODINGS gap. These are source statistics only.
- check-artifacts.py/artifact-checks.json: all actual 1346 distinct source spans
  match complete statements/declarations; all 1034 fact sentences and 312 declaration
  names survive PDF. Actual output is 88 A4 pages. Fixture retains 134 exact spans
  and every fact/declaration in 11 PDF pages. No truncation or arbitrary row cap.

## Visual inspection and limits

Inspected actual native Preview, exact property-source editor highlight, actual PDF
pages 1/10/43/88 and fixture gap page 5. Registers and gap context are readable in
these samples. Text/identifier completeness is checked across the entire PDF; this
is not manual inspection of all 88 pages or general large-print acceptance.

The first actual screenshot remained in Editor after the source-link test, matching
normal view-mode persistence. The test now explicitly restores Preview before the
next render check; final native/PDF checks pass. The initial capture is retained as
actual-editor-before-test-restore.png. No product behavior was changed to fit a test.
Legacy single-file spans initially lacked a displayed filename; the overview uses
the checked single-source identity rather than calling a file snapshot inline input.

No independent approval, new native product or authored model change. Existing
KB020 packet-label/union-routing/large-print and KB025 whole-root discovery residuals
remain. Explicit model tree/select works; no claim that discovery guard was removed.

## Reproduction and handoff

Configure build-ui with SDP_TOOL_EXECUTABLE=/tmp/xfmd-n13-sdptool; build
SdpOverviewGuiTest. Use tools/run_with_xvfb.py with XFMD_SDP_TOOL set to that binary,
XFMD_UI_EVIDENCE to native/, XFMD_OVERVIEW_ACTUAL_SOURCE to actual-view/entry.md,
XFMD_OVERVIEW_ACTUAL_PREFIX=vp11, XFMD_OVERVIEW_EXPECTED_SCENES=0 and
XFMD_OVERVIEW_EXPECTED_TEXT='| Facts | 1034 |'.

candidate.json and archived native test files identify exact inputs. N4-001 retains
native product baseline; preflight/precommit record PG 6dfde97/coordination with no
relevant overlap. final-git.json verifies candidate hashes and pushed branch/main.
No shared history import or PG mutation; shared producer receives KB032/Integration
handoff only. Phase pushes follow current AGENTS/selected increment authority;
the producer work unit's original N3d-only no-push note remains historical.

All VP01–VP11 have selected overview candidates. N3j/N4 is integrated; N5–N13
await selected integration. Next: reconcile the stacked candidate and prioritize
retained acceptance/renderer work. KB032 legacy eager language-export alignment,
release/publication, KB020 and KB025 remain separate obligations. This round is
complete; broader PLAN-XFMD-0009 and KB028/KB032 remain active.
