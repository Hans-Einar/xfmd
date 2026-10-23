# U1-001 — SDUI original-source access

Session 0004 T001, 2026-10-07; PLAN-XFMD-0010 U1 / KB-XFMD-027.
Candidate: HEAD 41fa4948c5377edd792ba3f503e6b92b0fd4717a plus shared dirty tree.
[Candidate hashes](candidate.json) identify changed source/test bytes (copied under
source/) and executed test binaries. This is not a clean commit or release.

Build: cmake --build build-ui --target SdpDiscoveryTest SdpDisplayTest
SdpNavigationGuiTest -j 4. PASS; [build log](build.log). Existing aggregate-field
initializer warnings are not treated as errors by this build.

Tests: XFMD_UI_EVIDENCE set to this evidence directory, then
ctest --test-dir build-ui -R '^(SdpDiscoveryTest|SdpDisplayTest|SdpNavigationGuiTest)$'
--output-on-failure. PASS 3/3, 12.37 seconds; [test log](tests.log).
Real producer is the CMake-selected cached sdptool distribution:
/home/warloc/.cache/sdptool/distribution/193f5fe8c1c6d15455561c00ce10ab39a3302a4ed320e25e870a73a904a61715-sdptool.
The isolated native Xvfb test uses a temporary project with spaces in its path.

Unit checks cover opaque identities, invalid target-free sources, inherited frame
context, conflicting model identities, mismatched kind/suffix and SDL-only tree
flattening. Native checks use the real context menu and document transaction:
SDUI frame opens original text in Editor, dirty cancel preserves token/content,
discard opens source, obsolete revision and incorrect path preserve the document,
and invalid SDUI remains inspectable. Existing SDL source/race/history and real
SDUI text-preview checks remain passing.

Inspected sdp-sdui-source-editor.png: selected page source visible in Editor;
the status diagnostic is the intentional rejected-path assertion, with source
content preserved. Inspected sdp-sdui-view.png: existing static prototype/layout
and content render are visible and labelled non-executing. Other screenshots
retain the test's existing SDL regression evidence.

No new composition diagram, launch, toolbar, source-span highlight, upstream
change, installation or independent review is claimed. U1b is next; installed
0.6 is untouched. Commit deferred to preserve unrelated shared/staged baseline.
