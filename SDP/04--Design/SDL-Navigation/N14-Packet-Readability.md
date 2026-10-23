# N14 — readable packet fields

Selected 2026-10-21, Session0007 T010, KB020, PLAN-XFMD-0009. Derived correction
to UR-040 / AT-060 and existing typed Packet profile; no new parser/ABI or SDL
semantics. Preserve authored-model hold and original bit offsets/width ratios.

## Decision and ownership

Mermaid fork `src/layout/packet.rs` owns packet geometry; XFMD FUNC-024 consumes
its layout through existing measured layout and SVG. Do not rewrite SDPTool
packet source or draw application-owned labels. Do not widen individual fields
to their names, which would falsify encoded widths; do not shrink text to fit.

Keep a common bit pitch for the diagram. Measure endpoint numbers at the actual
SVG font size, center each at its bit cell, and reserve measured width plus a gap.
The pitch also accommodates bracketed field ordinal markers. Single-bit fields
have one endpoint. This preserves proportional field widths and separates adjacent
multi-digit endpoints at both ordinary and large font sizes.

Wrap a field label within its segment where possible. If its measured width still
exceeds the segment's padded width, replace its in-cell label with `[n]` and add
an unclipped full label below that row, keyed by the same ordinal and exact segment
bit range. Multiple narrow fields and row-spanning fields remain unambiguous.
The legend may wrap, but an unbreakable word expands its text box and scene bounds.
Each row reserves all legend height before the next row. Labels are measured at
the rendered font size; no separately rendered text or clipping workaround.

## Verification contract

Test the actual N12 three-field repro, neighboring single-bit fields, long
unbreakable names, multi-row fields, high bit numbers, and different font sizes.
Assert proportional geometry, nonoverlapping endpoint boxes, contained text,
legend correspondence and scene bounds. Exercise the native measured bridge,
screen and PDF; inspect rendered artifacts, not only extracted text. Existing
packet/parser and diagram regression tests remain. Broader A4 readability,
VP08 routing budget and discovery/resource guards remain open in KB020/KB025.
