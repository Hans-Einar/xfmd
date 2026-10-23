# N14 — packet readability and integration review

Session0007 T010, 2026-10-21 / PLAN-XFMD-0009 / KB020.

N14 corrects the N12/KB020 packet renderer defect. The common measured bit
pitch preserves proportions, places endpoint numbers at their bit centers using
the actual SVG font, and separates adjacent multi-digit numbers. Overflow names
use numbered in-cell markers and full per-row legends with exact bit ranges.
Row-spanning fields retain the same ordinal. No producer source rewrite or ABI
change. Renderer commit 49bb0ee is locally committed and adopted via a checksum-
pinned archive; fork publication is separate.

Seven focused fork tests and 24 affected CTests pass (nine GUI tests), including
all existing diagram family/PDF and SDP overview/source/runtime regressions.
Native measured SVG, standalone native Preview and two one-page vector PDFs were
inspected; all field names are visible and the 12/13 PDF glyph boxes have a 5.55pt
gap. Original generated VP10 overview/detail workflow passes with full labels.
The development executable is built. Installed XFMD 0.8 and gh sdp are unchanged.

N5–N13 producer stack 486b29d has clean committed-tree merge forecasts against
main 5ca0e90 and PG 6dfde97. This does not establish independent semantic review
or authorize integration. Neither producer nor PG was changed. Broad KB020
large-print/VP08 routing acceptance, KB025 whole-project discovery/resource guards,
and KB032 eager-export alignment/integration remain open. Proposed next bounded
work: KB025 discovery of the growing project, starting with source/inventory policy.
No authored SDL/SDUI edits, merge, release, install or independent approval.

## Candidate and design

[Selected design](../../../../04--Design/SDL-Navigation/N14-Packet-Readability.md)
was recorded before implementation, refining existing UR-040/AT-060 and FUNC-024.
The isolated fork branch is `feature/xfmd-packet-readability`, based on 55eff5d.
`candidate.json` records actual dirty-consumer sources and binary hashes;
`candidate-sources.tar.xz` preserves source/test/build inputs. It does not claim
the shared XFMD tree equals HEAD. `fork.patch`, `previous-mermaid-source.json`
and `source-verification.log` identify the exact adoption; all 1296 tracked regular
fork files match the clean commit and extracted dependency. Previous dependency
source was preserved. No broad staging or commit of shared consumer work.

The retained N12-001/native screenshots/PDF are the before evidence. Mere PDF
text retention was insufficient there; the new acceptance inspects pixels and
geometry as well as content. Field/header ranges and bit ratios remain unchanged.

## Verification evidence

- `fork-tests.log`: packet_readability_suite (4) and planning_profile_suite (3),
  `cargo test --no-default-features --features libavoid --test packet_readability_suite --test planning_profile_suite`.
  Covers exact N12 repro, 1-bit neighbors, crossing rows, high offsets, large
  unbreakable labels, font sizes 12/16/28, proportions, bounds and number gaps.
- `build.log`, `build-regressions.log`: complete development build and relink.
- `native-layout.log`: actual XFMD measured bridge and all existing layout checks;
  `PacketLayoutChecks.h` verifies normal, split and high-offset regressions.
- `ctest.log`: `ctest --test-dir build-ui -R 'Mermaid|Diagram|Sdp.*Test|SdlSourceGuiTest' --output-on-failure -j4`, 24/24.
- `capture_native.py`, `native_probe.cpp`, `capture.log`: isolated Xvfb native
  application and real ExportPipeline, ordinary Wrap and A4 output.
- `native/packet-narrow.png`, `packet-split.png`, both corresponding `*-pdf.png`,
  `*-svg.png`, PDFs and source Markdown: inspected outputs. The generated VP10
  screen in `overview/vp10-native.png` also visibly preserves the narrow name.
- `check_artifacts.py`, `artifact-checks.log`: PDF number bounds, all names,
  row-crossing names in both segments, one A4 page per fixture and vector-only
  output. These automated checks complement the visual inspection.
- `preflight.json`, `pre-adoption-heads.json`: scoped worktree/dirty-path inventory
  and read-only merge forecasts; no merge was executed.
- `blueprints.log`, `symbols.log`, `ledger-checks.log`: current document checks.

## Verification limits and remaining work

Same-context verification is not independent review or owner acceptance. General
large-print readability and resource-limit removal are not claimed; very long
unbreakable legends may expand the scene, with existing fit/full-size behavior.

The attempted whole-project SDP check is retained in `sdp-checks-initial.log`.
The selected producer's Python validator only accepts manifest schema 1/2, while
the pre-existing installed receipt uses schema 3; it also rejects historical
`Scope` release-note headings. Neither file was changed here. Do not rewrite the
installation or historical release notes to make this old checker pass. Blueprint,
link/symbol and targeted ledger schema/lineage checks are reported separately;
whole-project SDP validation is not claimed passed.
