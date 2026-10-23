# N2-001 — XFMD 0.6 navigation implementation and installation

Owner Session 0003 T003, 2026-10-07, authorizes implementation, Release build and
local installation as 0.6. PLAN-XFMD-0009 N2 is delivered. This does not close
its producer-dependent N3/N4 or the full #028 request. No publication/tag,
independent review or authored-model change is claimed.

## Delivered behavior

- SDL source/System children present canonical viewpoints without healthy phase/
  abstraction wrappers or duplicate A-level routes. Unknown groups and source
  diagnostics remain visible. Valid references retain their original contextual
  target instead of replacing it with generic VP11.
- Original graph identity, canonical node and displayed occurrence key stay distinct;
  lazy expansion and refresh restore surviving context. Valid cyclic cross-links
  become terminal actionable leaves. Inert cycle/depth markers disappear; genuine
  truncation and invalid-source diagnostics remain visible.
- Native expansion-box clicks do not dispatch generation. Older producer parent
  activation expands with an explicit unavailable-overview message, not bulk output.
- Includes the earlier runtime allocation/on-demand/history cleanup increment,
  raw Mermaid file support and local heading links. Files/Index/SDUI/card behavior
  remains covered by the regression suite.

## Exact candidate and package

Version: xfmd 0.6 sprint/009/phase/057-favorites:206+dirty
HEAD baseline: 41fa4948c5377edd792ba3f503e6b92b0fd4717a plus dirty working tree.
Binary SHA256: ead78e401c4d28e85cf624024caa2cabee8a4f765a6cdd3904b4279037ca4c84
Package SHA256: 3bb8652c75fe6b18bb5593ffb946a1b8ca6448ec6c7f55aff92586328ead72e3
Package: /home/warloc/git/xfmd-sdl-navigation/build-ui/release-0.6/xfmd-0.6.0-Linux.tar.gz

candidate-sources.tar.gz captures current product/CMake/tests/tools/packaging and
license inputs; source-archive.sha256 identifies it. It includes preceding work,
not solely this increment. xfmd-build.json records development identity. No source
commit is claimed for the combined dirty candidate; shared staged work is preserved.
Package extraction matches the build binary; linked system libraries were checked.
package-files.json records all 168 packaged file hashes.

## Verification

`cmake --build build-ui -j4`: optimized Release build PASS.
`ctest --test-dir build-ui -j4 --output-on-failure`: **88/88 PASS**, including
42 GUI tests. Full suite includes real-producer navigation/source/SDUI, dynamic
sidebar/context actions, version, fault injection, IPC, Mermaid/PDF, document
history/dirty behavior and the new display projection unit test.

SdpRuntimeGuiTest now actually passes: 105 selections, no generation during
initial discovery/parent expansion, invalid-runtime preservation, private runtime
output, history survival/eviction, idle cleanup and window destruction. Tests use
isolated private XDG_RUNTIME_DIR under temporary HOME; no claim that every host
runtime filesystem is RAM-backed. Prior RT-001 sandbox failures remain historical.

Initial focused failures were test setup issues: horizontal scrolling hid the
expansion box; the previously unexecuted runtime test held rows before initial
watch/refresh reconciliation settled. Corrected box hit testing, focus and initial
settling without removing behavior assertions. Failed and successful logs are
retained; final full-suite evidence supersedes those attempts for this candidate.

Native screenshots inspected: sdl-clean-tree.png (deduplication, contextual leaf,
no A wrapper) and sdp-sdl-view.png (real generated diagram and runtime path).
Additional source/SDUI screenshots retained from real-producer checks. This is
same-context verification, not independent approval.

Documentation checks PASS: board schema/replay/placement, blueprint matrix/links,
implemented plumbing symbol existence (301), layer boundaries and diff whitespace.
These structural checks do not replace the native evidence above.

## Installation

Installed successfully over observed 0.5. All 168 installed files match
the package; isolated native startup with a raw Mermaid fixture PASS. Existing
user windows were not restarted or manipulated. New processes use this executable.
Receipt: /home/warloc/.local/state/xfmd/installations/20261006T221509Z/receipt.json
Previous files: /home/warloc/.local/state/xfmd/installations/20261006T221509Z/previous

Local copies receipt.json, installed-smoke.log and install.log retain evidence.
install.py performed checksum checks, backups and per-file atomic replacement
with rollback on failure. No rollback was needed.

## Remaining scope

PLAN-XFMD-0009 N3/N4: producer aggregate overview contract/projections, proper
native use-case notation, VP07 rendering defect, source-supported sequence/condition
links and exact source-line highlighting. The supplied libavoid error is not fixed
by tree cleanup. SDUI combined page/launch and toolbar #027/#029 remain later.
#025 still owns producer selection/bundle and other content limits. The default
producer still has the 24-diagram cap; this release does not claim all caps removed.
