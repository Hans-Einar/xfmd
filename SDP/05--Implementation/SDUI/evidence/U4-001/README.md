# U4-001 — XFMD 0.7 SDUI delivery and local installation

Owner SESSION-XFMD-0004 T002, 2026-10-07; PLAN-XFMD-0010 U1b/U2/U3/U4;
KB-XFMD-027 and the selected SDUI portion of KB-XFMD-029. Producer KB-SDUI-005.
Trace: UR-046/SR-029 → FUNC-032 (navigation/readiness/launch/source) and FUNC-023
(native group-ID correction) → documented implementation and evidence below.

## Delivered behavior

Selecting a discovered SDUI frame generates one page with composition mindmaps,
source/bindings provenance and the existing static text renderer. Each container
has its own overview/detail map, retaining hidden/reused instances and ordered
rows. The diagram describes source composition, not exact UI geometry.
Source links select exact UTF-8 byte ranges in the original editor; stale source
snapshots and invalid boundaries are rejected. Preview selection executes no UI.

SDP mode shows all four toolbar groups. SDUI Launch is enabled only for the current
validated, unchanged preview and available tools. Readiness distinguishes local
prototype and unbound symbolic SDL callbacks. Explicit launch opens the bundled
Fyne interpreter in its own window, surviving document navigation and XFMD close.
Failures produce a diagnostic/log. KanBan Add/Move remain visibly disabled pending
the separately unselected management-service implementation under KB-XFMD-029.
No embedded Fyne, SDL callback execution or native-widget parity is claimed.

SDUI owns normalization/provenance, combined projection and readiness. XFMD ships
sdui-preview and sdui-fyne privately in libexec/xfmd. The existing gh sdp extension
and its selected distribution were not replaced. The upstream SDPTool facade now
uses the same library when rebuilt; no upstream public release was performed.
Authored SDL/SDUI models remain unchanged under owner #022.

## Exact candidate and verification

[candidate.json](candidate.json) records both repository HEADs plus hashes for
686 source files and the executed application/helper/native-test binaries.
[source.tar.gz](source.tar.gz) preserves those bytes. Both workspaces contain
pre-existing/concurrent changes; this is an identified dirty local release, not a
clean committed/publication candidate. No broad staging, commit, reset or rebase.

- Final build PASS: [build.log](build.log).
- Final CTest PASS **89/89**, including **43 native GUI tests**, 81.78 seconds:
  [tests.log](tests.log). An earlier complete pass captured the images/PDF;
  [native-evidence-tests.log](native-evidence-tests.log). The subsequent product
  change only initializes the unavailable Launch tooltip, covered by the final test.
- SDUI `go test -race ./...` PASS: [sdui-race.log](sdui-race.log).
- SDPTool full tests and final facade test PASS:
  [sdptool-tests.log](sdptool-tests.log), [sdptool-facade.log](sdptool-facade.log).
- Producer CLI checks PASS: [cli-checks.py](cli-checks.py), [cli-checks.log](cli-checks.log).
  Checks revision, output/snapshot, UTF-8 content, nonempty/symlink output and entry.
- SduiWorkflowGuiTest uses the real producer and native Fyne in isolated Xvfb/HOME:
  combined page with three diagrams, exact byte selection, missing host, toolbar
  enablement, explicit launch, navigation survival, normal close, child failure,
  stale source refusal and PDF export. No producer/parser mocks on the normal path.
- Native images inspected: [composition-review.png](composition-review.png),
  [sdui-combined.png](sdui-combined.png), [sdui-fyne-launch.png](sdui-fyne-launch.png).
  The latter includes the actual mapped Fyne window with local-prototype label.
  [sdui-combined.pdf](sdui-combined.pdf) is four A4 pages; page-one raster inspected
  as [sdui-pdf-page1.png](sdui-pdf-page1.png). Existing text-rendering fidelity remains
  upstream KB-SDUI-004; no text/widget parity claim.
- Blueprint structure/symbol/layer and both board history checks are recorded in
  [document-checks.log](document-checks.log). Same-context verification is not
  independent review or owner acceptance.

Native inspection also found pre-existing overlapping nested flowchart groups.
Only the independent group-ID mapping defect was fixed and regression-tested.
Mindmaps avoid the geometry issue for SDUI; KB-XFMD-020 retains the layout repro
for the next SDL diagram Session. No SDL viewpoint changes were delivered.

## Package and installed identity

[package.json](package.json) identifies the CPack archive and all 210 files;
[package.log](package.log) records construction. Both private helpers match the
exact tested bytes; helper build/module identities are in
[helper-build-info.txt](helper-build-info.txt). Bundled Go notices are inventoried
under LICENSES/go; existing native assets/licenses remain packaged.

[install.py](install.py) verifies archive identity, backs up replaced files,
installs atomically per file and rolls back on failure. Installation PASS:
[install.log](install.log). All installed hashes equal the package inventory.
The [receipt](installation-receipt.json) retains the previous 0.6 identity,
backup location and exact installation time. No existing user window was stopped.
Installed startup with Mermaid passed in isolated Xvfb; installed producer/check,
combined Markdown, XFMD and Fyne startup additionally passed:
[installed-sdui-smoke.log](installed-sdui-smoke.log),
[installed_sdui_smoke.py](installed_sdui_smoke.py).

Release scope/checklist: [ReleaseChecklist.md](ReleaseChecklist.md).
Generated [Release-0.7.0.md](Release-0.7.0.md) matches canonical notes;
[release-log-check.json](release-log-check.json). Version 0.7 is owner-selected;
package version 0.7.0. Public tag/release/signature/CI/independent approval: none.

Implementation and requested installation are delivered. KB027 and producer KB005
await review; PLAN0010 retains that explicit review disposition. KB029 remains
in-progress only for the separate KanBan mutation scope. SDL diagrams continue
in Session0005 under PLAN-XFMD-0009 N3/N4 and KB020/021/028.
