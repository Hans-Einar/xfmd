# N3a-001 — Session0005 first diagram increment

Owner T001 authorizes the diagram round; work spans 2026-10-07/08 Europe/Oslo.
PLAN-XFMD-0009 / KB020 → UR039–041, SR021–023 → FUNC023/024.
[Analysis](Analysis.md) records producer capability recovery and bounded findings.
This is not full #020 compatibility or N3/N4 completion.

## Delivered consumer correction

Mindmap shaped nodes with quoted labels incorrectly evaluated the unused plain
fallback. trees.rs now selects that fallback lazily. New semantic regression
checks IDs, Unicode labels, all three supported shapes, parent identity and invalid
text/indentation/closure. No raw source is passed to layout and profile restrictions
are unchanged. Fixture: tests/fixtures/mermaid/mindmap-quoted.mmd.

- Before: [parser rejection](mindmap-quoted-before.log).
- After: [layout](mindmap-quoted-after.log), [native/PDF](mindmap-quoted-native-after.log),
  [inspected screenshot](mindmap-quoted-native-after.png) and [PDF](mindmap-quoted-native-after.pdf).
- Interpreter tests: **22/22 PASS**, [parser-tests.log](parser-tests.log).
- Release configuration build PASS, [build.log](build.log).
- Relevant CTest: **12/12 PASS**, including four GUI tests and Mermaid Rust/PDF,
  [tests.log](tests.log). No full-suite rerun claimed in this turn.

## VP07 routing candidate — verified separately, not adopted into the pin

The unchanged source parses as12 nodes/18 edges and reproduces the exact reported
edge11 coordinates: [vp07-before.log](vp07-before.log), [exact metrics](vp07-exact.metrics.tsv).
The actual native application has no diagram scene and PDF carries the error:
[before screenshot](vp07-native-before.png), [before PDF](vp07-native-before.pdf).

The isolated Mermaid fork classifies a two-point diagonal no-route sentinel as
NoSpace. Invalid geometry is still rejected; fresh placement uses its existing
single spacing retry with the same Libavoid engine and deadline. Frozen-position
calls do not gain a placement fallback; other malformed paths retain Backend.
No edge loss, Legacy fallback or loosened geometry checks.

Local fork commit: e79d8f9caf649995a5db5e54b0ea5d6b7fab8221 on fix/xfmd-vp07-routing. Base is the current pinned
589517a7107ab95ee7dfa5ae88c2f339c0a284dc. [Patch](mermaid-fork.patch) includes the
fixture, exact Pango metrics, regression and rationale. No public push/release.
The initial direct-library red run used rounded TSV metrics and reproduced the
same failing edge with different coordinates; the final exact-metrics test matches
the original coordinates before retry. Native before was exact throughout.

- **12 existing Libavoid tests +1 VP07 regression PASS**: [fork-tests.log](fork-tests.log).
  Includes cancellation/deadline, malformed inputs, fixed geometry and route identity.
- Isolated consumer build: [candidate-build.log](candidate-build.log). It uses the
  recorded [candidate Cargo lock](candidate-Cargo.lock), not an assertion that
  normal XFMD's dependency pin/lock has changed. Debug FFI links the existing C++
  consumer and passes within its unchanged production2-second diagram budget.
- [Candidate geometry](vp07-candidate-after.log):12 nodes/18 edges retained;
  [actual native/PDF](vp07-native-candidate-after.log):one scene/one page.
- Inspected [native result](vp07-native-candidate-after.png) and
  [PDF](vp07-native-candidate-after.pdf), with [PDF raster](vp07-candidate-pdf.png).
  All node/relation labels retained. Existing diagnostic reports one optional
  label leader omitted; no relation is omitted. No general layout-optimality claim.

The normal build still pins the original Mermaid commit, so VP07 remains broken
in that build until dependency adoption. This isolated candidate is concrete
integration evidence, not an installed or released fix. The .deps tree was never
patched. Source pin integration and a normal-candidate test remain next actions.

## Other findings and handoff

Nested groups still overlap: [geometry](nested-before.log), [native](nested-native-before.png),
[PDF](nested-native-before.pdf). Equal-member node sets do not preserve explicit
nesting in the current hierarchy. Resolve the model/layout boundary before fixing;
no speculative geometry adjustment selected.
Gantt axisFormat and gitGraph init rejections are captured in their *-before logs;
later statements are not individually verified by those first-error results.
[Producer snapshot](producer-contract.json) confirms33 viewpoints without explicit
overview targets. Full aggregates, proper use-case rendering and SDL provenance
remain separate producer/consumer milestones under #028/#021.

[candidate.json](candidate.json) records dirty XFMD identity, exact sources,
probe/application binaries and the fork commit. [Source archive](changed-source.tar.gz)
preserves this increment. Probe scripts compile against the current CMake command
inventory; run them in a separate copy/output area to preserve recorded evidence.
The normal consumer uses the existing renderer pin; isolated candidate uses the
fork and candidate lock. Independently reproduce each boundary before comparing.

[Document checks](document-checks.log) pass. Validation initially caught a verbatim
upstream Markdown payload resolving links from the wrong repo; it was renamed
.updated.md.txt with bytes/hashes preserved, not edited as a local document.
Shared staged #020 snapshot remains in the index; the live card was moved active
without reset or broad commit. Existing changes are preserved. Root milestone
commit awaits scoped integration; fork correction has its isolated local commit.

Installed0.7 hash is unchanged. No version bump/install/publication or independent
review in this turn. Session0005 remains active; S1 complete, S2 next.
