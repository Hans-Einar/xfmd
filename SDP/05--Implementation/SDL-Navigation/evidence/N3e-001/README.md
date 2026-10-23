# N3e-001 — VP07 architecture with feature annotations

Session0005 T005 / PLAN-XFMD-0009 / KB028 and producer KB-SDP-032.

## Outcome and limits

N3e implements lazy VP07 logical overview and named-mode targets. All declared
System/container/unit structure is retained. Feature keys annotate real owners and
selected-mode containers through proven Functionality paths; the legend and table
retain all Features, contributions and allocation gaps. No cross-mode union or
invented direct relation. Native Enter/selection exercises both mode targets.
Ten affected CTests pass (five GUI); SDL viewpoint/documents pass and the SDPTool
facade suite passes with one documented pre-existing SDUI test excluded. That test
also fails in an isolated copy with N3e edits removed: UIPreview uses Combined,
while the all-family test compares against Markdown. Do not claim full producer
suite success or change another workstream's acceptance in this increment.

The literal graph timed out; the adopted annotation projection renders the real
XFMD model (23 architecture nodes, 19 contains edges, 140 proof annotations and
137 facts) and exports an eight-page vector PDF. Small-fixture rendering is readable.
Real-model full-view labels remain small at fit-to-width / A4. Grouping disconnected
nodes was explored but not adopted: it improved native width while worsening PDF
pagination. Large-overview reading is still an acceptance gap owned by KB020/028;
no overall visual acceptance or Session completion is claimed.

The default target is logical. Named-mode children generate only their selected
mode. Feature keys F01... are local to this snapshot, not persistent model IDs.
Full declarations/spans stay in the bundle; annotation Elements contain role,
Feature, node and source-fact IDs. Exact clickable source navigation remains N4.
No authored model, installed XFMD0.7 or gh extension change; no release or review.

## Evidence

- [Final native suite](tests-final.log): 10/10, five GUI.
- [Restored producer checks](restored-producer-tests.log): focused native and
  integration checks after discarding the temporary grouping experiment.
- [SDL packages](sdl-tests.log) and [SDPTool suite with named exclusion](sdptool-tests.log).
  Command: go test . -skip '^TestSDUIAllFamilyDiscoveryAndSourceDelegation$'.
- [Isolated baseline failure](producer-baseline-failure.log) identifies the existing
  source-preview expectation mismatch; it is not a passing release gate.
- [Build](build-final.log), [document checks](document-checks.log),
  [producer document checks](producer-document-checks.log).
- [Native fixture](vp07-native.png), [fixture PDF](vp07-native.pdf),
  [real native window](live-vp07.png), [real PDF](live-vp07.pdf) and [native log](native.log).
- [Actual tree](live-tree.json), [logical bundle](live-logical/entry.md),
  [mode bundle](live-mode/entry.md), [mode manifest](live-mode/manifest.json).
- [Probe](native_probe.cpp) and [capture script](capture_native.py) reproduce the
  actual renderer/export pipeline against the normal Release libraries.
- [Candidate](candidate.json) records shared source snapshots and binary hashes.
  Archives contain preceding shared work too; scoped patches isolate captured
  before files, while new feature_overview files are in the producer archive.
- [Producer binary transition](producer-binary-transition.json) records concurrent
  documentation-only HEAD movement from 29baa77 to 1d54de6. Restoring the original
  projection after grouping is followed by actual-candidate native checks.

## Rejected candidates and follow-up

before-notation files retain unsupported hexagon/stadium syntax; native Flowchart1
rejects it. native-tests-initial.log records that failure. before-overlay files
retain the 57-node/137-edge literal projection and native timeout. before-label
files retain the escaped-line-break labels before their plain separator correction.
group-view and group-vp07 retain the rejected standalone grouping experiment.
No evidence is claimed for an installed producer. Fixed native graph bounds still
exist; complete unsupported graphs fail instead of silently losing declarations.

Continue with large-overview reading and focused use-case/scenario projections,
then exact source navigation. Existing per-feature detailed selections remain.
