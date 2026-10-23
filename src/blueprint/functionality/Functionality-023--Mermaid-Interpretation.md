---
id: FUNC-023
kind: Functionality
audience: System
role: Service
owner: interpreter
status: Implemented
scope: FirstRelease
requirements: UR-047, UR-039, UR-041, SR-021, SR-022, SR-023
uses: none
---

# Functionality-023: Mermaid interpretation

## 1. Purpose and scope

Convert supported Mermaid into XFMD's diagram model. This interpreter service
knows no fonts, layout, FOX, Cairo, filesystem or renderer.

## 2. Requirements and acceptance

UR-039, UR-041, UR-047; SR-021, SR-022, SR-023; AT-059, AT-061, AT-062, AT-063, AT-064, AT-076.
See [design/contracts](../../../docs/design/mermaid-integration.md) and
[requirements](../../../xfmd_requirements.md).

## 3. Contracts and ownership

IDiagramInterpreter::parse consumes DiagramSource and returns DiagramParseResult.
DiagramModel owns ordered nodes, edges, groups and direction. SemanticBlock keeps
source/spans for diagnostics; layout receives no original source. C++ copies
owned values; Rust ABI results are released by their matching free functions.
Flowchart 1 remains the adapter/cache profile; typed profiles extend the model.

## 4. Behavior, state and failures

Markdown recognizes explicit Mermaid fences. Standalone Mermaid snapshots are
classified by Application and parsed directly as one source-anchored block.
CmarkInterpreter skips an optional BOM only for parser input; original bytes and
identity remain in the snapshot. No synthetic fence or offset translation is
written to the document. Valid input becomes a Diagram; unsupported/invalid input
retains source and local diagnostics. This extends input admission, not grammar.

Rust uses parse_mermaid_strict plus XFMD profile checks to reject silent loss.
Init/CSS/HTML/callback/resource support is outside the initial profile. Precise
node/edge source spans are unavailable; whole-block mapping is Approximate.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | Fence metadata/literal/range to diagram source | Keep original fallback | Implemented |
| 2 | `IDiagramInterpreter virtual dispatch` | `MermaidInterpreter::parse` | `src/interpreter/mermaid/MermaidInterpreter.cpp` | DiagramSource to DiagramParseResult | Check limits and profile | Implemented |
| 3 | `MermaidInterpreter::parse` | `xfmd_mermaid_parse_v1` | `src/application/composition/mermaid/src/lib.rs` | Borrowed UTF-8 to owned ABI result | Panic/error becomes status | Implemented |
| 4 | `xfmd_mermaid_parse_v1` | `parse` | `src/interpreter/mermaid/rust/src/lib.rs` | UTF-8 to profile/upstream/model | Return result to ABI | Implemented |
| 5 | `parse` | `inspect` | `src/interpreter/mermaid/rust/src/profile.rs` | Source to admitted Flowchart 1 | Reject unknown syntax | Implemented |
| 6 | `parse` | `map_graph` | `src/interpreter/mermaid/rust/src/model.rs` | Upstream Graph to pure model | Validate enums and retained nodes/edges | Implemented |
| P36 | `parse` | `parse` | `src/interpreter/mermaid/rust/src/sequence.rs` | Sequence 1 to ordered events | Reject unknown syntax/scope | Implemented |
| P36 | `Sequence::read` | `validate` | `src/contracts/diagram/rust/src/sequence.rs` | Typed wire to validated sequence | Reject invalid values | Implemented |
| P38 | `parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/mod.rs` | Source to typed state/class/requirement/ER | Reject unconsumed syntax | Implemented |
| P38 | `Diagram::read` | `validate` | `src/contracts/diagram/rust/src/semantic.rs` | Wire model to schema/identity/scope checks | Fail before layout | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/c4.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/architecture.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/block.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/planning.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/charts.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/trees.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/git.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| P39–41 | `semantic::parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/zenuml.rs` | Source to typed semantic records | Reject loss or unknown syntax | Implemented |
| MM-001 | `CmarkInterpreter::parse` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | Raw standalone source and original range | Shared profile validation and source fallback | Implemented |

## 6. Reuse and dependencies

FUNC-003 consumes the injected port. Library and Rust types are private. There is
no dependency on FUNC-024; another parser can produce the same model without
changing Application or diagram layout. Native preview and PDF share preparation.

## 7. Verification

Rust profile/FFI, DiagramLayoutTest and DiagramPreparationTest cover fixtures,
directions/shapes/groups, rejected syntax, bounds, panic and ownership. Historical
native/PDF evidence and limits: [P29](../../../docs/evidence/P29.md),
[P36](../../../docs/evidence/P36.md). StandaloneMermaidTest and the native test
cover raw input, source preservation, format invalidation and actual rendering;
current results belong to PLAN-XFMD-0007 / MM-001.

## 8. Status, risks and change impact

Implemented in P26/P27; full profile validation rejects lost nodes/edges/groups.
P36/P37 add ordered Sequence events, asynchronous arrows and sequence_events::apply.
P38 adds typed state/class/requirement/ER and schema/reference validation:
[semantic contract](../../../docs/design/mermaid-semantic-model.md).
P39 adds C4/Architecture/Block boundaries, ports and grid:
[authoring profile](../../../docs/design/mermaid-architecture-authoring.md).
P40 preserves bitfields, periods, dependencies and scores through typed planning
records. P41 adds chart/tree/git/ZenUML records with coordinate/hierarchy/reference
validation. [Current coverage](../../../mermaid_coverage.md) retains profile limits.

MM-001 adds standalone input through the same port. It makes no broader grammar
or fidelity claim; #020 retains that separate study. Model sources remain held
by the owner. Implemented is not an independent acceptance disposition.

### SDUI nested-composition compatibility — Session 0004 T002

Native layout may reorder independent nested groups. Match returned group IDs to
model identities, reject missing/unknown/duplicate IDs and retain model order in
the inspection scene. Do not reject valid nested composition solely for a changed
wire order. DiagramLayoutTest includes independent nested row groups; actual
native composition must render before the 0.7 SDUI package is accepted.

### Quoted mindmap labels — Session0005 N3a

The existing supported circle/rectangle/rounded node forms must retain quoted
plain-text labels, Unicode, explicit IDs and parent identity. After a shaped node
has been parsed, do not also validate the complete source line as an unshaped
label: quotes belonging to the shape syntax are not part of the plain label.
Select the fallback only when no shape matched. Preserve text-content rejection,
shape closure checks and existing hierarchy limits; no new Mermaid dialect or
raw rendering path. Verify decoded semantic records and native preview/PDF.

### N3c — native use-case increment

[Use-case profile](../../../SDP/04--Design/SDL-Navigation/UseCase-Profile.md)
defines family23/records210–212 and acceptance. Implemented: strict usecase-beta parser
→ independently validated domain records → native fork UseCase graph/glyphs →
existing measured SVG/native/PDF path. No original source crosses the layout port.
The fork owns geometry and markers; XFMD owns syntax, semantic validation and
conversion. Producer overview selection is not delivered by this profile.

Plumbing: `src/interpreter/mermaid/rust/src/semantic/usecase.rs` emits element,
boundary and relation records; `src/contracts/diagram/rust/src/semantic_usecase.rs`
independently checks fields, identities, membership, endpoint kinds and labels.
`src/renderer/diagram/rust/src/semantic/usecase.rs` maps validated records to the
fork's UseCase graph. `compute_semantic_layout` routes real element endpoints with
Libavoid, including boundaries; actor ports attach to glyph strokes and avoid
captions. Unsupported syntax returns a diagnostic, never a partial diagram.
Source remains inside the interpreter; the existing SVG and native/PDF adapter
consumes measured output. See N3c-001 for candidate-specific verification.
