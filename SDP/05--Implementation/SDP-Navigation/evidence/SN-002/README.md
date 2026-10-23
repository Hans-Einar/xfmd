# SN-002 — Native navigation acceptance and development installation

Session T002, 2026-10-01. Owner authorized installing the latest XFMD and continuing
SESSION-XFMD-0001. This is a dirty development candidate, not a release, independent
review or completion of #014/#015. SN-001 is immutable historical evidence.

## Changes and observed checks

- Native SDP tab geometry now leaves the whole label reachable beside Files controls.
  Returning from SDP/Index to Files also restores sufficient header width. The
  status label fills available width without widening the sidebar to its text.
- The Xvfb test explicitly establishes keyboard focus and waits for native layout.
  Existing Index tests select the Index tab by position, rather than the last tab,
  which is now the conditionally hidden SDP tab. No Index expectation was removed.
- SdpWatch performs bounded background metadata scans, coalesces writes and queues
  rediscovery behind generation. It never follows symlinks or parses SDP sources.
  Navigation restores surviving expansion/selection IDs, including moved cards.
  Dirty document tokens and text survive automatic navigation updates.
- Focused SdpNavigationGuiTest, SdpPanelGuiTest and SdpWatchTest passed. They cover
  real producer SDL/SDUI delivery, dirty/document/root/new-selection races,
  generated Back/Forward retention, automatic card moves, cycle/depth/row limits,
  source edits/renames, symlink exclusion, watch retarget/stop and watch-limit errors.
- Focused C++ ASan/UBSan watch test passed with leak detection enabled. The linked
  Rust decoder archive is not instrumented; this is not full GUI sanitizer evidence.
- Current structural modeling verification passes: 38 blueprints, 11 use cases,
  73 requirements, 22 declared scenarios, six SDUI compositions, 293 source hashes
  and 248 generated outputs. The new async lifecycle still needs SDL scenarios.

## Final regression and installation

The final full CTest run passed **79/79** on the installed product sources,
including the additional SDP-to-Files header-width regression. Earlier runs found
actual tab geometry and test-focus issues, which were fixed rather than suppressed.
One intermediate blueprint check ran before this evidence README existed; the
missing link was resolved and the check passed its rerun and the final full suite.
Existing Index tests used `getLast()` for Index, which now selected the hidden
SDP tab; they now select the actual Index tab and retain their original assertions.

The configured suite includes 38 isolated GUI checks, real standalone SDPTool
integration, protocol/process tests, ptrace disk/PDF failure injection, blueprint
and layer checks. A separate default `gh sdp` integration passed using the verified
offline cache, including discovery of this actual project (3,440 graph nodes).

Installed `/home/warloc/.local/bin/xfmd` is `0.3 sprint/009/phase/057-favorites:206+dirty`.
Binary SHA-256 is `b7059c05f6dc2be217be49988f0656fb66f65c38504f200cd8ec9a01c86c17b2`,
identical to `build-ui/xfmd`. CMake staged the payload; existing files were backed
up, then replaced atomically per file with the executable last. The final receipt and immediate backup are under
`/home/warloc/.local/state/xfmd/installations/20261001T085456Z`.
The original installed build 205 is preserved in the earlier installation backup
`/home/warloc/.local/state/xfmd/installations/20261001T084739Z/previous/`.
The installed executable passed an isolated Xvfb/temp-HOME startup smoke check.
No owner processes were stopped. This is a local development install, not a
published release or an assertion that dirty bytes equal HEAD.

Commands: `cmake --build build-ui -j4`; `ctest --test-dir build-ui -j4
--output-on-failure`; `ctest --test-dir build-ui -R '^BlueprintStructure$'
--output-on-failure`; the recorded focused sanitizer argv; model inventory and
verification scripts; `SDP_OFFLINE=true XFMD_SDP_PROJECT=$PWD
build-ui/SdpIntegrationTest` with XFMD_SDP_TOOL unset. Installation was staged via
`cmake --install build-ui --prefix /home/warloc/.local` with temporary DESTDIR.
See installation receipt, logs and screenshots beside this record. The install
and smoke scripts are retained in the local installation receipt directory.


## Remaining acceptance

Generated-link routing across model selections, two-window isolation, hostile
producer output admission and full window-shutdown bundle cleanup still need
focused native evidence. Independent review is outstanding. #015 saved-source
preview remains queued. Do not close the Session from this checkpoint.

## Candidate reproduction

`candidate.json` records HEAD, source/binary/producer hashes and final installation
identity. `product.patch` plus `new-product-sources.tar.gz` preserve the dirty product
sources against HEAD, excluding the unrelated staged #020 card. Documentation and
models retain their own recorded hashes and project-owned working-tree baseline.
