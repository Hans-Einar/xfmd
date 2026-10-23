# MM-001 — Standalone Mermaid and local heading-link candidate

## Outcome and authority

Owner Session 0002 T002 authorizes KB-XFMD-023 implementation and a Release build.
T003 resumes that work and adds KB-XFMD-024 local file heading links. Requirements
UR-047/048 and textual FUNC-002/008/023 contracts were selected before code.
Implementation and local packaging are complete; independent review and owner
acceptance remain pending. No publication, tag, merge or installation performed.

## Exact candidate

- Product 0.4; CPack 0.4.0; Release (`-O3 -DNDEBUG`).
- HEAD 41fa4948c5377edd792ba3f503e6b92b0fd4717a, branch
  sprint/009/phase/057-favorites, commit count 206, +dirty.
- [candidate.json](candidate.json) hashes  the product source; the compressed
  [source snapshot](candidate-source.tar.gz) includes tracked/untracked product
  files and [tracked patch](candidate-tracked.patch) captures changes from HEAD.
  Shared prior SDP-navigation changes are part of the build, not newly attributed
  to #023/#024. [baseline.json](baseline.json) records the pre-task workspace.
- [package.json](package.json) records package and binary hashes. Package location:
  /home/warloc/git/xfmd-sdl-navigation/build-ui/release-mmd/xfmd-0.4.0-Linux-e361fdf1ae59.tar.gz
- Binary SHA256: e361fdf1ae5972eedf1af8ffd6e20fade6e15387a79828bea9ca130771aeb371
- Package SHA256: e7cfa978cb4dfebd22396ae2f382be69069d4151b55837ff81de4bbe5fbe66de
- Build host Linux x86_64 / glibc 2.39; system-linked libraries were resolved via
  [ldd](package-ldd.log). This is a host-compatible archive, not a self-contained
  distribution or portability guarantee.

## Verification

| Obligation | Evidence and actual result |
| --- | --- |
| Raw .mmd input | StandaloneMermaidTest: mixed-case extension, BOM, original bytes/range, 5 nodes/4 edges from the owner fixture; no synthetic fences |
| Save and format changes | Raw save preserves bytes; Save As .md/.mmd/.txt recomputes flags and invalidates tokens; malformed/empty source retains diagnostics |
| Native entry points | StandaloneMermaidGuiTest: startup callback used by CLI, dialog callback, actual Files-tree click, history, recent/favorite callbacks, local link, edit/save and dirty Cancel |
| SDP opening | SdpNavigationGuiTest: actual producer project discovery, captured typed open target redirected to a contained .mmd, real scene and Preview mode |
| Heading lookup | HeadingLinkTest: absolute/relative paths, separate escape decoding, literal encoded #, CamelCase/title/slugs, repeated IDs, Unicode preservation, unsafe input rejection |
| Native heading workflow | HeadingLinkGuiTest: actual renderer link click to absolute file#heading, viewport at target; Back/Forward, same-file dirty edits, cross-file Cancel, missing anchor/file, relative link and cancellation of stale pending navigation |
| Render/PDF | Actual DiagramScene in Wrap and A4; PDF export reparses raw source; one-page vector PDF. Native and PDF images inspected for the supplied 5-node/4-edge diagram |
| Regression | Full configured CTest: 85/85 PASS, including 41 GUI tests, in 74.60 s; focused 5/5 PASS |
| Structural checks | Blueprint metadata/matrix/links and symbols, layer boundaries, KanBan replay, git diff whitespace PASS |
| Package | CPack TGZ + SHA256; extracted binary equals built binary hash; --version correct; packaged executable remains running with owner .mmd fixture in isolated Xvfb/temp HOME |

Logs: [full suite](full-ctest.log), [focused tests](focused-ctest.log),
[artifact capture on the final candidate](visual-candidate.log), [build](build.log),
[packaging](package.log), [package startup](package-smoke.log),
[blueprints](blueprints.log), [symbols](symbols.log), [layers](layers.log),
[board](board.log), [whitespace](whitespace.log).
Visual artifacts: [native preview](standalone-preview.png), [PDF](standalone.pdf),
[PDF image](standalone-pdf.png). The supplied fixture remains in tests/fixtures/mermaid.

Commands (from repository root):

```sh
cmake --build build-ui -j4
ctest --test-dir build-ui -j4 --output-on-failure
python3 tools/validate_blueprints.py
python3 tools/check_blueprint_symbols.py
python3 tools/check_layers.py
python3 SDP/KanBan/verify_board.py
git diff --check
cpack --config build-ui/CPackConfig.cmake -B build-ui/release-mmd -D CPACK_PACKAGE_FILE_NAME=xfmd-0.4.0-Linux-e361fdf1ae59 -D CPACK_PACKAGE_CHECKSUM=SHA256
```

Package startup harness: [package_smoke.py](package_smoke.py), run through
`tools/run_with_xvfb.py` with extracted binary, fixture and output-log arguments.
The GUI smoke proves startup/liveness; scene correctness comes from the native
application tests, not from liveness alone. Open-dialog/recent/favorite tests use
production callbacks, not pointer clicks through each corresponding widget.

## Preserved boundaries and remaining review

[preservation.json](preservation.json): all 331 protected model/source/view files
unchanged from the pre-task baseline; staged concurrent #020 byte-identical to
its Git index. Owner's SDL/SDUI model-source hold remains in force, so textual
blueprints are current while model reconciliation remains explicitly deferred.
No invented blueprint diff from frozen model commits is claimed.

No broad Mermaid grammar expansion, .mermaid alias, explicit HTML anchors or full
Unicode case folding is included. Existing .md-only filter and index reference
file-grouping semantics remain documented. Existing profile/fidelity limits
belong to #020. Current fixed-input screenshots do not prove arbitrary diagrams.

Development checks initially caught a swapped SourceSnapshot aggregate in the new
unit fixture and a conflicting local variable in the extended SDP test. Both test
construction errors were corrected before the passing candidate; no acceptance
obligation was removed. SdpNavigationGuiTest now slightly exceeds 300 lines: its
linear real-producer fixture and race scenarios remain together; the extension is
a bounded typed-open assertion rather than a new production responsibility.

The [release checklist](../../ReleaseChecklist.md) distinguishes local preparation
from publication. Same-context verification is not independent review. Cards
#023/#024 remain active / gate-review, plan active, Session S4 awaits review.
