# Type inventory — A0

[Up](index.md)

The same model ID is used at every level; object levels are unspecified.

## actor (1)

### ReaderAuthor


## channel (40)

### ContextInteraction

### DiagramInterpretation

### DiagramLayoutCalls

### DocumentCalls

### DocumentNotifications

### DocumentStateChanges

### EditIntents

### ExportInterpretation

### ExportLayout

### ExternalOpening

### GeneratedAdmission

### GeneratedDelivery

### GeneratedDocumentCalls

### GeneratedIntents

### GenerationLifecycle

### IndexChanges

### IndexIntents

### InterpretationCalls

### LayoutCalls

### LeaseLifecycle

### LeaseNotifications

### NavigationCalls

### OpeningPolicy

### PreferenceChanges

### PresentationEvents

### PreviewAdmission

### PublicationDrawing

### PublicationInputs

### PublicationIntents

### PublicationState

### ReferenceInterpretation

### ReferenceStorage

### ResourcePreparation

### ScheduledWork

### StorageCalls

### UnsavedAdmission

### ViewportChanges

### WindowPresentation

### WorkspaceChanges

### WorkspaceIntents


## container (1)

### XfmdDesktop


## contract (44)

### ContextInteractionContract

### DiagramInterpretationContract

### DiagramLayoutCallsContract

### DocumentCallsContract

### DocumentNotificationsContract

### DocumentStateChangesContract

### DocumentStateShape

### EditIntentsContract

### ExportInterpretationContract

### ExportLayoutContract

### ExportSnapshotShape

### ExternalOpeningContract

### GeneratedAdmissionContract

### GeneratedDeliveryContract

### GeneratedDocumentCallsContract

### GeneratedIntentsContract

### GenerationLifecycleContract

### GenerationStateShape

### IndexChangesContract

### IndexIntentsContract

### InteractionObservationShape

### InterpretationCallsContract

### LayoutCallsContract

### LeaseLifecycleContract

### LeaseNotificationsContract

### NavigationCallsContract

### OpeningPolicyContract

### PreferenceChangesContract

### PresentationEventsContract

### PreviewAdmissionContract

### PublicationDrawingContract

### PublicationInputsContract

### PublicationIntentsContract

### PublicationStateContract

### ReferenceInterpretationContract

### ReferenceStorageContract

### ResourcePreparationContract

### ScheduledWorkContract

### StorageCallsContract

### UnsavedAdmissionContract

### ViewportChangesContract

### WindowPresentationContract

### WorkspaceChangesContract

### WorkspaceIntentsContract


## dataset (3)

### FrozenExportSnapshot

### GeneratedSelectionState

### MainDocumentState


## feature (10)

### ChapterNavigation

### ConfigurableScrolling

### DocumentNavigation

### DocumentReferences

### LivePreview

### MarkdownPresentation

### MermaidDiagrams

### PagedPublication

### SynchronizedScrolling

### WorkPathNavigation


## field (14)

### DocumentBytes

### DocumentIdentity

### DocumentPath

### DocumentRevision

### ExportBytes

### ExportDocument

### ExportPath

### ExportRevision

### GenerationSequence

### ObservationContext

### RegisteredProject

### SavedBaselineBytes

### TargetPane

### TargetWindow


## functionality (28)

### ApplicationIdentity

### ApplicationPreferences

### BuildIdentity

### DiagramLayout

### DiagramPreparation

### DocumentIndex

### DocumentSession

### EmbeddedVisuals

### EventScheduling

### FilteredFileTree

### FoxPresentation

### GeneratedDocumentNavigation

### LocalFileStorage

### MarkdownInterpretation

### MermaidInterpretation

### NavigationHistory

### PageComposition

### PdfExport

### PreviewPipeline

### PreviewTextSelection

### RenderLayout

### ScrollMotion

### SdpProjectNavigation

### SourceAnchorMapping

### TextEditing

### TypographyAndDrawing

### WorkPathHistory

### WorkspaceControls


## interface (14)

### DiagramInterpreterPort

### DiagramLayoutPort

### ExternalOpenProtocol

### FileStorePort

### InterpreterPort

### LeaseReleaseProtocol

### PreferencesStorePort

### RenderHostPort

### RendererPort

### SchedulerPort

### SdpToolProtocol

### TextMetricsPort

### ViewGenerationProtocol

### WindowDeliveryProtocol


## message (101)

### AcceptCurrentFrame

### AcceptCurrentModel

### AcceptWorkerTicket

### ActivateExternalTarget

### ActivateRootAndHistory

### AdmitDocument

### AdmitGeneratedDocument

### AdoptPreferencesDraft

### AdvanceGenerationSequence

### ApplyTextEdit

### AttemptRelease

### BeginPreferencesDraft

### CacheCompatibleScene

### CancelPdfRequest

### CancelPreviousChild

### CancelUnpublishedJob

### CandidateRead

### CandidateReadFailed

### CaptureContextTarget

### CheckCurrentMapping

### CheckReferenceGeneration

### ChooseCancel

### ChooseInternalFile

### ChooseSave

### ChooseUnsaved

### ClassifyExternalTarget

### CommitPdfCandidate

### CommitVisit

### CompleteObsoleteWorker

### CurrentWritten

### DebounceElapsed

### DeliverGeneratedMain

### DiscardObsoleteCompletion

### DiscardPdfCandidate

### DismissContextMenu

### DocumentChanged

### DocumentOpened

### DocumentRejected

### ExpandReference

### ExportPagesLaidOut

### ExportSnapshotParsed

### FavoriteFileChosen

### FavoriteFolderChosen

### FreezeExportSnapshot

### GeneratedDocumentChanged

### GeneratedDocumentOpened

### GeneratedMainAccepted

### GeneratedMainRejected

### InspectDirtyMain

### InstallNewLease

### KeepUnsentRelease

### LaunchConfiguredProgram

### LayoutExportPages

### LayoutModel

### LayoutTypedDiagram

### MapSourceAnchor

### MarkCurrentSaved

### ModelLaidOut

### NotifyPreferenceChange

### OpenTarget

### PaintPdfCandidate

### ParseExportSnapshot

### ParseMermaidBlock

### ParseReference

### ParseSnapshot

### ParsedSnapshot

### PersistPreferencesDraft

### PollReleaseQueue

### PrepareDiagramScenes

### PrepareEmbeddedResources

### PrepareExportResources

### PrepareTypedDiagram

### PresentFrame

### PublishReferenceHeadings

### QueuePreviousLease

### ReadCandidate

### ReadReference

### ReferenceParsed

### ReferenceRead

### RejectInvalidSelection

### ReleaseKeyboardGrab

### RemoveSentRelease

### ReplaceDocument

### RequestPdfExport

### RequestTreeRefresh

### RestartDebounce

### RetainDocumentAndAnchor

### SelectInvalidView

### SelectRegisteredView

### SetChosenParentRoot

### SetEditorAnchor

### ShowDirtyRejection

### StartGeneration

### TargetOpened

### ToggleFullscreen

### TypedDiagramLaidOut

### UserPreviewScroll

### ValidatePreferencesDraft

### ValidateProposedRoot

### ValidateSelection

### WriteCurrent


## mode (1)

### NormalDesktop


## scenario (22)

### ChooserOpensThenChangesRoot

### ContextMenuDismissed

### DiagramScenePrepared

### DirtyGeneratedDeliveryRejected

### DirtyOpenCancelled

### EditedPreviewPublished

### ExternalTargetHandedOff

### FavoriteFileOpens

### FavoriteFolderChangesRoot

### FullscreenChangesPresentation

### GeneratedReplacementReleasesOldLease

### GeneratedSelectionSupersedes

### InvalidSelectionRetainsChild

### PdfCancelledBeforePublication

### PdfSnapshotPublished

### PreferencesPersistBeforeAdoption

### PreviewResourcesPrepared

### PreviewScrollSynchronizesEditor

### ReferenceHeadingsLoaded

### ReleaseRetryAfterSendFailure

### SaveThenReadFails

### StalePreviewDiscarded


## unit (22)

### Application

### DocumentIndexService

### DocumentStorage

### Documents

### EventLoop

### ExternalLauncher

### ExternalViewBroker

### ExternalViewGenerator

### GeneratedViews

### Identity

### Interpreter

### NativeShell

### Navigation

### Preferences

### Preview

### Publication

### Renderer

### SdpProjects

### Typography

### Viewport

### VisualResources

### Workspace


## usecase (11)

### BrowseSdpProject

### BrowseWorkspace

### ConfigurePreferences

### EditDocuments

### NavigateDocuments

### PublishPages

### ReadDiagrams

### ReadDocuments

### ScrollDocuments

### SelectGeneratedView

### UseFullscreen
