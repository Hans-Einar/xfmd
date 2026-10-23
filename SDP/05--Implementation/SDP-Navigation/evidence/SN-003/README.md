# SN-003 — Dynamic SDP tabs, labels and source access

Date: 2026-10-01. Session T004 implementation and T005 owner steering.
Scope: KB-XFMD-021, PLAN-XFMD-0006 M3a, UR-046/SR-029 and FUNC-032.

## Outcome and evidence boundaries

Dynamic tabs, canonical viewpoint labels, producer-supplied abstraction labels
and native View source are implemented. The candidate is a shared dirty working
tree, identified in candidate.json; this is self-verification, not independent
approval, owner acceptance, release or Session completion.

Requirements and the textual FUNC-032 blueprint were reconciled before code.
Owner T005 explicitly holds authored SDL/SDUI and generated model views. No model
source or generated view was edited during this increment. Candidate metadata
records source hashes; the two canonical model hashes recorded by SN-002 still
match. #022 captures future Git/release-based design diff and change blueprints.
M5's semantic reconciliation remains outstanding.

## Verification matrix

| Obligation | Evidence and observed outcome | Boundary |
| --- | --- | --- |
| Ordered tab roots and no fixed names | SdpDiscoveryTest decodes nonstandard roots; SdpDynamicGuiTest uses real FOX tabs, adds Sessions under an arbitrary root ID, reorders/removes roots, updates labels, preserves tab/row identity, falls back to usable roots and handles zero tabs | Extra Sessions is a controlled native fixture; updated real producer pending |
| Names and depth bounds | Canonical viewpoint references display full producer names; A0 code fallback and a producer-defined expanded A0 label pass; existing cycle/depth/4096-row checks pass; JSON rejects invalid depth and caps producer depth at 32 | No independent A-level dictionary; actual producer still supplies code-only levels |
| Source association | Source IDs resolve catalog entry paths, inherited context and invalid target-less source rows; conflicts/missing identities do not infer paths from labels | Composed model opens entry source, not guessed declaration file |
| Native source opening | Real SDPTool SdpNavigationGuiTest right-clicks a generated-view descendant, opens original Example.design in Editor and opens invalid SDL source without generation | Actual existing producer, isolated Xvfb/private project |
| Dirty and stale source guards | Cancel retains dirty token/path; Discard opens source; stale revision, mismatched path, missing file and escaping symlink preserve current document | Uses existing Save/Discard/Cancel document transaction; no new save implementation |
| Popup lifetime and refresh | SdpDynamicGuiTest changes tab topology while native popup is modal; stale action is rejected, dispatch completes and replacement tree appears | Native application test, not GUI sanitizer or independent review |
| Existing navigation | Real-producer generation, invalid SDL diagnostics, dirty/root/document/new-selection races, history and automatic card move tests retained | Older remaining M3 boundaries are not closed |
| Host default command | Offline gh-sdp integration passes against installed cache and this actual project (3,469 navigation nodes) | No gh extension or project upgrade performed |

## Commands and results

- `cmake --build build-ui -j4`: passed; build.log.
- `XFMD_UI_EVIDENCE=/tmp/xfmd-t004-ui ctest --test-dir build-ui -j4 --output-on-failure`:
  79/81 passed initially. BlueprintStructure found an extra level-2 heading;
  corrected it to a subsection. The expanded native test restarted the source
  watcher then asserted a diagnostic during its initial refresh; it now waits
  for that baseline, as its existing startup scenario already does. No product
  behavior or acceptance assertion was removed for these corrections.
- Rebuilt SdpNavigationGuiTest, then repeated SdpNavigationGuiTest and
  BlueprintStructure: 2/2 passed. All 81 configured checks have passing results
  on the final relevant sources. Product code was unchanged after the full run;
  see regression-initial.log and regression-corrections.log.
- `SDP_OFFLINE=true XFMD_SDP_PROJECT=/home/warloc/git/xfmd-sdl-navigation build-ui/SdpIntegrationTest`
  with XFMD_SDP_TOOL unset: passed; gh-sdp-integration.log.
- Blueprint structure/symbol checks, board schema/replay and diff whitespace
  checks are recorded in document-checks.log at closeout.

Screenshots: sdp-source-menu.png and sdp-dynamic-sessions-fixture.png show the
controlled native fixture; sdp-original-source-editor.png and sdp-sdl-view.png
show the actual standalone producer workflow. Long tree labels scroll horizontally
and have full-label tooltips; screenshots are supplementary to input assertions.
The native source screenshot contains a disposable test-project path, not a
modified XFMD model file.

## Development installation and reproduction

Continued owner T002 installation authority: staged with CMake, backed up existing
files and atomically replaced each payload file, executable last. No user process
was stopped. Installed binary matches build-ui/xfmd:

`485e236a927d4da95d1ca24c5885bfd7f65f50f91ebac336c6559903e32626bb`

Version remains `0.3 sprint/009/phase/057-favorites:206+dirty`; use the hash to
distinguish this snapshot from SN-002. Installation receipt and previous payload:
`/home/warloc/.local/state/xfmd/installations/20261001T143058Z/`.
The installed executable passed isolated Xvfb/temp-HOME startup; see installation.json
and installed-smoke.log. No published release or signed upgrade claim is made.

candidate.json records HEAD, product/test/model/producer/binary hashes. Apply
product.patch plus new-product-sources.tar.gz to the recorded HEAD for the dirty
product candidate; the archive includes prior uncommitted product work. The shared
documentation/model baseline remains repository-owned. Staged concurrent #020 was
preserved and no milestone commit, push or merge was made.

## Next evidence

After the owner provides the updated gh-sdp release, verify actual Sessions root
appearance and open its returned files. Producer-owned descriptive abstraction
titles remain a contract follow-up. Retain M3 generated-link, two-window isolation,
hostile-output and shutdown acceptance; #015 paired source/preview; held M5 model
reconciliation and independent review. #021 is gate-review, not silently completed.
