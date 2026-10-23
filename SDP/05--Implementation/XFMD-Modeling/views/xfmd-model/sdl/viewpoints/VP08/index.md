# VP08 — Channel contracts and sequences

[Navigator](../../navigator.md)

Explicit scenario steps validated against permits, participation, mode and request/result correlation.

- [Scenario: ChooserOpensThenChangesRoot — mode NormalDesktop](VP08-ChooserOpensThenChangesRoot.md)
- [Scenario: ContextMenuDismissed — mode NormalDesktop](VP08-ContextMenuDismissed.md)
- [Scenario: DiagramScenePrepared — mode NormalDesktop](VP08-DiagramScenePrepared.md)
- [Scenario: DirtyGeneratedDeliveryRejected — mode NormalDesktop](VP08-DirtyGeneratedDeliveryRejected.md)
- [Scenario: DirtyOpenCancelled — mode NormalDesktop](VP08-DirtyOpenCancelled.md)
- [Scenario: EditedPreviewPublished — mode NormalDesktop](VP08-EditedPreviewPublished.md)
- [Scenario: ExternalTargetHandedOff — mode NormalDesktop](VP08-ExternalTargetHandedOff.md)
- [Scenario: FavoriteFileOpens — mode NormalDesktop](VP08-FavoriteFileOpens.md)
- [Scenario: FavoriteFolderChangesRoot — mode NormalDesktop](VP08-FavoriteFolderChangesRoot.md)
- [Scenario: FullscreenChangesPresentation — mode NormalDesktop](VP08-FullscreenChangesPresentation.md)
- [Scenario: GeneratedReplacementReleasesOldLease — mode NormalDesktop](VP08-GeneratedReplacementReleasesOldLease.md)
- [Scenario: GeneratedSelectionSupersedes — mode NormalDesktop](VP08-GeneratedSelectionSupersedes.md)
- [Scenario: InvalidSelectionRetainsChild — mode NormalDesktop](VP08-InvalidSelectionRetainsChild.md)
- [Scenario: PdfCancelledBeforePublication — mode NormalDesktop](VP08-PdfCancelledBeforePublication.md)
- [Scenario: PdfSnapshotPublished — mode NormalDesktop](VP08-PdfSnapshotPublished.md)
- [Scenario: PreferencesPersistBeforeAdoption — mode NormalDesktop](VP08-PreferencesPersistBeforeAdoption.md)
- [Scenario: PreviewResourcesPrepared — mode NormalDesktop](VP08-PreviewResourcesPrepared.md)
- [Scenario: PreviewScrollSynchronizesEditor — mode NormalDesktop](VP08-PreviewScrollSynchronizesEditor.md)
- [Scenario: ReferenceHeadingsLoaded — mode NormalDesktop](VP08-ReferenceHeadingsLoaded.md)
- [Scenario: ReleaseRetryAfterSendFailure — mode NormalDesktop](VP08-ReleaseRetryAfterSendFailure.md)
- [Scenario: SaveThenReadFails — mode NormalDesktop](VP08-SaveThenReadFails.md)
- [Scenario: StalePreviewDiscarded — mode NormalDesktop](VP08-StalePreviewDiscarded.md)

## Derived MessageSet

| Channel | Mode | Datagram | Sender | Receiver | Source IDs |
| --- | --- | --- | --- | --- | --- |
| ContextInteraction | NormalDesktop | CaptureContextTarget | NativeShell | NativeShell | f0074, f0108, f0110, f0546, f0549 |
| ContextInteraction | NormalDesktop | DismissContextMenu | NativeShell | NativeShell | f0108, f0111, f0164, f0547, f0550 |
| ContextInteraction | NormalDesktop | ReleaseKeyboardGrab | NativeShell | NativeShell | f0108, f0112, f0548, f0551, f0826 |
| DiagramInterpretation | NormalDesktop | ParseMermaidBlock | Interpreter | Interpreter | f0126, f0128, f0486, f0487, f0611 |
| DiagramLayoutCalls | NormalDesktop | LayoutTypedDiagram | VisualResources | Renderer | f0131, f0133, f0512, f0847, f0986 |
| DiagramLayoutCalls | NormalDesktop | TypedDiagramLaidOut | Renderer | VisualResources | f0131, f0134, f0848, f0943, f0985 |
| DocumentCalls | NormalDesktop | AdmitDocument | Navigation | Documents | f0011, f0167, f0169, f0234, f0579 |
| DocumentCalls | NormalDesktop | DocumentOpened | Documents | Navigation | f0167, f0170, f0198, f0235, f0577 |
| DocumentCalls | NormalDesktop | DocumentRejected | Documents | Navigation | f0167, f0171, f0204, f0236, f0578 |
| DocumentNotifications | NormalDesktop | CommitVisit | Documents | Navigation | f0103, f0191, f0193, f0237, f0580 |
| DocumentNotifications | NormalDesktop | DocumentChanged | Documents | Preview | f0173, f0191, f0194, f0238, f0689 |
| DocumentNotifications | NormalDesktop | GeneratedDocumentChanged | Documents | GeneratedViews | f0191, f0195, f0239, f0383, f0425 |
| DocumentStateChanges | NormalDesktop | MarkCurrentSaved | Documents | Documents | f0212, f0214, f0240, f0242, f0530 |
| DocumentStateChanges | NormalDesktop | ReplaceDocument | Documents | Documents | f0212, f0215, f0241, f0243, f0854 |
| EditIntents | NormalDesktop | ApplyTextEdit | NativeShell | Documents | f0053, f0244, f0257, f0259, f0552 |
| ExportInterpretation | NormalDesktop | ExportSnapshotParsed | Interpreter | Publication | f0288, f0290, f0305, f0489, f0748 |
| ExportInterpretation | NormalDesktop | ParseExportSnapshot | Publication | Interpreter | f0288, f0291, f0488, f0609, f0749 |
| ExportLayout | NormalDesktop | ExportPagesLaidOut | Renderer | Publication | f0292, f0294, f0298, f0750, f0850 |
| ExportLayout | NormalDesktop | LayoutExportPages | Publication | Renderer | f0292, f0295, f0508, f0751, f0849 |
| ExternalOpening | NormalDesktop | LaunchConfiguredProgram | Application | ExternalLauncher | f0035, f0311, f0312, f0314, f0502 |
| GeneratedAdmission | NormalDesktop | AdvanceGenerationSequence | GeneratedViews | GeneratedViews | f0017, f0367, f0369, f0426, f0429 |
| GeneratedAdmission | NormalDesktop | InspectDirtyMain | GeneratedViews | Documents | f0245, f0367, f0370, f0430, f0474 |
| GeneratedAdmission | NormalDesktop | RejectInvalidSelection | GeneratedViews | GeneratedViews | f0367, f0371, f0427, f0431, f0824 |
| GeneratedAdmission | NormalDesktop | ValidateSelection | GeneratedViews | GeneratedViews | f0367, f0372, f0428, f0432, f0962 |
| GeneratedDelivery | NormalDesktop | DeliverGeneratedMain | ExternalViewGenerator | GeneratedViews | f0125, f0324, f0373, f0375, f0433 |
| GeneratedDelivery | NormalDesktop | GeneratedMainAccepted | GeneratedViews | ExternalViewGenerator | f0322, f0373, f0376, f0395, f0434 |
| GeneratedDelivery | NormalDesktop | GeneratedMainRejected | GeneratedViews | ExternalViewGenerator | f0323, f0373, f0377, f0398, f0435 |
| GeneratedDocumentCalls | NormalDesktop | AdmitGeneratedDocument | GeneratedViews | Documents | f0013, f0246, f0378, f0380, f0437 |
| GeneratedDocumentCalls | NormalDesktop | GeneratedDocumentOpened | Documents | GeneratedViews | f0247, f0378, f0381, f0388, f0436 |
| GeneratedIntents | NormalDesktop | SelectInvalidView | NativeShell | GeneratedViews | f0389, f0391, f0438, f0553, f0898 |
| GeneratedIntents | NormalDesktop | SelectRegisteredView | NativeShell | GeneratedViews | f0389, f0392, f0439, f0554, f0900 |
| GenerationLifecycle | NormalDesktop | CancelPreviousChild | GeneratedViews | ExternalViewGenerator | f0064, f0325, f0440, f0453, f0455 |
| GenerationLifecycle | NormalDesktop | StartGeneration | GeneratedViews | ExternalViewGenerator | f0326, f0441, f0453, f0456, f0920 |
| IndexChanges | NormalDesktop | CheckReferenceGeneration | DocumentIndexService | DocumentIndexService | f0079, f0182, f0183, f0466, f0468 |
| IndexChanges | NormalDesktop | PublishReferenceHeadings | DocumentIndexService | NativeShell | f0184, f0466, f0469, f0555, f0779 |
| IndexIntents | NormalDesktop | ExpandReference | NativeShell | DocumentIndexService | f0185, f0283, f0470, f0472, f0556 |
| InterpretationCalls | NormalDesktop | ParseSnapshot | Preview | Interpreter | f0479, f0481, f0490, f0615, f0691 |
| InterpretationCalls | NormalDesktop | ParsedSnapshot | Interpreter | Preview | f0479, f0482, f0491, f0618, f0690 |
| LayoutCalls | NormalDesktop | LayoutModel | Preview | Renderer | f0503, f0505, f0510, f0693, f0851 |
| LayoutCalls | NormalDesktop | ModelLaidOut | Renderer | Preview | f0503, f0506, f0543, f0692, f0852 |
| LeaseLifecycle | NormalDesktop | InstallNewLease | GeneratedViews | GeneratedViews | f0442, f0446, f0476, f0513, f0515 |
| LeaseLifecycle | NormalDesktop | KeepUnsentRelease | GeneratedViews | GeneratedViews | f0443, f0447, f0500, f0513, f0516 |
| LeaseLifecycle | NormalDesktop | QueuePreviousLease | GeneratedViews | GeneratedViews | f0444, f0448, f0513, f0517, f0781 |
| LeaseLifecycle | NormalDesktop | RemoveSentRelease | GeneratedViews | GeneratedViews | f0445, f0449, f0513, f0518, f0836 |
| LeaseNotifications | NormalDesktop | AttemptRelease | GeneratedViews | ExternalViewBroker | f0055, f0321, f0450, f0519, f0521 |
| NavigationCalls | NormalDesktop | OpenTarget | Application | Navigation | f0037, f0581, f0583, f0585, f0596 |
| NavigationCalls | NormalDesktop | TargetOpened | Navigation | Application | f0036, f0582, f0583, f0586, f0931 |
| OpeningPolicy | NormalDesktop | ClassifyExternalTarget | Application | Application | f0038, f0039, f0099, f0597, f0599 |
| PreferenceChanges | NormalDesktop | AdoptPreferencesDraft | Preferences | Preferences | f0015, f0644, f0646, f0653, f0657 |
| PreferenceChanges | NormalDesktop | BeginPreferencesDraft | NativeShell | Preferences | f0057, f0558, f0644, f0647, f0654 |
| PreferenceChanges | NormalDesktop | NotifyPreferenceChange | Preferences | NativeShell | f0557, f0592, f0644, f0648, f0658 |
| PreferenceChanges | NormalDesktop | PersistPreferencesDraft | Preferences | Preferences | f0641, f0644, f0649, f0655, f0659 |
| PreferenceChanges | NormalDesktop | ValidatePreferencesDraft | Preferences | Preferences | f0644, f0650, f0656, f0660, f0958 |
| PresentationEvents | NormalDesktop | PresentFrame | Preview | NativeShell | f0559, f0678, f0679, f0681, f0694 |
| PresentationEvents | NormalDesktop | ShowDirtyRejection | GeneratedViews | NativeShell | f0451, f0560, f0679, f0682, f0906 |
| PreviewAdmission | NormalDesktop | AcceptCurrentFrame | Preview | Preview | f0001, f0695, f0700, f0710, f0712 |
| PreviewAdmission | NormalDesktop | AcceptCurrentModel | Preview | Preview | f0003, f0696, f0701, f0710, f0713 |
| PreviewAdmission | NormalDesktop | AcceptWorkerTicket | Preview | Preview | f0005, f0697, f0702, f0710, f0714 |
| PreviewAdmission | NormalDesktop | CompleteObsoleteWorker | Preview | Preview | f0105, f0698, f0703, f0710, f0715 |
| PreviewAdmission | NormalDesktop | DiscardObsoleteCompletion | Preview | Preview | f0160, f0699, f0704, f0710, f0716 |
| PublicationDrawing | NormalDesktop | PaintPdfCandidate | Publication | Typography | f0607, f0752, f0763, f0765, f0945 |
| PublicationInputs | NormalDesktop | FreezeExportSnapshot | Documents | Publication | f0248, f0360, f0753, f0766, f0768 |
| PublicationIntents | NormalDesktop | CancelPdfRequest | NativeShell | Publication | f0062, f0561, f0754, f0769, f0771 |
| PublicationIntents | NormalDesktop | RequestPdfExport | NativeShell | Publication | f0562, f0755, f0769, f0772, f0856 |
| PublicationState | NormalDesktop | CancelUnpublishedJob | Publication | Publication | f0066, f0756, f0759, f0773, f0775 |
| PublicationState | NormalDesktop | CommitPdfCandidate | Publication | Publication | f0101, f0757, f0760, f0773, f0776 |
| PublicationState | NormalDesktop | DiscardPdfCandidate | Publication | Publication | f0162, f0758, f0761, f0773, f0777 |
| ReferenceInterpretation | NormalDesktop | ParseReference | DocumentIndexService | Interpreter | f0187, f0492, f0613, f0807, f0809 |
| ReferenceInterpretation | NormalDesktop | ReferenceParsed | Interpreter | DocumentIndexService | f0186, f0493, f0807, f0810, f0813 |
| ReferenceStorage | NormalDesktop | ReadReference | DocumentIndexService | DocumentStorage | f0189, f0223, f0785, f0817, f0819 |
| ReferenceStorage | NormalDesktop | ReferenceRead | DocumentStorage | DocumentIndexService | f0188, f0224, f0816, f0817, f0820 |
| ResourcePreparation | NormalDesktop | CacheCompatibleScene | VisualResources | VisualResources | f0060, f0859, f0861, f0987, f0992 |
| ResourcePreparation | NormalDesktop | PrepareDiagramScenes | Preview | VisualResources | f0670, f0705, f0859, f0862, f0988 |
| ResourcePreparation | NormalDesktop | PrepareEmbeddedResources | Preview | VisualResources | f0672, f0706, f0859, f0863, f0989 |
| ResourcePreparation | NormalDesktop | PrepareExportResources | Publication | VisualResources | f0674, f0762, f0859, f0864, f0990 |
| ResourcePreparation | NormalDesktop | PrepareTypedDiagram | Preview | VisualResources | f0676, f0707, f0859, f0865, f0991 |
| ScheduledWork | NormalDesktop | DebounceElapsed | EventLoop | Preview | f0123, f0278, f0708, f0884, f0886 |
| ScheduledWork | NormalDesktop | PollReleaseQueue | EventLoop | GeneratedViews | f0279, f0452, f0643, f0884, f0887 |
| ScheduledWork | NormalDesktop | RestartDebounce | Preview | EventLoop | f0277, f0709, f0867, f0884, f0888 |
| StorageCalls | NormalDesktop | CandidateRead | DocumentStorage | Documents | f0069, f0227, f0249, f0921, f0923 |
| StorageCalls | NormalDesktop | CandidateReadFailed | DocumentStorage | Documents | f0072, f0228, f0250, f0921, f0924 |
| StorageCalls | NormalDesktop | CurrentWritten | DocumentStorage | Documents | f0121, f0229, f0251, f0921, f0925 |
| StorageCalls | NormalDesktop | ReadCandidate | Documents | DocumentStorage | f0225, f0252, f0783, f0921, f0926 |
| StorageCalls | NormalDesktop | WriteCurrent | Documents | DocumentStorage | f0226, f0253, f0921, f0927, f1030 |
| UnsavedAdmission | NormalDesktop | ChooseCancel | NativeShell | Documents | f0082, f0254, f0564, f0950, f0952 |
| UnsavedAdmission | NormalDesktop | ChooseSave | NativeShell | Documents | f0087, f0255, f0565, f0950, f0953 |
| UnsavedAdmission | NormalDesktop | ChooseUnsaved | Documents | NativeShell | f0089, f0256, f0563, f0950, f0954 |
| ViewportChanges | NormalDesktop | CheckCurrentMapping | Viewport | Viewport | f0077, f0967, f0971, f0974, f0976 |
| ViewportChanges | NormalDesktop | MapSourceAnchor | Viewport | Viewport | f0528, f0968, f0972, f0974, f0977 |
| ViewportChanges | NormalDesktop | RetainDocumentAndAnchor | NativeShell | Viewport | f0567, f0869, f0969, f0974, f0978 |
| ViewportChanges | NormalDesktop | SetEditorAnchor | Viewport | NativeShell | f0566, f0904, f0973, f0974, f0979 |
| ViewportChanges | NormalDesktop | UserPreviewScroll | NativeShell | Viewport | f0568, f0956, f0970, f0974, f0980 |
| WindowPresentation | NormalDesktop | ToggleFullscreen | NativeShell | NativeShell | f0569, f0570, f0940, f0993, f0995 |
| WorkspaceChanges | NormalDesktop | ActivateRootAndHistory | Workspace | Workspace | f0009, f1001, f1004, f1007, f1009 |
| WorkspaceChanges | NormalDesktop | RequestTreeRefresh | Workspace | NativeShell | f0571, f0858, f1005, f1007, f1010 |
| WorkspaceChanges | NormalDesktop | SetChosenParentRoot | Application | Workspace | f0040, f0902, f1002, f1007, f1011 |
| WorkspaceChanges | NormalDesktop | ValidateProposedRoot | Application | Workspace | f0041, f0960, f1003, f1007, f1012 |
| WorkspaceIntents | NormalDesktop | ActivateExternalTarget | NativeShell | Application | f0007, f0042, f0572, f1023, f1025 |
| WorkspaceIntents | NormalDesktop | ChooseInternalFile | NativeShell | Application | f0043, f0084, f0573, f1023, f1026 |
| WorkspaceIntents | NormalDesktop | FavoriteFileChosen | NativeShell | Application | f0044, f0328, f0574, f1023, f1027 |
| WorkspaceIntents | NormalDesktop | FavoriteFolderChosen | NativeShell | Workspace | f0348, f0575, f1006, f1023, f1028 |

