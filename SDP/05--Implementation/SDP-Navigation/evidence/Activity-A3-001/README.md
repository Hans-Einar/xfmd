# Activity A3 native consumer evidence

Observed 2026-11-12. Candidate is XFMD HEAD
`8893a32e15ce3bd3f4e49f694f11264435aa0d4c` plus this isolated worktree's recorded
diff and new files, on `sdp/activity-management`. This is an uncommitted worker
candidate, not an independent review, integrated release or installation claim.
`candidate-source.sha256` pins changed product/test inputs. The existing primary
XFMD worktree's dirty RPM/process changes are excluded.

## Targeted verification

- Isolated CMake Release build of SdpActivityTest, SdpActivityGuiTest,
  SdpProtocolTest, SdpDisplayTest, SdpDiscoveryTest, SdpDynamicGuiTest and
  SdpRuntimeGuiTest succeeds with GCC 14.3.1, FOX 1.6.57 and Rust 1.92.0.
  Existing .deps and previously patched MicroTeX sources supply local dependencies;
  build outputs remain in the isolated worktree. No dependencies were installed.
- `ctest --test-dir build -R '^(SdpActivityTest|SdpActivityGuiTest|SdpProtocolTest|SdpDisplayTest|SdpDiscoveryTest)$' --output-on-failure`:
  5/5 pass. Final test-harness-only changes were rebuilt and Activity GUI rerun.
- `ctest --test-dir build -R '^(SdpDynamicGuiTest|SdpRuntimeGuiTest)$' --output-on-failure`:
  2/2 pass, covering prior dynamic navigation and private runtime resource behavior.
- Blueprint validation and implemented-symbol checks pass; `git diff --check` passes.
  Blueprint checks do not prove C++ call semantics or native behavior.

Activity protocol tests prove model-free view replies, independent expansion
targets, exact-parent/project/revision validation, atomic rejected merges and
preservation of the original graph. Native FOX/Xvfb fixture tests prove Activities
without Sessions, distinct view click and expansion, stale errors/refresh retry,
dirty-buffer preservation and browsing all 5,000 documents in 50 branches by
collapsing each native subtree after visiting it. It does not claim 5,000 rows can
be visible concurrently.

## Real producer / native workflows

Producer `/tmp/sdptool-activity2` SHA-256:
`9ecd85febae6c83eff674f47c71de532a0a8c51e46c7095b1ad4c8f4332a77eb`.
It is a concurrent SDP worktree build, not a released producer. The coordinating
agent must rebuild and qualify the final integrated candidate after source freeze.

The actual producer created the disposable `/tmp/activity2-native-project` through
its typed CLI. Native tests read it without mutation, expanded Planned through
`activity tree`, selected its Activity via native keyboard input and opened the
model-free generated Markdown/SVG bundle. The same native test read real manual
Activity0002 under `/home/warloc/git/SDP/SDP`, expanded Active and selected Activity
management. Both workflows pass. `real-activity-page.md` and inspected screenshots
retain the real project's Activity page and rendered calendar SVG. Test-home bundle
paths in screenshots are temporary and have been reclaimed normally.

Reproduction from the isolated worktree:

```sh
XFMD_ACTIVITY_REAL_PROJECT=/tmp/activity2-native-project \
XFMD_ACTIVITY_REAL_GROUP=Planned XFMD_SDP_TOOL=/tmp/sdptool-activity2 \
XFMD_UI_EVIDENCE=/tmp/xfmd-activity-native-evidence \
python3 tools/run_with_xvfb.py build/SdpActivityGuiTest

XFMD_ACTIVITY_REAL_PROJECT=/home/warloc/git/SDP/SDP \
XFMD_SDP_TOOL=/tmp/sdptool-activity2 \
XFMD_UI_EVIDENCE=/tmp/xfmd-activity-real-project-evidence \
python3 tools/run_with_xvfb.py build/SdpActivityGuiTest
```

## Compatibility and remaining qualification

This candidate supports `activity-view` and `activity-tree`; it does not declare
that released native clients already support them. The prior decoder rejects
Activity deferred groups with the visible `Invalid SDP deferred source` diagnostic.
An older client may leave an unfamiliar nondeferred view target inert. A universal
old-client diagnostic therefore requires producer capability negotiation or an
explicit compatible fallback; it is not proved by this consumer change.

The existing native 4,096 rendered-row and depth protections remain. Collapsing
branches releases widgets and restores the row budget. Every item in a >4,096-item
catalog is reachable when the selected expansion fits those visible bounds;
indefinitely nested continuation pages can still hit the retained depth limit.
No unlimited or fully virtualized browsing claim is made. Producer paging policy
and final large-catalog acceptance remain coordinating A3 obligations.

An existing detection ambiguity treats a selected directory whose basename is
`SDP` as the process area. Selecting `/home/warloc/git/SDP` therefore conflicts
with its nested process area and produces a visible project-root diagnostic;
selecting `/home/warloc/git/SDP/SDP` works. This pre-existing behavior was preserved.
Final integrated review, broader regressions, acceptance and publication remain
separate from these targeted worker gates.
