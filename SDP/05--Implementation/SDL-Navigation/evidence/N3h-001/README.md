# N3h-001 — Full-size continuous diagram reading

Session0005 T013 / PLAN-XFMD-0009 / KB020 and KB028, 2026-10-10.

## Delivered boundary

View → Full-size diagrams (Wrap) retains prepared diagram dimensions instead of
refitting them to the text column on each zoom. Existing horizontal/vertical
scrolling and 25–300% document zoom provide readable detail. Default fit, ordinary
text wrapping, source/dirty/undo state and the same DiagramScene are preserved.
The menu is disabled in A4; the window-local choice survives A4/Wrap switches.
Paged placement always fits the complete diagram. No renderer/producer dependency
change, authored-model edit, version bump, installation or publication.

The real N3e VP07 snapshot was exercised in the native application. Fit labels
remain small, while the full-size panned detail is readable. Large canvas whitespace
and offscreen content remain: users must pan, and can toggle fit back on for an
overview. This is continuous-view reading, not a compact overview redesign or a
solution for readable A4 publication. Focused use-case/scenario projections,
conditions, N4 exact sources and remaining viewpoint dispositions are still open.

## Design and checks

[Selected design](../../../../04--Design/SDL-Navigation/N3h-Reading.md) and
UR-040/AT-060 were reconciled before implementation, with FUNC010/007/024 ownership.
Architecture layers stay unchanged: Application owns FOX/menu/anchor coordination,
LayoutProfile owns the view flag/frame identity, renderer owns diagram placement.

- [Build](build.log) completes the normal Release build. The first invocation
  ended with exit 143 near completion; [initial log](build-initial.log) is retained.
  The resumed build completed successfully; no compiler failure is claimed.
- [Tests](tests.log) exercise affected contracts, renderer, preview/scroll, native
  diagrams, shared zoom/DPI, selection, paged/PDF, SDP overview/navigation and SDUI.
- DiagramReadingGuiTest uses native View-menu clicks and verifies horizontal reach,
  scene identity at 100/200%, resize, A4, return to fit, stale-frame rejection and
  retained dirty text/undo/editor selection. DiagramReadingTest compares paged
  geometry with the flag on/off and tests profile identity plus full scene extents.
- [Actual VP07 test](real-vp07-test.log) reruns that workflow with the captured
  complete XFMD overview from N3e. The [initial failure](real-vp07-test-initial.log)
  requested byte 8 inside the UTF-8 em dash in its heading. FOX correctly clamps
  that invalid cursor position. The final harness uses valid ASCII byte boundaries;
  the buffer/selection preservation assertions remain, with no product workaround.
- Inspected [fitted overview](real-vp07/fit.png),
  [panned full-size detail](real-vp07/full-size-detail.png), and
  [focused native example](native/full-size.png). The real source hash is recorded
  in candidate.json; this is a previously generated real-model snapshot, not a
  claim that today's installed producer generated an overview.

## Identity and dependency recovery

[candidate.json](candidate.json) records HEAD plus the shared dirty candidate,
510 captured inputs, source archive and binary SHA-256 hashes. The root HEAD alone
is not the tested identity. [scoped.patch](scoped.patch) isolates this turn's code
from [before.tar.gz](before.tar.gz); candidate-sources.tar.gz contains the tested
source tree including prior shared work. The existing staged KB020/030 snapshots
and broad dirty baseline are preserved; no root-wide milestone commit or
independent approval is claimed.

Read-only installed discovery now reports SDPTool 2.2.0 revision
`ef8741c433c7fde67e689f94a74e7afdf938f24f`, with **zero overviewTarget fields**.
[Inspection](producer-inspection.json) and installed-discovery.json.gz retain the
observation. N3d/N3e remain local producer candidates. The current upstream source
also introduces composed-model provenance, so N4 must distinguish model revision
from each source file's revision. See the updated Producer-Handoff for concrete
owners and remaining contracts. This inspection does not verify the KB030 action
API or resume Session0006, which remains paused for its upstream delivery gate.

## Final verification outcome

Normal Release build passes. **19/19 affected CTests pass (11 native GUI checks)**;
the separate actual VP07 workflow also passes. These are same-context automated
and visual checks, not independent review or owner acceptance. Installed XFMD 0.8
remains unchanged. Document/board/layer checks are recorded in document-checks.log.
