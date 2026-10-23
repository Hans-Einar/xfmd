# XFMD model sources

System: XFMD. Runtime: the existing FOX desktop application.
Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Design: [model semantics and source boundaries](../../04--Design/XFMD-Modeling/Model-Semantics.md).

## Current entry and coverage — 2026-10-01

**XfmdDesktop.design** is the current canonical input. It covers all 10 existing
Feature and 28 Functionality blueprint identities and all 11 current use cases.
It contains responsibility/containment, contribution, runtime allocation,
interface consumption, three bounded state records and 22 declared scenarios.
This is complete **blueprint-level structural coverage**, not a claim that every
method, predicate, race, wire field or native behavior is fully modeled or verified.
Full semantic coverage and independent review remain open under #017.

`model-map.json` maps existing IDs to SDL symbols and scenario scope. It is local
verification metadata, not a second language, an adopted Traceability schema or
an alternative authority for SDL facts. Existing requirements and blueprints
remain normative pending explicit reconciliation.

One supported design-core 0.5 input is used because the pinned tool lacks linked
System/source sets. The desktop is the actual runtime Container; Application,
Interpreter and Renderer are source responsibilities. Named units under Application
refine existing responsibilities, not new services/classes. External tool roles
remain outside the desktop. The System boundary is described here, not encoded
by renaming it to Unit. No custom concatenator/import syntax is introduced.
This single-input compromise should be replaced by responsibility-grouped source
files only after a supported composition contract is selected and validated.

Future UC-006/xfw and historical BoxUI reservations remain outside implemented
scope; they are listed explicitly in the baseline and mapping. #014/#015/#019/#020
retain separate delivery scope; #014 now has a development implementation under
PLAN-XFMD-0006. Their open acceptance obligations are not completed by the model.

## What scenarios mean

Channel scenarios are ordered **design examples**, not executable C++ or complete
state machines. Logical messages describe interactions/observations; their shared
`InteractionObservationShape` carries a design-time context annotation. It does
not define a runtime payload, transport, C++ signature or IPC schema. The actual
ports and XFMD1/SDLVIEW1 wire formats remain in existing contracts/blueprints.
The closed scenario property checks request/result pairing in an example only.

The parser checks ordering, participant/mode permissions and result correlation.
Absence of an action in an example is not a proof that runtime forbids it. Context,
guards, cancellation, interleavings, state effects and evidence limits are linked
from [Model-Semantics](../../04--Design/XFMD-Modeling/Model-Semantics.md).

## Feasible SDUI

`SDUI/XfmdDesktop/Desktop.sdui` provides six conceptual compositions: split,
editor, preview, preferences, publication and SDP project browsing. Editor/preview are clearly labeled
regions. Controls have no SDL/native action bindings; these exports do not load,
edit, save or render actual documents. This meets the owner's latest request to
use SDUI where feasible without requiring full editor/viewer widgets.

`Navigation.design` and `SDUI/XfmdDesktop/Navigation.sdui` are the unchanged XM-M3
pilot inputs. They remain historical reproduction fixtures; do not extend them
as parallel authoritative definitions. Their three old static states/evidence are
preserved, including Files/Index and dirty generated-delivery rejection.

## Identity map

| Symbol | Existing identity / responsibility |
| --- | --- |
| MarkdownPresentation | FTR-001 |
| LivePreview | FTR-002 |
| DocumentNavigation | FTR-003 |
| SynchronizedScrolling | FTR-004 |
| WorkPathNavigation | FTR-005 |
| ConfigurableScrolling | FTR-006 |
| PagedPublication | FTR-007 |
| ChapterNavigation | FTR-008 |
| DocumentReferences | FTR-009 |
| MermaidDiagrams | FTR-010 |
| DocumentSession | FUNC-001 |
| LocalFileStorage | FUNC-002 |
| MarkdownInterpretation | FUNC-003 |
| RenderLayout | FUNC-004 |
| FoxPresentation | FUNC-005 |
| EventScheduling | FUNC-006 |
| PreviewPipeline | FUNC-007 |
| NavigationHistory | FUNC-008 |
| SourceAnchorMapping | FUNC-009 |
| WorkspaceControls | FUNC-010 |
| TextEditing | FUNC-011 |
| WorkPathHistory | FUNC-012 |
| FilteredFileTree | FUNC-013 |
| ApplicationPreferences | FUNC-014 |
| ScrollMotion | FUNC-015 |
| TypographyAndDrawing | FUNC-016 |
| PageComposition | FUNC-017 |
| PdfExport | FUNC-018 |
| ApplicationIdentity | FUNC-019 |
| DocumentIndex | FUNC-020 |
| PreviewTextSelection | FUNC-021 |
| EmbeddedVisuals | FUNC-022 |
| MermaidInterpretation | FUNC-023 |
| DiagramLayout | FUNC-024 |
| DiagramPreparation | FUNC-025 |
| BuildIdentity | FUNC-026 |
| GeneratedDocumentNavigation | FUNC-031 |
| SdpProjectNavigation | FUNC-032 |
| ReadDocuments | UC-001 |
| NavigateDocuments | UC-002 |
| BrowseWorkspace | UC-003 |
| EditDocuments | UC-004 |
| ScrollDocuments | UC-005 |
| ConfigurePreferences | UC-007 |
| PublishPages | UC-008 |
| UseFullscreen | UC-009 |
| ReadDiagrams | UC-010 |
| SelectGeneratedView | UC-011 |
| BrowseSdpProject | UC-012 |

## Generation and evidence

Tools remain pinned to the tracked producer archive
`3b4cff695ecab36a0e897052e3f096bbeb9f3ea0` and XM-M2 binary hashes. No installation
or producer source edit is performed. Run `render_model.py --sdl /path/to/sdl
--sdui /path/to/sdui` from the implementation modeling directory (or its full
repository path), then `verify_model.py`. These use real SDL/SDUI tools.

[Generated view index](../../05--Implementation/XFMD-Modeling/views/xfmd-model/sdl/index.md)
and [XM-M4 evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M4/README.md)
record source/tool/output hashes, source inspection, visual review and remaining
limits. Markdown/Mermaid viewpoints are tool-generated; no diagram is hand-maintained.
SDPTool 2.0.0 discovers these sources and derives model IDs from their paths.
No navigation.json registration is required. The pinned standalone generators
remain the reproduction tools for current model artifacts; the native adapter
separately tests the installed 2.0.0 facade.

FUNC-032 ownership, allocation, protocol consumption and UC-012 are included.
Its asynchronous lifecycle is specified in the blueprint and native tests; the
22 historical declared scenarios do not yet cover this new coordinator.
SdpProjectWorkspace is a sixth descriptive SDUI composition, not GUI evidence.
