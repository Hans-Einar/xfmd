# SN-001 — First native SDP navigation increment

Date: 2026-10-01. Plan: [PLAN-XFMD-0006](../../Plan.md), phase P058,
Session SESSION-XFMD-0001. Base HEAD 41fa4948c5377edd792ba3f503e6b92b0fd4717a plus
an uncommitted working tree, including preserved earlier modeling/process work.
The candidate manifest identifies the actual native inputs, producer and binary;
this is not evidence for an unchanged commit, release or installation.

## Implemented outcome

The native application now composes a conditional SDP tab containing KanBan,
SDL and SDUI trees. Application-owned jobs execute host-selected gh sdp or a
standalone SDPTool. Discovery uses sdptool/0.2 and its typed targets, not an SDL
parser, guessed model IDs or navigation.json. The graph is materialized lazily;
references/depth/row counts are bounded. Cards use the document-opening service;
selected diagrams/frames generate a private Markdown/resource bundle.

Root changes/new selections cancel process groups and invalidate pending results.
Generated delivery checks the document token, dirty state, revision and output
containment. Accepted bundles survive window history and rendering, with bounded
retention. The process adapter closes inherited descriptors, bounds stdout/stderr,
retains the child identity through cleanup and enforces a deadline. Existing
explicit DocumentViews navigator/IPC behavior remains separate.

These are implemented paths, not a claim that native interaction acceptance passed.

## Executed checks

| Check | Actual outcome | Scope / limit |
| --- | --- | --- |
| CMake native build in build-ui | Passed, including xfmd and SdpNavigationGuiTest | Compilation/linking; no installation |
| SdpProtocolTest | Passed | Schema, malformed JSON, graph identity/reference errors, control characters, exact argv/Unicode, inherited-descriptor closure, closed-stream timeout, output bounds, missing executable, error envelope, timeout, child/descendant cancellation, path containment |
| SdpIntegrationTest with installed SDPTool 2.0.0 d304261 | Passed | Real discovery, validated/invalid sources, ledger-backed KanBan, SDL diagram generation, explicit SDUI frame, stale revision, standalone preview producer capability; full XFMD project snapshot decoded |
| Default gh sdp path with SDP_OFFLINE=true | Passed | Same real integration suite plus current XFMD project discovery; verified cached engine, no network fetch |
| Non-GUI regression selection | 36 passed of 38 attempted | DiskFullFailure and PdfFlushFailure could not run their injections: sandbox denies PTRACE_TRACEME. See raw log; do not count these as passing |
| Focused C++ ASan/UBSan adapter tests | Passed on the final adapter sources | Rust static archive is not sanitizer-instrumented; leak detection disabled because ptrace is unavailable. Does not cover FOX/bundle lifetime |
| SdpNavigationGuiTest | Compiled; execution blocked before test entry | Xvfb cannot bind a listener in this sandbox. No native screenshot, pointer/keyboard, dirty-delivery, multi-window or GUI-lifetime pass is claimed |
| Blueprint metadata/links/symbols and layer boundaries | Passed | Structural checks, not independent design review |
| Canonical model generation/verification | Passed | 38 blueprints, 11 current use cases, 73 requirement identities, 22 existing scenarios, six SDUI concepts, 248 generated outputs; structural evidence only |

The broad regression run preceded the final subprocess refinement (close inherited
FDs, avoid closed-stream busy polling, keep the leader unreaped until group cleanup).
The protocol and real-producer tests were rerun after that refinement; a later
protocol fixture extension also passed. Final adapter ASan/UBSan used the final
sources. Other passed regressions exercise unchanged behavior; no repeat of the
entire suite on a final commit is claimed.

Regression selection and output are retained beside this record. Thirty-nine
socket/GUI tests were excluded from the non-GUI run, including the separately
attempted new native test. Their exclusion is a limitation, not a pass.

The initial integration fixture incorrectly used noncanonical declaration order,
omitted the required KanBan board/ledger, selected a collection instead of a
concrete diagram, and made a noncanonical source edit. Tests were corrected to
exercise the producer's actual documented contract; production code did not
weaken validation to accommodate those fixtures.

## Reproduction

Use the installed compatible standalone tool, not the older ~/.local/bin/sdptool
that speaks schema 0.1 on this host. The exact executable hash is in the manifest.

```sh
cmake -S . -B build-ui -DSDP_TOOL_EXECUTABLE=/absolute/path/to/compatible/sdptool
cmake --build build-ui -j4
ctest --test-dir build-ui -R '^Sdp(Protocol|Integration)Test$' --output-on-failure
# Requires permission to create local X11 sockets; isolated from the owner's desktop.
XFMD_UI_EVIDENCE=/tmp/xfmd-sdp-ui-evidence ctest --test-dir build-ui -R '^SdpNavigationGuiTest$' --output-on-failure
python3 tools/validate_blueprints.py
python3 tools/check_blueprint_symbols.py
python3 tools/check_layers.py
python3 SDP/02--Requirements/XFMD-Modeling/build_inventory.py
python3 SDP/05--Implementation/XFMD-Modeling/verify_model.py
```

The existing build reuses the project's pinned cmark/MicroTeX dependencies. A new
sanitizer CMake directory first lacked cmark's pkg-config path and then could not
fetch MicroTeX in this network-restricted host. Focused adapter sanitization was
therefore compiled directly from the same C++ sources and existing locked Rust
archive; this is not a successful full sanitizer build.

## Remaining acceptance

Run the native test and inspect real screenshots. Extend graph-boundary, link and
Back/Forward, multi-window, output-rejection and shutdown tests. Automatic file
watching/refresh, expansion restoration and #015 native .design source/preview
remain open. Detailed new SDL behavior scenarios and independent review remain
open; existing model evidence has not been relabeled as native behavior evidence.
Git metadata is read-only, so no branch/commit was created. No install/push/merge.

[SDUI concept](sdui-concept.png) was rendered from the generated SVG with rsvg-convert
and visually inspected for readable regions/no clipping. This is a conceptual
SDUI artifact, explicitly not a screenshot of the native FOX implementation.
