# N3c — Native use-case profile

Selected by Session0005 T003 continuation of PLAN-XFMD-0009 / KB020 and KB028.
This is a bounded renderer capability, not a delivered SDL aggregate projection.
Official syntax checked 2026-10-08:
https://mermaid.js.org/syntax/usecase.html

XFMD owns strict parsing and family23 domain records. The fork owns glyphs,
measurement, layout and SVG; the existing native/PDF consumer remains unchanged.
Add native UseCase kind, ellipse and actor glyphs in the fork, reusing its graph
placement and bounded semantic routing. This preserves UML notation without
introducing browser execution or translating authored source into flowchart text.

Initial supported profile: usecase-beta; direction TD/TB/BT/LR/RL; explicit actor
IDs with optional plain labels; explicit use-case IDs with ellipse/rectangle
labels; one-level explicitly identified systemBoundary blocks; top-level solid
associations (--, -->, <--), include/extend (..> : include/extend), and
specialization (--|>). Forward references resolve after declarations; undeclared
endpoints become ellipse use cases. Actor identity is never inferred. Labels use
the existing plain-text restrictions; reject unsupported metadata, styles, icons,
notes, quoted-only generated IDs and trailing syntax with explicit diagnostics.
No unsupported statement may disappear silently. Empty boundaries and separator-only
`---` labels are rejected; the shared native text renderer reserves that label.
This is a documented subset, not full Mermaid 12 compatibility.

Records: 210 element [id,label,kind(actor/usecase),shape(ellipse/rectangle),boundary];
211 boundary [id,label]; 212 relation [from,to,kind(association/include/extend/
generalization),arrow(none/forward/reverse),label]. Direction retains tag0.
Validate field counts, plain labels, unique shared IDs, references, membership,
endpoint kinds and direction independently at the contract boundary. Include and
extend require two use cases; generalization requires matching element kinds.
A label saying include is still an association. Preserve source ordering and
isolated elements. Family23/records210–212 are additive to the existing model-wire3;
older consumers reject unknown family rather than misinterpret it.

Actors get a measured stick glyph above their label; ellipses enclose measured
labels. Boundary titles and all members remain visible. Association markers,
dashed include/extend with stereotypes and hollow generalization triangles retain
direction. Shared preview/PDF SVG is acceptance, plus typed roundtrip/rejection
checks. Test Unicode, forward references, isolated elements, all relation kinds,
multiple boundaries and wrong endpoint kinds. Keep current budgets and cancellation.

Producer handoff: SDL VP01 must emit every source-supported actor/use case,
including isolated declarations, and only modeled relations. System boundaries
require actual ownership facts. Functionality realization remains a separate
companion view. overviewTarget remains an unimplemented proposal under KB-SDP-032;
no consumer guesses or upstream implementation is selected by this renderer step.

## Attachment and containment correction

Initial native inspection rejected full-box actor attachments: relationships ended
in empty space around the stick figure. UseCase routing now selects ports on
visible head/arm/leg strokes and treats its caption as a separate obstacle, even
for incident edges. Both bounded and unbounded diagrams use Libavoid; boundary
titles are obstacles and interiors remain traversable. Existing graph containment
cleanup also applies to UseCase, preserving declared membership and preventing
outsiders from overlapping members. Other diagram-family policies are unchanged.
Four directions, with/without boundaries, are checked in the fork regression.
