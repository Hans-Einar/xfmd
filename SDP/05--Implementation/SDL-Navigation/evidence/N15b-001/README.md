# N15b — XFMD 0.9 deferred discovery and local installation

Session0007 T012 / KB-XFMD-025 / PLAN-XFMD-0009 / FUNC-032.
Owner authorizes autonomous completion through local build/install of XFMD and
the required producer. Publication and project process upgrades are separate.

## Delivery

XFMD negotiates the read-only action catalogue, retains all source entries in
compact discovery, and loads a revision-bound model graph on expansion. Requests
are serialized; refresh/project switches invalidate pending work. Complete model
identity, namespace, references and targets are validated before atomic snapshot
replacement. Current documents, dirty buffers, source opening and tab/selection
state are preserved. Failed sources remain retryable. Older explicit unsupported
command contracts retain eager compatibility; transport/JSON failures are surfaced.

SdpDiscovery owns request orchestration, SdpExpansion owns pure graph validation,
and SdpPanel retains native presentation. The JSON bridge adds typed boolean
reading. There is no SDL parser or language-runtime ownership change in XFMD.

Version selection: XFMD 0.9 / CPack 0.9.0, additive feature delivery under the local
rule. The accompanying SDPTool is 2.2.0-dev at e194721, incorporating the N5–N13
viewpoint stack and N15a. Explicit XFMD_SDP_TOOL wins; otherwise XFMD searches its
private installation helpers using absolute executable paths, then uses gh sdp.
The standalone CLI receives the same verified binary during installation.
The signed gh-sdp default/cache and installed project profile are not changed.

## Exact candidate

candidate.json and source.tar.gz identify the shared dirty XFMD inputs, including
tests and the checksum-pinned renderer 49bb0ee. This is not an unchanged HEAD build.
producer-source.tar.gz retains selected producer source directories; its full clean
commit is recorded and pushed on sdp/xfmd-lazy-discovery. External Go dependencies
are identified by the module files and binary metadata, not claimed vendored here.
No broad staging, model-source edit, main merge or public release.

## Verification and installation status

Final results and actual installation receipt are recorded below after execution.
ReleaseChecklist.md distinguishes local acceptance from inapplicable public
publisher gates. No independent review or owner visual approval is implied.

## Reproduction and retained failures

Build with SDP_TOOL_EXECUTABLE pointing at the recorded private producer. The
integration tests require its deferred contract; older-producer fallback is covered
by explicit catalogue/argument fixtures. The full CTest command is
`ctest --test-dir build-ui --output-on-failure -j2`.

The actual-project native check uses SdpDeferredGuiTest with
XFMD_DISCOVERY_PROJECT=/home/warloc/git/xfmd-sdl-navigation and the evidence-only
run_actual_xvfb.py harness (180-second outer timeout). Write screenshot/log output
outside SDP while the watcher runs, then copy it here. Assertions cover initial
unloaded nodes, all eleven loaded viewpoints, dirty-document preservation, refresh
and actual VP02 generation. The smaller normal test also covers serialized model
requests, retry after failure, project-switch cancellation and legacy/error policy.

Early runs are retained: one CTest raced an unfinished link; an existing stale
selection test needed to await deferred loading; its source-menu test now freezes
watch reconciliation while locating/injecting a native row click. Dedicated dynamic
and deferred tests retain refresh-race coverage. The actual-project harness initially
hit its short timeout under concurrent load; a later inspection identified missing
X-server keyboard focus in the new test, preventing Enter delivery. The fixture now
sets focus like the established native tests. None of these failures was waived.

## Remaining scope

KB025 still owns source scanning/parsing costs, filesystem/byte/per-model guards
and native row/depth guards. This delivery removes the combined eager SDL graph
failure for the checked project, not every possible content limit. KB020 retains
broader large-print/routing acceptance. KB028 and producer KB032 retain integration,
legacy export alignment and independent review obligations. Local installation does
not mark those cards complete or assert the whole producer stack is merged.

## Final result

- Build: xfmd-n15b-build-final-candidate.log; full final CTest:
  xfmd-n15b-tests-release-candidate.log — 94/94 passed, 47 GUI.
- Actual native workflow: xfmd-n15b-actual-pass.log and actual-native/. PASS;
  inspected loaded tree and generated VP02 screenshot. No general large-print claim.
- Exact package: package.json; 225 packaged files, all binary/helper hashes match.
  Installation additionally copies the same private producer to bin/sdptool.
- Installed: installation.json; 226 files match the verified staged bytes.
  Private producer selection and actual discovery pass; installed raw Mermaid and
  SDUI/Fyne checks pass. gh-sdp and project receipt hashes remain unchanged.
- Release projections: 0.9.0 and all three dated sections pass generator checks.
  Blueprint structure and 311 implemented plumbing symbols pass.

Backup and canonical receipt directory:
/home/warloc/.local/state/xfmd/installations/20261021T004942Z

The installed-process observation initially inspected only main-thread children;
SDP jobs spawn from a worker thread. The corrected smoke observes children of all
threads and confirms the exact private executable/argv. This was an instrumentation
failure, not waived installation evidence. Initial logs remain alongside final logs.
