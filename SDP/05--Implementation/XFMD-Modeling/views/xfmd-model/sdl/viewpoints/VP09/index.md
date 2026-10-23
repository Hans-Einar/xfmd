# VP09 — Dataset, Datagram and persistent Database

[Navigator](../../navigator.md)

Explicit holders, sources, contracts, variants, fields and projections.

- [Data origin and holder: FrozenExportSnapshot](VP09-data-FrozenExportSnapshot.md)
- [Data origin and holder: GeneratedSelectionState](VP09-data-GeneratedSelectionState.md)
- [Data origin and holder: MainDocumentState](VP09-data-MainDocumentState.md)
- [Contract structure: ContextInteractionContract](VP09-contract-ContextInteractionContract.md)
- [Contract structure: DiagramInterpretationContract](VP09-contract-DiagramInterpretationContract.md)
- [Contract structure: DiagramLayoutCallsContract](VP09-contract-DiagramLayoutCallsContract.md)
- [Contract structure: DocumentCallsContract](VP09-contract-DocumentCallsContract.md)
- [Contract structure: DocumentNotificationsContract](VP09-contract-DocumentNotificationsContract.md)
- [Contract structure: DocumentStateChangesContract](VP09-contract-DocumentStateChangesContract.md)
- [Contract structure: DocumentStateShape](VP09-contract-DocumentStateShape.md)
- [Contract structure: EditIntentsContract](VP09-contract-EditIntentsContract.md)
- [Contract structure: ExportInterpretationContract](VP09-contract-ExportInterpretationContract.md)
- [Contract structure: ExportLayoutContract](VP09-contract-ExportLayoutContract.md)
- [Contract structure: ExportSnapshotShape](VP09-contract-ExportSnapshotShape.md)
- [Contract structure: ExternalOpeningContract](VP09-contract-ExternalOpeningContract.md)
- [Contract structure: GeneratedAdmissionContract](VP09-contract-GeneratedAdmissionContract.md)
- [Contract structure: GeneratedDeliveryContract](VP09-contract-GeneratedDeliveryContract.md)
- [Contract structure: GeneratedDocumentCallsContract](VP09-contract-GeneratedDocumentCallsContract.md)
- [Contract structure: GeneratedIntentsContract](VP09-contract-GeneratedIntentsContract.md)
- [Contract structure: GenerationLifecycleContract](VP09-contract-GenerationLifecycleContract.md)
- [Contract structure: GenerationStateShape](VP09-contract-GenerationStateShape.md)
- [Contract structure: IndexChangesContract](VP09-contract-IndexChangesContract.md)
- [Contract structure: IndexIntentsContract](VP09-contract-IndexIntentsContract.md)
- [Contract structure: InteractionObservationShape](VP09-contract-InteractionObservationShape.md)
- [Contract structure: InterpretationCallsContract](VP09-contract-InterpretationCallsContract.md)
- [Contract structure: LayoutCallsContract](VP09-contract-LayoutCallsContract.md)
- [Contract structure: LeaseLifecycleContract](VP09-contract-LeaseLifecycleContract.md)
- [Contract structure: LeaseNotificationsContract](VP09-contract-LeaseNotificationsContract.md)
- [Contract structure: NavigationCallsContract](VP09-contract-NavigationCallsContract.md)
- [Contract structure: OpeningPolicyContract](VP09-contract-OpeningPolicyContract.md)
- [Contract structure: PreferenceChangesContract](VP09-contract-PreferenceChangesContract.md)
- [Contract structure: PresentationEventsContract](VP09-contract-PresentationEventsContract.md)
- [Contract structure: PreviewAdmissionContract](VP09-contract-PreviewAdmissionContract.md)
- [Contract structure: PublicationDrawingContract](VP09-contract-PublicationDrawingContract.md)
- [Contract structure: PublicationInputsContract](VP09-contract-PublicationInputsContract.md)
- [Contract structure: PublicationIntentsContract](VP09-contract-PublicationIntentsContract.md)
- [Contract structure: PublicationStateContract](VP09-contract-PublicationStateContract.md)
- [Contract structure: ReferenceInterpretationContract](VP09-contract-ReferenceInterpretationContract.md)
- [Contract structure: ReferenceStorageContract](VP09-contract-ReferenceStorageContract.md)
- [Contract structure: ResourcePreparationContract](VP09-contract-ResourcePreparationContract.md)
- [Contract structure: ScheduledWorkContract](VP09-contract-ScheduledWorkContract.md)
- [Contract structure: StorageCallsContract](VP09-contract-StorageCallsContract.md)
- [Contract structure: UnsavedAdmissionContract](VP09-contract-UnsavedAdmissionContract.md)
- [Contract structure: ViewportChangesContract](VP09-contract-ViewportChangesContract.md)
- [Contract structure: WindowPresentationContract](VP09-contract-WindowPresentationContract.md)
- [Contract structure: WorkspaceChangesContract](VP09-contract-WorkspaceChangesContract.md)
- [Contract structure: WorkspaceIntentsContract](VP09-contract-WorkspaceIntentsContract.md)

## Fields and contract properties

Database means a persistent data source, not necessarily SQL.

| Fact | ID |
| --- | --- |
| ContextInteractionContract has completeness = closed. | f0109 |
| DiagramInterpretationContract has completeness = closed. | f0127 |
| DiagramLayoutCallsContract has completeness = closed. | f0132 |
| DocumentBytes has presence = required. | f0165 |
| DocumentBytes has value-type = bytes. | f0166 |
| DocumentCallsContract has completeness = closed. | f0168 |
| DocumentIdentity has presence = required. | f0174 |
| DocumentIdentity has value-type = unsigned. | f0175 |
| DocumentNotificationsContract has completeness = closed. | f0192 |
| DocumentPath has presence = required. | f0199 |
| DocumentPath has value-type = text. | f0200 |
| DocumentRevision has presence = required. | f0205 |
| DocumentRevision has value-type = unsigned. | f0206 |
| DocumentStateChangesContract has completeness = closed. | f0213 |
| DocumentStateShape has completeness = closed. | f0216 |
| EditIntentsContract has completeness = closed. | f0258 |
| ExportBytes has presence = required. | f0284 |
| ExportBytes has value-type = bytes. | f0285 |
| ExportDocument has presence = required. | f0286 |
| ExportDocument has value-type = unsigned. | f0287 |
| ExportInterpretationContract has completeness = closed. | f0289 |
| ExportLayoutContract has completeness = closed. | f0293 |
| ExportPath has presence = required. | f0299 |
| ExportPath has value-type = text. | f0300 |
| ExportRevision has presence = required. | f0301 |
| ExportRevision has value-type = unsigned. | f0302 |
| ExportSnapshotShape has completeness = closed. | f0306 |
| ExternalOpeningContract has completeness = closed. | f0313 |
| GeneratedAdmissionContract has completeness = closed. | f0368 |
| GeneratedDeliveryContract has completeness = closed. | f0374 |
| GeneratedDocumentCallsContract has completeness = closed. | f0379 |
| GeneratedIntentsContract has completeness = closed. | f0390 |
| GenerationLifecycleContract has completeness = closed. | f0454 |
| GenerationSequence has presence = required. | f0457 |
| GenerationSequence has value-type = unsigned. | f0458 |
| GenerationStateShape has completeness = closed. | f0459 |
| IndexChangesContract has completeness = closed. | f0467 |
| IndexIntentsContract has completeness = closed. | f0471 |
| InteractionObservationShape has completeness = closed. | f0477 |
| InterpretationCallsContract has completeness = closed. | f0480 |
| LayoutCallsContract has completeness = closed. | f0504 |
| LeaseLifecycleContract has completeness = closed. | f0514 |
| LeaseNotificationsContract has completeness = closed. | f0520 |
| NavigationCallsContract has completeness = closed. | f0584 |
| ObservationContext has presence = required. | f0593 |
| ObservationContext has value-type = text. | f0594 |
| OpeningPolicyContract has completeness = closed. | f0598 |
| PreferenceChangesContract has completeness = closed. | f0645 |
| PresentationEventsContract has completeness = closed. | f0680 |
| PreviewAdmissionContract has completeness = closed. | f0711 |
| PublicationDrawingContract has completeness = closed. | f0764 |
| PublicationInputsContract has completeness = closed. | f0767 |
| PublicationIntentsContract has completeness = closed. | f0770 |
| PublicationStateContract has completeness = closed. | f0774 |
| ReferenceInterpretationContract has completeness = closed. | f0808 |
| ReferenceStorageContract has completeness = closed. | f0818 |
| RegisteredProject has presence = required. | f0821 |
| RegisteredProject has value-type = text. | f0822 |
| ResourcePreparationContract has completeness = closed. | f0860 |
| SavedBaselineBytes has presence = required. | f0882 |
| SavedBaselineBytes has value-type = bytes. | f0883 |
| ScheduledWorkContract has completeness = closed. | f0885 |
| StorageCallsContract has completeness = closed. | f0922 |
| TargetPane has presence = required. | f0932 |
| TargetPane has value-type = text. | f0933 |
| TargetWindow has presence = required. | f0934 |
| TargetWindow has value-type = text. | f0935 |
| UnsavedAdmissionContract has completeness = closed. | f0951 |
| ViewportChangesContract has completeness = closed. | f0975 |
| WindowPresentationContract has completeness = closed. | f0994 |
| WorkspaceChangesContract has completeness = closed. | f1008 |
| WorkspaceIntentsContract has completeness = closed. | f1024 |

## Projection responsibilities

| Functionality | Dataset | Datagram | Fact |
| --- | --- | --- | --- |

