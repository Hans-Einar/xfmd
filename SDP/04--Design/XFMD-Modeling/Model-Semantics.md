# XFMD model semantics and source boundaries

Parent: [PLAN-XFMD-0003](Plan.md). Current model: [XfmdDesktop.design](../../SDL/XFMD/XfmdDesktop.design).
Date: 2026-09-30. Source baseline: XFMD `41fa494` plus identified modeling artifacts.
This document explains model abstraction/limits; it is not a hand-authored generated viewpoint.

## Coverage and authority

All existing blueprint identities have an SDL owner/contribution representation.
Requirements are mapped by [Coverage](../../02--Requirements/XFMD-Modeling/Coverage.md);
future xfw and historical BoxUI scope is not silently implemented. The existing
requirements, amended blueprints and source/test evidence remain authoritative.
SDL core cannot yet carry their complete normative narrative and evidence state.

## Responsibilities and actual ports

| Model unit / interface | Existing responsibility / source |
| --- | --- |
| Documents | DocumentSession, DocumentCoordinator, EditController and TextProjection under src/application/document; native EditorWidget adapts input without owning a second document |
| DocumentStorage / FileStorePort | LocalFileStore read/writeAtomic and input/open policy; files are read before session replacement, successful save updates the baseline |
| NativeShell | FOX host/window/command composition and application UI adapters; delegates document, navigation, preference and export work |
| EventLoop / SchedulerPort | IScheduler and FoxScheduler; debounce and polling lifetime are application-owned |
| Preview / InterpreterPort / RendererPort | PreviewCoordinator and ParserWorker call IInterpreter::parse and IRenderer::layout; no interpreter-to-renderer dependency |
| Navigation | NavigationCoordinator/HistoryStore/LinkResolver; visit commit follows successful open and anchors have independent validity |
| Viewport / RenderHostPort | ScrollCoordinator/AnchorMapper/ScrollDynamics and DocumentZoom; source anchors, feedback suppression and display scale |
| Workspace | WorkPathHistory/DirectoryScanner/FileNameFilter and favorites adapters; paths/history/favorites are not document-session state |
| Preferences / PreferencesStorePort | PreferencesService validates and persists before replacing current preferences; native appearance preview/cancel restoration stays in the UI adapter |
| Typography / TextMetricsPort | ITextMetrics/ITextShaper with SharedTextMetrics, FontCatalog and DisplayListPainter; shared measured/painted glyphs for preview and PDF |
| Publication | ExportCoordinator/ExportPipeline/PdfFilePublisher; frozen snapshot and candidate publication, no implicit document save |
| Identity | IconResources and generated BuildVersion; build-time Git/Python is not a runtime service |
| DocumentIndexService | DocumentIndex/ReferenceWorker; bounded lazy reference reads and generation-scoped completion |
| VisualResources / DiagramLayoutPort | EmbeddedVisuals and DiagramPreparation/DiagramCache; bounded application-owned preparation via IDiagramLayout |
| Interpreter / DiagramInterpreterPort | cmark semantic construction and IDiagramInterpreter; typed Mermaid parsing, no display or native resource I/O |
| Renderer | MarkdownRenderer, PageComposer and typed diagram layout/placement; no FOX or interpreter dependency |
| GeneratedViews | DocumentViews/NavigationPanel/WindowEndpoint; own external generation, pane admission, endpoint sequencing and lease notifications |
| ViewGenerationProtocol | Registered external SDL command/request tool; exec arguments, not a new parser inside XFMD |
| WindowDeliveryProtocol | Existing XFMD1 INFO/OPEN socket protocol; logical channel examples are not its byte schema |
| LeaseReleaseProtocol | Existing SDLVIEW1 RELEASE notification; successful send has no durable broker acknowledgement |
| ExternalOpenProtocol | Existing browser/desktop adapters; logical launcher role, not an XFMD-owned service |

Logical units group responsibility across files. They are not proposed new classes,
processes or a mandatory API per Functionality. Contracts remain shared values/ports;
no independent Contracts runtime is invented. Three Dataset declarations describe
selected state aspects only: document bytes/baseline, generation target/sequence,
and frozen export identity/text. They do not replace complete C++ struct definitions.

## Declared workflow examples

NAV-01–12 correspond to the [source-traced navigation contract](Navigation-Workflow.md).
Additional FLOW examples cover other principal workflows; they are design examples,
not new requirement IDs or tests. Message payloads use a **design observation**
record with textual context so closed Channel scenarios can describe interactions.
That record is explicitly not a newly specified wire/API payload. Each example's
actual types, guards and effects must be read in its governing blueprint/source.

| Case | SDL scenario | Scope / excluded inference |
| --- | --- | --- |
| NAV-01 | FavoriteFileOpens | Clean internal favorite-file example; the work root is unchanged. Classification precedes internal opening. |
| NAV-02 | FavoriteFolderChangesRoot | Root/tree/history change without opening a document. |
| NAV-03 | ContextMenuDismissed | Dismissal ends here; absence of open in this example is not a universal no-open proof. |
| NAV-04 | DirtyOpenCancelled | No read or history commit on Cancel. |
| NAV-05 | SaveThenReadFails | Successful save survives later read failure; old document may now be clean. |
| NAV-06 | ChooserOpensThenChangesRoot | Successful internal chooser case only; folder-only and external handoff have separate rules in the workflow. |
| NAV-07 | EditedPreviewPublished | Abstracts worker/resource preparation; document token, worker ticket and FrameKey checks remain distinct. Does not prove scheduling/timing. |
| NAV-08 | GeneratedSelectionSupersedes | Accepted selection with an existing child; validation precedes cancellation. Start is not a successful-delivery claim. |
| NAV-09 | DirtyGeneratedDeliveryRejected | Logical addressed-delivery interaction; dirty main rejection has no unsaved dialog. Events do not encode a guard predicate. |
| NAV-10 | InvalidSelectionRetainsChild | Validation failure ends before cancellation; cannot itself prove absence of later effects. |
| NAV-11 | GeneratedReplacementReleasesOldLease | Clean successful leased delivery; read/replace is abstracted. Release is a notification, not broker acknowledgement. |
| NAV-12 | ReleaseRetryAfterSendFailure | First send fails, later send succeeds. No durable ACK, crash recovery or exactly-once guarantee. |
| FLOW-13 | PreviewResourcesPrepared | Pipeline collaborator abstraction: application worker prepares resources before current-ticket publication. No FOX work occurs in the parser thread. |
| FLOW-14 | StalePreviewDiscarded | ParserWorker checks ticket/stopping and PreviewCoordinator checks DocumentToken; conditions and interleavings are not executable SDL predicates. |
| FLOW-15 | PreviewScrollSynchronizesEditor | Valid split-view case; programmatic, stale, unavailable and feedback events are rejected by ScrollCoordinator. |
| FLOW-16 | PreferencesPersistBeforeAdoption | Successful commit ordering; validate/save failure leaves PreferencesService.current unchanged. UI appearance preview/cancel restoration is a separate host path. |
| FLOW-17 | PdfSnapshotPublished | Uncached success case. Frozen token/paper/fonts, separate contexts and atomic publication; export never marks DocumentSession saved. |
| FLOW-18 | PdfCancelledBeforePublication | Before-commit case only; a completed commit cannot be reported as cancelled. The locking/cleanup contract requires native evidence. |
| FLOW-19 | ReferenceHeadingsLoaded | Bounded lazy reference load; current-generation results only. No active-buffer reload. |
| FLOW-20 | DiagramScenePrepared | Typed parse/layout separation, no renderer source parsing. Per-worker cache, font identity and block-local fallback remain governing contracts. |
| FLOW-21 | ExternalTargetHandedOff | Configured browser/OS handoff; no shell interpretation or implicit work-root replacement. |
| FLOW-22 | FullscreenChangesPresentation | Native presentation change does not own document parsing or persistence; resize may request normal relayout. |

## Semantic inspection and remaining proof

The navigation contract traces actual source entrypoints; this continuation also
read ParserWorker, PreviewCoordinator, DocumentCoordinator, NavigationCoordinator,
ExportPipeline/ExportCoordinator, PreferencesService, ReferenceWorker,
ScrollCoordinator and EmbeddedVisuals. The evidence source manifest pins these
inputs. Blueprint-derived structural ownership across the remaining inventory
has not received equivalent full call-graph review.

Model checks establish example consistency, not C++ execution. In particular:

- NAV-05 saves before reading the candidate; failed read can leave the old document
  clean. NAV-10 rejects invalid selection before cancelling a valid active child.
  Dedicated current native regression evidence for both remains pending.
- NAV-07 and FLOW-13 abstract parser workers/resources. Worker ticket, DocumentToken
  and FrameKey are different identities; no single generic revision replaces them.
- NAV-11 abstracts successful read/replacement. Document change queues the previous
  lease before the new lease is installed. NAV-12 only describes best-effort send.
- FLOW-16 is the service's successful persistence path; appearance preview rollback
  is a separate native concern. FLOW-17/18 distinguish cancellation before publish
  from a committed export; output does not change dirty state.
- Scenarios cannot express all predicates, loops, thread interleavings, quantified
  absence of effects or exact C++ bindings. Named closed examples are not a complete
  protocol or proof of full behavioral coverage. GAP-XFMD-SDL-003 remains open.

## SDUI and basic text input

The owner accepts labeled editor and Markdown-view regions; a functioning editor
is not needed for modeling. Desktop.sdui therefore uses existing frames/Markdown
and basic controls rather than inventing textarea syntax or native bindings.

Fyne provides `widget.NewMultiLineEntry` and Entry text/change callbacks:
[official API](https://docs.fyne.io/api/v2/widget/entry/). The inspected producer
host already wraps `widget.Entry` for SDUI `input` in
`SDUI/go/host/fynehost/input.go`; `view.go` maps values and OnChanged but exposes
no multiline SDUI property. Thus Fyne can supply basic editing, while SDUI still
needs a selected property/layout/event contract to expose it. Opening/saving remain
explicit domain/application actions, not automatic effects of an editing widget.
This is a bounded extension proposal under GAP-XFMD-SDUI-005, not producer work
selected by the XFMD modeling assignment. The full editor/host-surface proposal
is optional future scope. Basic model-backed Fyne preview need not reproduce XFMD.

## 2026-10-01 — Native SDP browsing delta

PLAN-XFMD-0006 adds FUNC-032/SdpProjectNavigation, owned by SdpProjects under
Application and allocated to XfmdDesktop. It consumes SdpToolProtocol and the
existing SchedulerPort; BrowseSdpProject is UC-012. This is separate from the
legacy GeneratedViews coordinator and its broker lease protocol. The executable
producer still owns SDL/SDUI interpretation; no parser enters XFMD's application.

The canonical structural model and model-map include these identities. The new
SdpProjectWorkspace SDUI entry uses descriptive regions for the native project
tree and generated document. It does not implement selection, editing or process
execution. Existing 22 declared scenarios are preserved; detailed protocol fields,
process cancellation and the new document-token admission lifecycle remain defined
by FUNC-032 and its tests until new scenarios are selected. GUI test compilation
is not evidence that these native interactions passed.

## T002 implementation refinement — automatic navigation refresh

SdpWatch refines the existing application-owned SdpProjects responsibility; it is
not a new container or language parser. Background metadata changes trigger
coalesced producer discovery. SdpPanel restores surviving IDs and keeps source
documents and generated snapshots separate. Native tests now cover automatic
card moves with dirty-buffer retention, graph bounds and generated Back/Forward.
The 22 existing declared SDL scenarios still do not encode this lifecycle;
PLAN-XFMD-0006 M5 retains that semantic modeling obligation.
