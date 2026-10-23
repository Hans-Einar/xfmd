# Mermaid coverage in XFMD

The pre-Session0005 baseline documents **23 bounded diagram profiles**. The current
working candidate adds a 24th: native use-case notation. The installed 0.7 binary
has not been replaced. [N3c evidence](SDP/05--Implementation/SDL-Navigation/evidence/N3c-001/README.md)
identifies the tested candidate; [the gallery](mermaid_evicence.md) retains the
29 earlier practical examples.

Coverage means interpretation, semantic preservation, layout, SVG preview and
vector PDF for the listed constructs. It does not imply full Mermaid-language
compatibility. Syntax accepted by the dependency is not automatically supported
by XFMD. This English revision preserves the earlier profiles and adds N3c.

## Scenarios, states and models

| Diagram / header | Supported constructs | Important limits | Example |
| --- | --- | --- | --- |
| Use case — `usecase-beta` | Explicit actors, ellipse/rectangle use cases, one-level system boundaries, associations, include/extend and generalization; isolated declarations and Unicode labels | Bounded syntax, no notes/styles/icons/business variants; 128 identities and 512 records; explicit semantic endpoint checks | [Native notation](tests/fixtures/mermaid/usecase-native.mmd), [profile](docs/design/mermaid-usecase-authoring.md) |
| Flowchart — `flowchart LR` / `graph TD` | LR/RL/TD/TB/BT; rectangle, rounded rectangle, decision and circle; labels, groups, chains, cycles, self-loops and parallel edges; solid, dotted and thick lines | Limited shapes; no arbitrary styling or interactive directives | [Responsibilities](docs/design/mermaid/service-map.mmd) |
| Sequence — `sequenceDiagram` | Participants/actors, aliases, sync/reply/async arrows, activation, notes and nested alt/else, opt, loop and par/and | Explicit participants; balanced activity at branch boundaries; no inline activation, create/destroy, autonumber or box | [APT import](docs/design/mermaid/apt-import.mmd), [nested sequences](docs/design/mermaid/sequence-nested.mmd) |
| State — `stateDiagram-v2` | Start/end, named states, labelled transitions, composite states, choice, fork/join and concurrent regions | No state notes, local direction or styling; composites use simpler native routing | [Freshness](docs/design/mermaid/measurement-state.mmd), [regions](docs/design/mermaid/state-regions.mmd), [branching](docs/design/mermaid/state-choice.mmd) |
| Class — `classDiagram` | Named types, attributes, operations, annotations, relation text, multiplicities, inheritance, composition, aggregation, dependency and realization | No namespace, generics, callbacks or styling | [Metamodel](docs/design/mermaid/sdl-class.mmd) |
| Requirement — `requirementDiagram` | Requirement ID/text, types, risk/verifymethod, elements with type/docref; contains, copies, derives, satisfies, verifies, refines and traces | Mermaid vocabulary; no automatic connection to running tests or SDL type checking | [APT requirements](docs/design/mermaid/apt-requirements.mmd) |
| ER — `erDiagram` | Entities, attributes, PK/FK/UK, comments, cardinalities and identifying/non-identifying relations | No aliases, styling or database execution | [Provenance](docs/design/mermaid/provenance-er.mmd) |

Sequence diagrams preserve event order and fragment scope. Participants are not
automatically threads, and asynchronous arrows do not determine scheduling. State
diagrams show transitions without evaluating guards. Class and ER diagrams do not
require an object-oriented implementation or a particular database. A drawn
`verifies` relation is not evidence of a passed test. SDL/SDP examples illustrate
candidate concepts; XFMD does not turn them into adopted language rules.

## Architecture

| Diagram / header | Supported constructs | Important limits | Example |
| --- | --- | --- | --- |
| C4 — `C4Context`, `C4Container`, `C4Component` | Person/System/Container/Component, Db/Queue/_Ext variants, boundaries, directed relations, names, technology and description | No Dynamic/Deployment, styles, callbacks, external resources or named arguments; simple native routing | [Context](docs/design/mermaid/c4-context.mmd), [containers](docs/design/mermaid/c4-container.mmd), [components](docs/design/mermaid/c4-component.mmd) |
| Architecture — `architecture-beta` | Groups, services, junctions, L/R/T/B connection sides and built-in cloud/database/disk/internet/server icons | No nested groups, `{group}` edges, align or external icon packages | [Resources](docs/design/mermaid/architecture-resources.mmd) |
| Block — `block-beta` | Columns, rectangular cells, spans, spaces and simple connections | 1–16 columns; one declaration per line; no nested blocks or special shapes | [Horizontal layers](docs/design/mermaid/block-layers.mmd) |

C4 represents system boundaries and responsibilities. Architecture shows resources
and graphical port directions; Block shows explicit layers and grids. None alone
provides the complete semantics of SDL Containers, ports or Channels.

## Encoding, planning and visualization

| Diagram / header | Supported constructs | Important limits | Example |
| --- | --- | --- | --- |
| Packet — `packet` / `packet-beta` | Bits, start–end bit ranges, relative field lengths and labels; proportional widths and 32-bit rows | 4096 bits / 128 fields; no overlap or init directives | [Wire format](docs/design/mermaid/packet-encoding.mmd) |
| Timeline — `timeline` | Title, sections, periods and ordered events, including continuation lines | Categorical time, not a proportional calendar | [Decisions](docs/design/mermaid/timeline-decisions.mmd) |
| Gantt — `gantt` | Title, sections, task IDs, date or `after`, duration and done/active/crit | YYYY-MM-DD, 1970–2100, 1–3650 whole days, one earlier dependency; no excludes or milestones | [Documentation pilot](docs/design/mermaid/gantt-pilot.mmd) |
| Journey — `journey` | Title, sections, ordered tasks, scores and actors | Scores 1–5; no styling | [Document review](docs/design/mermaid/journey-review.mmd) |
| Pie — `pie` / `pie showData` | Title, quoted categories and positive values | 12 categories; no init or styling | [Evidence categories](docs/design/mermaid/pie-evidence.mmd) |
| Mindmap — `mindmap` | Indented tree with plain/rectangle/rounded/circle nodes | Two spaces per level, one root, eight levels; no icons/classes | [Review](docs/design/mermaid/mindmap-review.mmd) |
| GitGraph — `gitGraph` | Commits with IDs, branch, checkout/switch and merge; commit parents preserved | LR layout, explicit commit IDs; no cherry-pick, order, tag or type | [Proposal/review](docs/design/mermaid/gitgraph-proposal.mmd) |
| Sankey — `sankey` / `sankey-beta` | Source/target/value, weighted flows and labels attached to their bands | Positive values, acyclic graph, simple CSV without quoted fields | [Occurrence distribution](docs/design/mermaid/sankey-provenance.mmd) |
| Quadrant — `quadrantChart` | Title, axis ends, four quadrant names and points | Coordinates 0–1; no point styling | [Priorities](docs/design/mermaid/quadrant-priorities.mmd) |
| ZenUML — `zenuml` | Participants, aliases and asynchronous messages using `A->B: text` | No synchronous calls, creation, replies or control blocks | [Observations](docs/design/mermaid/zenuml-observation.mmd) |
| Kanban — `kanban` | Columns and indented cards with ID, text and column membership | No metadata, tickets, assignments or styling | [Document work](docs/design/mermaid/kanban-review.mmd) |
| Radar — `radar-beta` | Title, axes/aliases, curves, min/max and graticule | 3–12 axes, complete positional values; no named values | [Illustrated assessments](docs/design/mermaid/radar-quality.mmd) |
| Treemap — `treemap-beta` | Indented groups and quoted leaves with positive weights | Eight levels; parents have no independent value | [Illustrated effort](docs/design/mermaid/treemap-effort.mmd) |
| XYChart — `xychart-beta` | Title, categorical X axis, Y name/range, bar and line series | Vertical orientation, 1–12 categories, complete finite number series | [Comparison](docs/design/mermaid/xychart-evidence.mmd) |

Packet describes a concrete encoding, not an abstract SDL Datagram family.
Planning and visualization diagrams supplement the design model.

## Shared behavior and limits

- Use a code fence whose first info token is `mermaid`, or open a standalone
  `.mmd`/`.mermaid` file. Ordinary code blocks and `.txt` remain text. Unknown
  syntax, invalid references and exceeded limits produce a local explanation with
  preserved source; the rest of the Markdown document remains visible.
- Plain Unicode is supported. Profiles do not allow arbitrary HTML, CSS, scripts,
  external resources or init directives. Rendering works offline without Node,
  a browser or a routing service.
- Preview supports light/dark mode and reading colors. Category colors remain
  where they distinguish data series or groups. Zoom and window resizing preserve
  aspect ratio.
- PDF uses the same SVG with vector geometry and text. Large diagrams shrink to
  available page space and may be small on A4. Diagram text cannot currently be
  selected separately in preview; source can be copied from the editor.
- Limits: 64 KiB source per diagram, 64 diagrams per document, 8 MiB per scene and
  64 MiB combined scene response. Flowcharts: 128 nodes, 512 edges, 32 groups and
  group depth eight. Sequences: 16 participants, 128 events and eight fragment
  levels. Other family limits are in the profile guides.
- Layout uses a cooperative time budget, not a hard deadline. Lack of space or
  routing errors produce visible fallback. Competing Libavoid ports can produce
  different valid routes; byte-identical SVG is not guaranteed.
- Flowcharts use Libavoid orthogonal routing, with Legacy as an explicit option.
  Crossings are allowed; optional crossing arcs and label leaders are presentation.
  Composite state diagrams and C4 use simpler native routing. Use-case diagrams
  use Libavoid with or without boundaries and preserve explicit relation kinds.

## Detailed profiles

[Flowchart](docs/design/mermaid-integration.md#2-profil-xfmd-flowchart-1),
[Sequence](docs/design/mermaid-sequence-authoring.md),
[State/Class/Requirement/ER](docs/design/mermaid-semantic-authoring.md),
[Use case](docs/design/mermaid-usecase-authoring.md),
[Architecture](docs/design/mermaid-architecture-authoring.md),
[Encoding/planning](docs/design/mermaid-planning-authoring.md),
[Other visualizations](docs/design/mermaid-broad-authoring.md).
The pinned dependency is in the [source manifest](cmake/mermaid-source.json).
Unlisted types or constructs are not claimed as supported.

Open the gallery from the build directory:

```sh
./xfmd ../mermaid_evicence.md
```
