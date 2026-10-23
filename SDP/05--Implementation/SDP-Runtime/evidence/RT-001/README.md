# RT-001 — Runtime navigation candidate, 2026-10-06

Scope: PLAN-XFMD-0008 R1/R2, Session 0002 T007/T008, KB-XFMD-025.
This is partial verification of a dirty working-tree candidate, not release,
installation, independent approval or owner acceptance. See candidate.json and
candidate-sources.tar.gz for binary identity and the captured source/test inputs.
The archive includes preceding work; HEAD alone does not identify this candidate.

## Implementation and evidence

The Release build completed. Runtime allocation validates a private user runtime
location and creates a workspace only for generated selections. Viewpoint parents
with children expand instead of requesting a bulk preview. Cumulative 64-context /
256-MiB admission checks were removed; idle cleanup preserves current/history
snapshots and releases unreachable bundles. Requirements and FUNC-032 record the
selected design. No authored SDL/SDUI model files were changed by this increment.

Commands and observed results:

- `cmake --build build-ui -j4`: Release candidate built (compressed build log).
- `ctest --test-dir build-ui -LE GUI -j4 --output-on-failure`: 41/45 PASS.
  DiskFullFailure and PdfFlushFailure cannot use ptrace fault injection; the host
  reports PTRACE_TRACEME Operation not permitted. WindowEndpointBoundaryTest cannot
  create its socket listener. PreferencesTest requires Xvfb despite lacking the GUI
  label and is blocked at Xvfb startup. These failures are not passing evidence.
- SdpRuntimeTest PASS: private unique directories and invalid/missing/relative/
  permissive/symlink path rejection. SdpIntegrationTest PASS against the producer.
- T008 retry: `ctest --test-dir build-ui -R
  '^(SdpRuntime(Test|GuiTest)|SdpNavigationGuiTest|SdpDynamicGuiTest|SdpPanelGuiTest)$'
  --output-on-failure`: 1/5 PASS (SdpRuntimeTest), four GUI tests fail before
  application assertions because Xvfb cannot bind its socket. A direct AF_UNIX
  bind probe under /tmp returns errno 1, Operation not permitted.

SdpRuntimeGuiTest compiles and specifies 105 selections, lazy discovery/parent
activation, runtime failure preservation, history retention/eviction and cleanup.
Those GUI assertions have NOT executed successfully in this environment. Existing
real-producer native navigation and cancellation regressions are also unverified
for this candidate. Tests isolate XDG_RUNTIME_DIR under /tmp; they do not certify
actual /run/user/1000 runtime behavior. No permission restrictions were bypassed.

Documentation/structure checks rerun after the T008 checkpoint: KanBan validator
PASS (26 cards, 106 card events, 14 management records); blueprint validator PASS
(38 blueprints, 75 requirements); symbol check PASS (297 implemented callees);
layer check PASS; git diff --check PASS. These checks do not prove GUI behavior.

## Remaining work

Run focused native tests and affected fault-injection/IPC regressions in an
environment that permits their required sockets/ptrace, inspect native behavior,
then package/install the verified candidate. The installed MM-001/MM-002 binary
remains unchanged. Earlier 85/85 evidence applies only to that older candidate.

Producer selection still has its 24-diagram limit; per-bundle 128-file/32-MiB and
other protocol/renderer limits remain. Parent expansion avoids that broad request
in the tree, but does not remove the producer limit for explicit broad links.
Keep #025 active. Do not start a new Session or #026 on the strength of this partial
increment. Upstream SDP-vNow, installed files and Git writes are unavailable under
the current host permissions.
