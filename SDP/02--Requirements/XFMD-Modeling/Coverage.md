# Requirement and source-responsibility reconciliation

Parent: [PLAN-XFMD-0001](Plan.md). Baseline: XFMD `41fa494`.
This map assigns all 73 table-defined UR/SR identities to a modeling concern.
It preserves current normative sources rather than replacing them with summaries.
Assignment to a concern is not evidence that its model or acceptance tests are complete.

## Requirements by responsibility

| Concern | UR identities | SR identities | Design owners / intended model area |
| --- | --- | --- | --- |
| SDP project browsing and generation | UR-046 | SR-029 | FUNC-032; producer boundary, graph projection and guarded delivery |
| Open, classify, save, navigate | UR-001, UR-005, UR-009, UR-024, UR-028 | SR-005, SR-006, SR-007 | FUNC-001/002/008/009/010; Desktop document workflow and external launch contracts |
| Read, interpret and lay out Markdown | UR-002 | SR-003, SR-004 | FTR-001; FUNC-003/004/005/016; shared semantic/render value contracts |
| Edit and schedule preview | UR-003, UR-004 | SR-008, SR-010 | FTR-002; FUNC-001/006/007/011; revision and worker lifecycles |
| Sidebar, roots, paths and favorites | UR-006, UR-012, UR-013, UR-014, UR-029, UR-038, UR-045 | — | FTR-005; FUNC-010/012/013; Desktop workspace and persisted path projections |
| Workspace, theme and interaction | UR-007, UR-019, UR-025, UR-026, UR-027, UR-030, UR-031, UR-032, UR-033, UR-034 | SR-018 | FUNC-005/010/011/014; Native desktop responsibilities and preference transactions; feasible SDUI shell with labeled document areas |
| Scroll, anchors and zoom | UR-008, UR-011, UR-015, UR-016 | SR-009, SR-015, SR-019 | FTR-004/006; FUNC-009/014/015 and DocumentZoom; viewport intents and source anchors |
| Page composition and PDF | UR-017, UR-018 | SR-016, SR-017 | FTR-007; FUNC-016/017/018; shared page geometry and export lifetime |
| Index and references | UR-021, UR-022, UR-023 | SR-020 | FTR-008/009; FUNC-020/008/009/010; lazy generation-scoped reference reads |
| Selection, images and mathematics | UR-035, UR-036, UR-037 | — | FUNC-021/022 with interpreter/render contracts; resource limits and preview/PDF parity |
| Mermaid profiles and external library boundaries | UR-039, UR-040, UR-041 | SR-021, SR-022, SR-023, SR-024 | FTR-010; FUNC-023/024/025; typed model, FFI, preparation and external routing |
| Identity, build, layering, performance and design quality | UR-020, UR-042 | SR-001, SR-002, SR-011, SR-012, SR-013, SR-025 | FUNC-019/026 plus shared constraints; build-time tools are not runtime Containers |
| Generated documents and addressed delivery | UR-044 | SR-027, SR-028 | FUNC-031; Desktop endpoint plus external SDL generation/broker contract |
| Future xfw integration | UR-010 | SR-014 | UC-006; future scope, not delivered by FUNC-031 or the pilot |

UC-001/002 concern reading/navigation, UC-003 sidebar, UC-004 editing,
UC-005 scrolling, UC-006 future xfw, UC-007 preferences, UC-008 page/PDF,
UC-009 fullscreen, UC-010 Mermaid and UC-011 generated documents.
Existing AT clauses remain attached to their normative requirements. The inventory
retains occurrence/definition lines and blueprint references; neither an AT mention
nor a row in this map is a passed acceptance check.

## Amendments that must govern model authoring

| Earlier wording | Current governing amendment | Modeling consequence |
| --- | --- | --- |
| Suffix-only document admission | P053 / UR-001/024/028 | Classify content; internal plain text, configured browser and OS handoff are separate outcomes |
| Every open changes root | P053/P054 | Only successful internal chooser opens change parent root; links/recent/typed files preserve root |
| Separate sidebar filter field and fixed toolbar placement | P054 | Shared editable document-path/filter row; theme popup and grouped top toolbar |
| Two type toggles / text button | P056 / UR-014 | One Markdown pill; hide Markdown/Up/Open on Index, retain Refresh |
| Recent files permanently below recent folders | P054 then P057 | Lower resizable tab area: Folders, Files, Favorites; separate persistence |
| Favorites Add/Remove buttons | P057-M2 / UR-045 | Context actions on clicked paths, no navigation on dismissal, no explanatory empty-list text |
| A4-only fit/default zoom | P055 / UR-011/017/033, SR-016/019 | Shared 25–300% manual zoom, default 100%, fit only A4; sidebar Ctrl gestures retain their own meaning |
| Local-image placeholders / no math | P23 / UR-036/037 | Bounded local decoding/typesetting; no automatic network or TeX side effects |
| Native individually selectable diagram labels | P31 onward / UR-041 | One SVG diagram with approximate source anchor; ordinary Markdown selection remains |
| Flowchart is the only Mermaid family | P36–P41 and mermaid_coverage.md | Preserve each supported typed profile and its failure/fallback boundary |
| All layout/paint on one GUI path | P9–P13 and SR-017, P23, SR-022 | FOX stays on GUI thread; export/resource/diagram workers own separate non-FOX resources |
| Every dirty open prompts | UR-044/SR-027 addressed delivery | Reject dirty main delivery without a modal prompt; ordinary interactive open still prompts |

The old sections are historical baselines, not silent alternatives. This map is
the English reconciliation index; translation of their full maintained prose and
all acceptance evidence remains under GAP-XFMD-DOC-003 / EVID-001.

## Source responsibility map

The paths below refer to repository `src/`. Shared headers define contracts;
consuming a contract is not ownership of its implementation.

| Source family | Responsibility / existing functionality |
| --- | --- |
| application/Application*, main.cpp | Composition and workflow adapters: opening, workspace, index, appearance, export and zoom remain delegated to their named services |
| application/document | DocumentSession FUNC-001; admission/save coordination with FUNC-002; EditController/TextProjection support FUNC-011 and UTF-8 preservation |
| application/io | LocalFileStore/InputPolicy/FileOpenPolicy: FUNC-002 plus opening policy; PdfFilePublisher belongs to FUNC-018 export publication |
| application/navigation | HistoryStore/NavigationCoordinator/LinkResolver: FUNC-008/009; DocumentViews/NavigationPanel/WindowEndpoint: FUNC-031 |
| application/preview | ParserWorker/PreviewCoordinator: FUNC-006/007; PreviewSelection: FUNC-021 |
| application/scroll and zoom | AnchorMapper/ScrollCoordinator: FUNC-009/015; ScrollDynamics: FUNC-015; DocumentZoom owns shared P055 state under workspace controls |
| application/workspace | WorkPathHistory FUNC-012; DirectoryScanner/FileNameFilter FUNC-013 |
| application/index | DocumentIndex/ReferenceWorker FUNC-020; no second parser of the active buffer |
| application/preferences | PreferencesService/Appearance/ReadingColors FUNC-014; active versus draft profile and persistence boundaries |
| application/export | ExportCoordinator/ExportPipeline FUNC-018; immutable snapshot and publish/cancel policy |
| application/media | EmbeddedVisuals/ImageDecoder/GifBudget/MathTypesetter/CairoVisual FUNC-022; bounded resource decoding and lifetime |
| application/diagrams and composition | DiagramPreparation/DiagramCache FUNC-025; composition registers parser/layout ports and owns Rust C exports |
| application/build | BuildVersion FUNC-026; runtime reads generated identity, not Git/Python |
| application/ui | Window/toolbar/workspace/path/history/favorites composition FUNC-010/012/013; EditorWidget/EditorPresentation FUNC-011; IndexPanel/NavigationTree FUNC-020; Preferences pages FUNC-014; IconResources FUNC-019 |
| application/ui/controls and style | Shared native control presentation, geometry, icon/theme roles and command binding under FUNC-005/010/014; no new Container per widget |
| application/adapters | FOX host/canvas/input FUNC-005/021; scheduler FUNC-006; window mode FUNC-010; wheel FUNC-015; preferences store FUNC-014; typography/drawing FUNC-016; PdfOutput FUNC-018; browser/desktop opening FUNC-008/010; diagram/SVG presentation FUNC-025 |
| contracts and contracts/diagram | Shared semantic/render/page/visual/diagram ports and values; Rust schema/serialization owns wire representation, not interpretation or layout |
| interpreter | Cmark/ModelBuilder/TableModelBuilder/SourceMapBuilder/MathSyntax FUNC-003; mermaid adapter/crate/profiles FUNC-023 |
| renderer | Markdown/block/inline/table/hit testing/markers FUNC-004; page breaking/composition/anchors FUNC-017; diagram placement/layout/measurement/crate FUNC-024 with ordinary render placement |

All 281 inventoried files belong to these source families, including internal Rust
modules and tests colocated in crates. The 144 missing literal filename mentions
are therefore not 144 missing services. This family-level reconciliation does not
close every function-call/ownership review: the generated-navigation workflow has
received a focused source trace; remaining families retain the verification queue.

## Historical first model coverage

The [navigation workflow](../../04--Design/XFMD-Modeling/Navigation-Workflow.md)
traces actual entrypoints and failure paths. Its
[SDL/SDUI pilot](../../SDL/XFMD/README.md) covers 14 functionality identities and
three features structurally, with three static shell-state compositions.
It does not cover all 37 blueprints, encode all behavioral rules, or satisfy
the full inventory's model/evidence columns. Expansion follows the same explicit
identity/fidelity approach after this bounded pilot is checked.

## Current structural coverage — 2026-09-30

[XfmdDesktop.design](../../SDL/XFMD/XfmdDesktop.design) now maps all 37 existing
blueprints (10 Features, 27 Functionality) and 10 current use cases. UC-006 remains
future scope. The mapping is checked against blueprint identities, declared source
layers and Feature uses/contributions. [Model-Semantics](../../04--Design/XFMD-Modeling/Model-Semantics.md)
explains 22 declared workflow examples, state excerpts and actual port/source owners.
Five supported SDUI concepts illustrate editor/preview regions without implementing
their native functionality. The old pilot remains a historical reproduction fixture.

Every blueprint now has a structural model link. This closes the previously empty
structural inventory cells, not all normative requirement, detailed method, branch,
concurrency or evidence obligations. The all-source semantic review and independent
review remain open; native behavior is not inferred from generated diagrams.

## 2026-10-01 SDP navigation delta

FUNC-032 / UC-012 add application/sdp and SdpPanel to the responsibility map.
The canonical model includes ownership/allocation/protocol facts and a sixth
SDUI concept. This structural update does not establish native GUI acceptance
or claim new executable scenarios. See PLAN-XFMD-0006 evidence.
