# VP09 — Dataset, Datagram and persistent Database

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

Explicit holders, sources, contracts, variants, fields and projections.

## Data origin and holder: FrozenExportSnapshot

```mermaid
flowchart LR
    n_ExportSnapshotShape["ExportSnapshotShape (contract)"]
    n_FrozenExportSnapshot["FrozenExportSnapshot (dataset)"]
    n_Publication["Publication (unit)"]
    n_FrozenExportSnapshot -->|upholds| n_ExportSnapshotShape
    n_Publication -->|holds| n_FrozenExportSnapshot
```

Source facts: [f0361](sdl-source://s0/31783/31832), [f0746](sdl-source://s0/58490/58529).

Declarations: [ExportSnapshotShape](sdl-source://s0/2814/2843), [FrozenExportSnapshot](sdl-source://s0/3285/3314), [Publication](sdl-source://s0/6298/6315).

## Data origin and holder: GeneratedSelectionState

```mermaid
flowchart LR
    n_GeneratedSelectionState["GeneratedSelectionState (dataset)"]
    n_GeneratedViews["GeneratedViews (unit)"]
    n_GenerationStateShape["GenerationStateShape (contract)"]
    n_GeneratedSelectionState -->|upholds| n_GenerationStateShape
    n_GeneratedViews -->|holds| n_GeneratedSelectionState
```

Source facts: [f0410](sdl-source://s0/35331/35384), [f0423](sdl-source://s0/36366/36411).

Declarations: [GeneratedSelectionState](sdl-source://s0/3836/3868), [GeneratedViews](sdl-source://s0/3908/3928), [GenerationStateShape](sdl-source://s0/4022/4052).

## Data origin and holder: MainDocumentState

```mermaid
flowchart LR
    n_DocumentStateShape["DocumentStateShape (contract)"]
    n_Documents["Documents (unit)"]
    n_MainDocumentState["MainDocumentState (dataset)"]
    n_Documents -->|holds| n_MainDocumentState
    n_MainDocumentState -->|upholds| n_DocumentStateShape
```

Source facts: [f0231](sdl-source://s0/22726/22760), [f0526](sdl-source://s0/43201/43246).

Declarations: [DocumentStateShape](sdl-source://s0/2276/2304), [Documents](sdl-source://s0/2327/2342), [MainDocumentState](sdl-source://s0/4808/4834).

## Scope and notation

Dataset neighborhoods retain explicit from, holds, owns and upholds relations. Database means a persistent data source, not SQL tables, keys, transactions, a storage engine or deployment. Unit-held data has no declared persistence. These are definitions, not runtime instances. Contract completeness is a source assertion, not execution evidence. Optional fields do not imply null, empty, false or zero. Projections do not specify executable transformations. Encoding and bit positions belong to VP10.

## Data definitions

- [FrozenExportSnapshot](sdl-source://s0/3285/3314) (Dataset): holder [Publication](sdl-source://s0/6298/6315); contract [ExportSnapshotShape](sdl-source://s0/2814/2843); Unit-held; persistence unspecified. Source facts: [f0361](sdl-source://s0/31783/31832), [f0746](sdl-source://s0/58490/58529).
- [GeneratedSelectionState](sdl-source://s0/3836/3868) (Dataset): holder [GeneratedViews](sdl-source://s0/3908/3928); contract [GenerationStateShape](sdl-source://s0/4022/4052); Unit-held; persistence unspecified. Source facts: [f0410](sdl-source://s0/35331/35384), [f0423](sdl-source://s0/36366/36411).
- [MainDocumentState](sdl-source://s0/4808/4834) (Dataset): holder [Documents](sdl-source://s0/2327/2342); contract [DocumentStateShape](sdl-source://s0/2276/2304); Unit-held; persistence unspecified. Source facts: [f0231](sdl-source://s0/22726/22760), [f0526](sdl-source://s0/43201/43246).

No datagram declarations.


## Database register

No Databases are declared; no persistent store is inferred.

## Contract register

All Contracts are included, even unused/open definitions. Open a Contract in the navigation tree for its structure diagram. Counts describe explicit shape membership (fields / variants / permitted messages), not runtime coverage.

| Contract | Completeness | Fields / variants / messages | Source fact |
| --- | --- | --- | --- |
| [ContextInteractionContract](sdl-source://s0/1172/1208) | closed | 0 / 0 / 3 | [f0109](sdl-source://s0/14980/15033) |
| [DiagramInterpretationContract](sdl-source://s0/1350/1389) | closed | 0 / 0 / 1 | [f0127](sdl-source://s0/16069/16125) |
| [DiagramLayoutCallsContract](sdl-source://s0/1481/1517) | closed | 0 / 0 / 2 | [f0132](sdl-source://s0/16346/16399) |
| [DocumentCallsContract](sdl-source://s0/1818/1849) | closed | 0 / 0 / 3 | [f0168](sdl-source://s0/19017/19065) |
| [DocumentNotificationsContract](sdl-source://s0/2014/2053) | closed | 0 / 0 / 3 | [f0192](sdl-source://s0/20574/20630) |
| [DocumentStateChangesContract](sdl-source://s0/2237/2275) | closed | 0 / 0 / 2 | [f0213](sdl-source://s0/21604/21659) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | closed | 5 / 0 / 0 | [f0216](sdl-source://s0/21769/21814) |
| [EditIntentsContract](sdl-source://s0/2387/2416) | closed | 0 / 0 / 1 | [f0258](sdl-source://s0/24838/24884) |
| [ExportInterpretationContract](sdl-source://s0/2624/2662) | closed | 0 / 0 / 2 | [f0289](sdl-source://s0/26995/27050) |
| [ExportLayoutContract](sdl-source://s0/2685/2715) | closed | 0 / 0 / 2 | [f0293](sdl-source://s0/27211/27258) |
| [ExportSnapshotShape](sdl-source://s0/2814/2843) | closed | 4 / 0 / 0 | [f0306](sdl-source://s0/27818/27864) |
| [ExternalOpeningContract](sdl-source://s0/2924/2957) | closed | 0 / 0 / 1 | [f0313](sdl-source://s0/28191/28241) |
| [GeneratedAdmissionContract](sdl-source://s0/3383/3419) | closed | 0 / 0 / 4 | [f0368](sdl-source://s0/32286/32339) |
| [GeneratedDeliveryContract](sdl-source://s0/3447/3482) | closed | 0 / 0 / 3 | [f0374](sdl-source://s0/32621/32673) |
| [GeneratedDocumentCallsContract](sdl-source://s0/3515/3555) | closed | 0 / 0 / 2 | [f0379](sdl-source://s0/32907/32964) |
| [GeneratedIntentsContract](sdl-source://s0/3692/3726) | closed | 0 / 0 / 2 | [f0390](sdl-source://s0/33567/33618) |
| [GenerationLifecycleContract](sdl-source://s0/3958/3995) | closed | 0 / 0 / 2 | [f0454](sdl-source://s0/39169/39223) |
| [GenerationStateShape](sdl-source://s0/4022/4052) | closed | 4 / 0 / 0 | [f0459](sdl-source://s0/39424/39471) |
| [IndexChangesContract](sdl-source://s0/4090/4120) | closed | 0 / 0 / 2 | [f0467](sdl-source://s0/39768/39815) |
| [IndexIntentsContract](sdl-source://s0/4143/4173) | closed | 0 / 0 / 1 | [f0471](sdl-source://s0/39969/40016) |
| [InteractionObservationShape](sdl-source://s0/4225/4262) | closed | 1 / 0 / 0 | [f0477](sdl-source://s0/40255/40309) |
| [InterpretationCallsContract](sdl-source://s0/4292/4329) | closed | 0 / 0 / 2 | [f0480](sdl-source://s0/40425/40479) |
| [LayoutCallsContract](sdl-source://s0/4495/4524) | closed | 0 / 0 / 2 | [f0504](sdl-source://s0/42115/42161) |
| [LeaseLifecycleContract](sdl-source://s0/4625/4657) | closed | 0 / 0 / 4 | [f0514](sdl-source://s0/42585/42634) |
| [LeaseNotificationsContract](sdl-source://s0/4686/4722) | closed | 0 / 0 / 1 | [f0520](sdl-source://s0/42889/42942) |
| [NavigationCallsContract](sdl-source://s0/5125/5158) | closed | 0 / 0 / 2 | [f0584](sdl-source://s0/47447/47497) |
| [OpeningPolicyContract](sdl-source://s0/5313/5344) | closed | 0 / 0 / 1 | [f0598](sdl-source://s0/48139/48187) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | closed | 0 / 0 / 5 | [f0645](sdl-source://s0/51212/51264) |
| [PresentationEventsContract](sdl-source://s0/6042/6078) | closed | 0 / 0 / 2 | [f0680](sdl-source://s0/53779/53832) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | closed | 0 / 0 / 5 | [f0711](sdl-source://s0/55986/56037) |
| [PublicationDrawingContract](sdl-source://s0/6344/6380) | closed | 0 / 0 / 1 | [f0764](sdl-source://s0/59983/60036) |
| [PublicationInputsContract](sdl-source://s0/6408/6443) | closed | 0 / 0 / 1 | [f0767](sdl-source://s0/60144/60196) |
| [PublicationIntentsContract](sdl-source://s0/6472/6508) | closed | 0 / 0 / 2 | [f0770](sdl-source://s0/60308/60361) |
| [PublicationStateContract](sdl-source://s0/6535/6569) | closed | 0 / 0 / 3 | [f0774](sdl-source://s0/60519/60570) |
| [ReferenceInterpretationContract](sdl-source://s0/6832/6873) | closed | 0 / 0 / 2 | [f0808](sdl-source://s0/62633/62691) |
| [ReferenceStorageContract](sdl-source://s0/6948/6982) | closed | 0 / 0 / 2 | [f0818](sdl-source://s0/63127/63178) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | closed | 0 / 0 / 5 | [f0860](sdl-source://s0/65693/65747) |
| [ScheduledWorkContract](sdl-source://s0/7509/7540) | closed | 0 / 0 / 3 | [f0885](sdl-source://s0/67434/67482) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | closed | 0 / 0 / 5 | [f0922](sdl-source://s0/69418/69465) |
| [UnsavedAdmissionContract](sdl-source://s0/8292/8326) | closed | 0 / 0 / 3 | [f0951](sdl-source://s0/70826/70877) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | closed | 0 / 0 / 5 | [f0975](sdl-source://s0/72213/72263) |
| [WindowPresentationContract](sdl-source://s0/8660/8696) | closed | 0 / 0 / 1 | [f0994](sdl-source://s0/73539/73592) |
| [WorkspaceChangesContract](sdl-source://s0/8798/8832) | closed | 0 / 0 / 4 | [f1008](sdl-source://s0/74465/74516) |
| [WorkspaceIntentsContract](sdl-source://s0/8892/8926) | closed | 0 / 0 / 4 | [f1024](sdl-source://s0/75330/75381) |

## Variant register

No Variants are declared.

## Field register

| Field | Shape owner | Value type | Presence | Source facts |
| --- | --- | --- | --- | --- |
| [DocumentBytes](sdl-source://s0/1774/1794) | [DocumentStateShape](sdl-source://s0/2276/2304) | bytes | required | [f0165](sdl-source://s0/18895/18933), [f0166](sdl-source://s0/18934/18971), [f0217](sdl-source://s0/21815/21858) |
| [DocumentIdentity](sdl-source://s0/1875/1898) | [DocumentStateShape](sdl-source://s0/2276/2304) | unsigned | required | [f0174](sdl-source://s0/19300/19341), [f0175](sdl-source://s0/19342/19385), [f0218](sdl-source://s0/21859/21905) |
| [DocumentPath](sdl-source://s0/2078/2097) | [DocumentStateShape](sdl-source://s0/2276/2304) | text | required | [f0199](sdl-source://s0/20936/20973), [f0200](sdl-source://s0/20974/21009), [f0219](sdl-source://s0/21906/21948) |
| [DocumentRevision](sdl-source://s0/2152/2175) | [DocumentStateShape](sdl-source://s0/2276/2304) | unsigned | required | [f0205](sdl-source://s0/21198/21239), [f0206](sdl-source://s0/21240/21283), [f0220](sdl-source://s0/21949/21995) |
| [ExportBytes](sdl-source://s0/2553/2571) | [ExportSnapshotShape](sdl-source://s0/2814/2843) | bytes | required | [f0284](sdl-source://s0/26781/26817), [f0285](sdl-source://s0/26818/26853), [f0307](sdl-source://s0/27865/27907) |
| [ExportDocument](sdl-source://s0/2572/2593) | [ExportSnapshotShape](sdl-source://s0/2814/2843) | unsigned | required | [f0286](sdl-source://s0/26854/26893), [f0287](sdl-source://s0/26894/26935), [f0308](sdl-source://s0/27908/27953) |
| [ExportPath](sdl-source://s0/2744/2761) | [ExportSnapshotShape](sdl-source://s0/2814/2843) | text | required | [f0299](sdl-source://s0/27507/27542), [f0300](sdl-source://s0/27543/27576), [f0309](sdl-source://s0/27954/27995) |
| [ExportRevision](sdl-source://s0/2762/2783) | [ExportSnapshotShape](sdl-source://s0/2814/2843) | unsigned | required | [f0301](sdl-source://s0/27577/27616), [f0302](sdl-source://s0/27617/27658), [f0310](sdl-source://s0/27996/28041) |
| [GenerationSequence](sdl-source://s0/3996/4021) | [GenerationStateShape](sdl-source://s0/4022/4052) | unsigned | required | [f0457](sdl-source://s0/39334/39377), [f0458](sdl-source://s0/39378/39423), [f0460](sdl-source://s0/39472/39522) |
| [ObservationContext](sdl-source://s0/5244/5269) | [InteractionObservationShape](sdl-source://s0/4225/4262) | text | required | [f0478](sdl-source://s0/40310/40367), [f0593](sdl-source://s0/47921/47964), [f0594](sdl-source://s0/47965/48006) |
| [RegisteredProject](sdl-source://s0/6983/7007) | [GenerationStateShape](sdl-source://s0/4022/4052) | text | required | [f0461](sdl-source://s0/39523/39572), [f0821](sdl-source://s0/63275/63317), [f0822](sdl-source://s0/63318/63358) |
| [SavedBaselineBytes](sdl-source://s0/7460/7485) | [DocumentStateShape](sdl-source://s0/2276/2304) | bytes | required | [f0221](sdl-source://s0/21996/22044), [f0882](sdl-source://s0/67302/67345), [f0883](sdl-source://s0/67346/67388) |
| [TargetPane](sdl-source://s0/8066/8083) | [GenerationStateShape](sdl-source://s0/4022/4052) | text | required | [f0462](sdl-source://s0/39573/39615), [f0932](sdl-source://s0/69866/69901), [f0933](sdl-source://s0/69902/69935) |
| [TargetWindow](sdl-source://s0/8084/8103) | [GenerationStateShape](sdl-source://s0/4022/4052) | text | required | [f0463](sdl-source://s0/39616/39660), [f0934](sdl-source://s0/69936/69973), [f0935](sdl-source://s0/69974/70009) |

## Shape membership

| Shape | Relation | Member | Source fact |
| --- | --- | --- | --- |
| [ContextInteractionContract](sdl-source://s0/1172/1208) | permits | [CaptureContextTarget](sdl-source://s0/719/748) | [f0110](sdl-source://s0/15034/15090) |
| [ContextInteractionContract](sdl-source://s0/1172/1208) | permits | [DismissContextMenu](sdl-source://s0/1746/1773) | [f0111](sdl-source://s0/15091/15145) |
| [ContextInteractionContract](sdl-source://s0/1172/1208) | permits | [ReleaseKeyboardGrab](sdl-source://s0/7040/7068) | [f0112](sdl-source://s0/15146/15201) |
| [DiagramInterpretationContract](sdl-source://s0/1350/1389) | permits | [ParseMermaidBlock](sdl-source://s0/5458/5484) | [f0128](sdl-source://s0/16126/16182) |
| [DiagramLayoutCallsContract](sdl-source://s0/1481/1517) | permits | [LayoutTypedDiagram](sdl-source://s0/4573/4600) | [f0133](sdl-source://s0/16400/16454) |
| [DiagramLayoutCallsContract](sdl-source://s0/1481/1517) | permits | [TypedDiagramLaidOut](sdl-source://s0/8184/8212) | [f0134](sdl-source://s0/16455/16510) |
| [DocumentCallsContract](sdl-source://s0/1818/1849) | permits | [AdmitDocument](sdl-source://s0/182/204) | [f0169](sdl-source://s0/19066/19110) |
| [DocumentCallsContract](sdl-source://s0/1818/1849) | permits | [DocumentOpened](sdl-source://s0/2054/2077) | [f0170](sdl-source://s0/19111/19156) |
| [DocumentCallsContract](sdl-source://s0/1818/1849) | permits | [DocumentRejected](sdl-source://s0/2126/2151) | [f0171](sdl-source://s0/19157/19204) |
| [DocumentNotificationsContract](sdl-source://s0/2014/2053) | permits | [CommitVisit](sdl-source://s0/1030/1050) | [f0193](sdl-source://s0/20631/20681) |
| [DocumentNotificationsContract](sdl-source://s0/2014/2053) | permits | [DocumentChanged](sdl-source://s0/1850/1874) | [f0194](sdl-source://s0/20682/20736) |
| [DocumentNotificationsContract](sdl-source://s0/2014/2053) | permits | [GeneratedDocumentChanged](sdl-source://s0/3556/3589) | [f0195](sdl-source://s0/20737/20800) |
| [DocumentStateChangesContract](sdl-source://s0/2237/2275) | permits | [MarkCurrentSaved](sdl-source://s0/4860/4885) | [f0214](sdl-source://s0/21660/21714) |
| [DocumentStateChangesContract](sdl-source://s0/2237/2275) | permits | [ReplaceDocument](sdl-source://s0/7228/7252) | [f0215](sdl-source://s0/21715/21768) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | has-field | [DocumentBytes](sdl-source://s0/1774/1794) | [f0217](sdl-source://s0/21815/21858) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | has-field | [DocumentIdentity](sdl-source://s0/1875/1898) | [f0218](sdl-source://s0/21859/21905) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | has-field | [DocumentPath](sdl-source://s0/2078/2097) | [f0219](sdl-source://s0/21906/21948) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | has-field | [DocumentRevision](sdl-source://s0/2152/2175) | [f0220](sdl-source://s0/21949/21995) |
| [DocumentStateShape](sdl-source://s0/2276/2304) | has-field | [SavedBaselineBytes](sdl-source://s0/7460/7485) | [f0221](sdl-source://s0/21996/22044) |
| [EditIntentsContract](sdl-source://s0/2387/2416) | permits | [ApplyTextEdit](sdl-source://s0/394/416) | [f0259](sdl-source://s0/24885/24927) |
| [ExportInterpretationContract](sdl-source://s0/2624/2662) | permits | [ExportSnapshotParsed](sdl-source://s0/2784/2813) | [f0290](sdl-source://s0/27051/27109) |
| [ExportInterpretationContract](sdl-source://s0/2624/2662) | permits | [ParseExportSnapshot](sdl-source://s0/5429/5457) | [f0291](sdl-source://s0/27110/27167) |
| [ExportLayoutContract](sdl-source://s0/2685/2715) | permits | [ExportPagesLaidOut](sdl-source://s0/2716/2743) | [f0294](sdl-source://s0/27259/27307) |
| [ExportLayoutContract](sdl-source://s0/2685/2715) | permits | [LayoutExportPages](sdl-source://s0/4525/4551) | [f0295](sdl-source://s0/27308/27355) |
| [ExportSnapshotShape](sdl-source://s0/2814/2843) | has-field | [ExportBytes](sdl-source://s0/2553/2571) | [f0307](sdl-source://s0/27865/27907) |
| [ExportSnapshotShape](sdl-source://s0/2814/2843) | has-field | [ExportDocument](sdl-source://s0/2572/2593) | [f0308](sdl-source://s0/27908/27953) |
| [ExportSnapshotShape](sdl-source://s0/2814/2843) | has-field | [ExportPath](sdl-source://s0/2744/2761) | [f0309](sdl-source://s0/27954/27995) |
| [ExportSnapshotShape](sdl-source://s0/2814/2843) | has-field | [ExportRevision](sdl-source://s0/2762/2783) | [f0310](sdl-source://s0/27996/28041) |
| [ExternalOpeningContract](sdl-source://s0/2924/2957) | permits | [LaunchConfiguredProgram](sdl-source://s0/4441/4473) | [f0314](sdl-source://s0/28242/28298) |
| [GeneratedAdmissionContract](sdl-source://s0/3383/3419) | permits | [AdvanceGenerationSequence](sdl-source://s0/268/302) | [f0369](sdl-source://s0/32340/32401) |
| [GeneratedAdmissionContract](sdl-source://s0/3383/3419) | permits | [InspectDirtyMain](sdl-source://s0/4174/4199) | [f0370](sdl-source://s0/32402/32454) |
| [GeneratedAdmissionContract](sdl-source://s0/3383/3419) | permits | [RejectInvalidSelection](sdl-source://s0/7008/7039) | [f0371](sdl-source://s0/32455/32513) |
| [GeneratedAdmissionContract](sdl-source://s0/3383/3419) | permits | [ValidateSelection](sdl-source://s0/8441/8467) | [f0372](sdl-source://s0/32514/32567) |
| [GeneratedDeliveryContract](sdl-source://s0/3447/3482) | permits | [DeliverGeneratedMain](sdl-source://s0/1289/1318) | [f0375](sdl-source://s0/32674/32729) |
| [GeneratedDeliveryContract](sdl-source://s0/3447/3482) | permits | [GeneratedMainAccepted](sdl-source://s0/3727/3757) | [f0376](sdl-source://s0/32730/32786) |
| [GeneratedDeliveryContract](sdl-source://s0/3447/3482) | permits | [GeneratedMainRejected](sdl-source://s0/3758/3788) | [f0377](sdl-source://s0/32787/32843) |
| [GeneratedDocumentCallsContract](sdl-source://s0/3515/3555) | permits | [AdmitGeneratedDocument](sdl-source://s0/205/236) | [f0380](sdl-source://s0/32965/33027) |
| [GeneratedDocumentCallsContract](sdl-source://s0/3515/3555) | permits | [GeneratedDocumentOpened](sdl-source://s0/3633/3665) | [f0381](sdl-source://s0/33028/33091) |
| [GeneratedIntentsContract](sdl-source://s0/3692/3726) | permits | [SelectInvalidView](sdl-source://s0/7729/7755) | [f0391](sdl-source://s0/33619/33670) |
| [GeneratedIntentsContract](sdl-source://s0/3692/3726) | permits | [SelectRegisteredView](sdl-source://s0/7756/7785) | [f0392](sdl-source://s0/33671/33725) |
| [GenerationLifecycleContract](sdl-source://s0/3958/3995) | permits | [CancelPreviousChild](sdl-source://s0/608/636) | [f0455](sdl-source://s0/39224/39280) |
| [GenerationLifecycleContract](sdl-source://s0/3958/3995) | permits | [StartGeneration](sdl-source://s0/7935/7959) | [f0456](sdl-source://s0/39281/39333) |
| [GenerationStateShape](sdl-source://s0/4022/4052) | has-field | [GenerationSequence](sdl-source://s0/3996/4021) | [f0460](sdl-source://s0/39472/39522) |
| [GenerationStateShape](sdl-source://s0/4022/4052) | has-field | [RegisteredProject](sdl-source://s0/6983/7007) | [f0461](sdl-source://s0/39523/39572) |
| [GenerationStateShape](sdl-source://s0/4022/4052) | has-field | [TargetPane](sdl-source://s0/8066/8083) | [f0462](sdl-source://s0/39573/39615) |
| [GenerationStateShape](sdl-source://s0/4022/4052) | has-field | [TargetWindow](sdl-source://s0/8084/8103) | [f0463](sdl-source://s0/39616/39660) |
| [IndexChangesContract](sdl-source://s0/4090/4120) | permits | [CheckReferenceGeneration](sdl-source://s0/805/838) | [f0468](sdl-source://s0/39816/39870) |
| [IndexChangesContract](sdl-source://s0/4090/4120) | permits | [PublishReferenceHeadings](sdl-source://s0/6592/6625) | [f0469](sdl-source://s0/39871/39925) |
| [IndexIntentsContract](sdl-source://s0/4143/4173) | permits | [ExpandReference](sdl-source://s0/2528/2552) | [f0472](sdl-source://s0/40017/40062) |
| [InteractionObservationShape](sdl-source://s0/4225/4262) | has-field | [ObservationContext](sdl-source://s0/5244/5269) | [f0478](sdl-source://s0/40310/40367) |
| [InterpretationCallsContract](sdl-source://s0/4292/4329) | permits | [ParseSnapshot](sdl-source://s0/5509/5531) | [f0481](sdl-source://s0/40480/40530) |
| [InterpretationCallsContract](sdl-source://s0/4292/4329) | permits | [ParsedSnapshot](sdl-source://s0/5532/5555) | [f0482](sdl-source://s0/40531/40582) |
| [LayoutCallsContract](sdl-source://s0/4495/4524) | permits | [LayoutModel](sdl-source://s0/4552/4572) | [f0505](sdl-source://s0/42162/42202) |
| [LayoutCallsContract](sdl-source://s0/4495/4524) | permits | [ModelLaidOut](sdl-source://s0/5016/5037) | [f0506](sdl-source://s0/42203/42244) |
| [LeaseLifecycleContract](sdl-source://s0/4625/4657) | permits | [InstallNewLease](sdl-source://s0/4200/4224) | [f0515](sdl-source://s0/42635/42682) |
| [LeaseLifecycleContract](sdl-source://s0/4625/4657) | permits | [KeepUnsentRelease](sdl-source://s0/4414/4440) | [f0516](sdl-source://s0/42683/42732) |
| [LeaseLifecycleContract](sdl-source://s0/4625/4657) | permits | [QueuePreviousLease](sdl-source://s0/6626/6653) | [f0517](sdl-source://s0/42733/42783) |
| [LeaseLifecycleContract](sdl-source://s0/4625/4657) | permits | [RemoveSentRelease](sdl-source://s0/7108/7134) | [f0518](sdl-source://s0/42784/42833) |
| [LeaseNotificationsContract](sdl-source://s0/4686/4722) | permits | [AttemptRelease](sdl-source://s0/417/440) | [f0521](sdl-source://s0/42943/42993) |
| [NavigationCallsContract](sdl-source://s0/5125/5158) | permits | [OpenTarget](sdl-source://s0/5270/5289) | [f0585](sdl-source://s0/47498/47541) |
| [NavigationCallsContract](sdl-source://s0/5125/5158) | permits | [TargetOpened](sdl-source://s0/8044/8065) | [f0586](sdl-source://s0/47542/47587) |
| [OpeningPolicyContract](sdl-source://s0/5313/5344) | permits | [ClassifyExternalTarget](sdl-source://s0/970/1001) | [f0599](sdl-source://s0/48188/48241) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | permits | [AdoptPreferencesDraft](sdl-source://s0/237/267) | [f0646](sdl-source://s0/51265/51321) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | permits | [BeginPreferencesDraft](sdl-source://s0/441/471) | [f0647](sdl-source://s0/51322/51378) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | permits | [NotifyPreferenceChange](sdl-source://s0/5212/5243) | [f0648](sdl-source://s0/51379/51436) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | permits | [PersistPreferencesDraft](sdl-source://s0/5652/5684) | [f0649](sdl-source://s0/51437/51495) |
| [PreferenceChangesContract](sdl-source://s0/5738/5773) | permits | [ValidatePreferencesDraft](sdl-source://s0/8377/8410) | [f0650](sdl-source://s0/51496/51555) |
| [PresentationEventsContract](sdl-source://s0/6042/6078) | permits | [PresentFrame](sdl-source://s0/5992/6013) | [f0681](sdl-source://s0/53833/53881) |
| [PresentationEventsContract](sdl-source://s0/6042/6078) | permits | [ShowDirtyRejection](sdl-source://s0/7840/7867) | [f0682](sdl-source://s0/53882/53936) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | permits | [AcceptCurrentFrame](sdl-source://s0/34/61) | [f0712](sdl-source://s0/56038/56090) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | permits | [AcceptCurrentModel](sdl-source://s0/62/89) | [f0713](sdl-source://s0/56091/56143) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | permits | [AcceptWorkerTicket](sdl-source://s0/90/117) | [f0714](sdl-source://s0/56144/56196) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | permits | [CompleteObsoleteWorker](sdl-source://s0/1051/1082) | [f0715](sdl-source://s0/56197/56253) |
| [PreviewAdmissionContract](sdl-source://s0/6119/6153) | permits | [DiscardObsoleteCompletion](sdl-source://s0/1682/1716) | [f0716](sdl-source://s0/56254/56313) |
| [PublicationDrawingContract](sdl-source://s0/6344/6380) | permits | [PaintPdfCandidate](sdl-source://s0/5402/5428) | [f0765](sdl-source://s0/60037/60090) |
| [PublicationInputsContract](sdl-source://s0/6408/6443) | permits | [FreezeExportSnapshot](sdl-source://s0/3255/3284) | [f0768](sdl-source://s0/60197/60252) |
| [PublicationIntentsContract](sdl-source://s0/6472/6508) | permits | [CancelPdfRequest](sdl-source://s0/582/607) | [f0771](sdl-source://s0/60362/60414) |
| [PublicationIntentsContract](sdl-source://s0/6472/6508) | permits | [RequestPdfExport](sdl-source://s0/7253/7278) | [f0772](sdl-source://s0/60415/60467) |
| [PublicationStateContract](sdl-source://s0/6535/6569) | permits | [CancelUnpublishedJob](sdl-source://s0/637/666) | [f0775](sdl-source://s0/60571/60625) |
| [PublicationStateContract](sdl-source://s0/6535/6569) | permits | [CommitPdfCandidate](sdl-source://s0/1002/1029) | [f0776](sdl-source://s0/60626/60678) |
| [PublicationStateContract](sdl-source://s0/6535/6569) | permits | [DiscardPdfCandidate](sdl-source://s0/1717/1745) | [f0777](sdl-source://s0/60679/60732) |
| [ReferenceInterpretationContract](sdl-source://s0/6832/6873) | permits | [ParseReference](sdl-source://s0/5485/5508) | [f0809](sdl-source://s0/62692/62747) |
| [ReferenceInterpretationContract](sdl-source://s0/6832/6873) | permits | [ReferenceParsed](sdl-source://s0/6874/6898) | [f0810](sdl-source://s0/62748/62804) |
| [ReferenceStorageContract](sdl-source://s0/6948/6982) | permits | [ReadReference](sdl-source://s0/6722/6744) | [f0819](sdl-source://s0/63179/63226) |
| [ReferenceStorageContract](sdl-source://s0/6948/6982) | permits | [ReferenceRead](sdl-source://s0/6899/6921) | [f0820](sdl-source://s0/63227/63274) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | permits | [CacheCompatibleScene](sdl-source://s0/552/581) | [f0861](sdl-source://s0/65748/65805) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | permits | [PrepareDiagramScenes](sdl-source://s0/5867/5896) | [f0862](sdl-source://s0/65806/65863) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | permits | [PrepareEmbeddedResources](sdl-source://s0/5897/5930) | [f0863](sdl-source://s0/65864/65925) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | permits | [PrepareExportResources](sdl-source://s0/5931/5962) | [f0864](sdl-source://s0/65926/65985) |
| [ResourcePreparationContract](sdl-source://s0/7336/7373) | permits | [PrepareTypedDiagram](sdl-source://s0/5963/5991) | [f0865](sdl-source://s0/65986/66042) |
| [ScheduledWorkContract](sdl-source://s0/7509/7540) | permits | [DebounceElapsed](sdl-source://s0/1264/1288) | [f0886](sdl-source://s0/67483/67529) |
| [ScheduledWorkContract](sdl-source://s0/7509/7540) | permits | [PollReleaseQueue](sdl-source://s0/5685/5710) | [f0887](sdl-source://s0/67530/67577) |
| [ScheduledWorkContract](sdl-source://s0/7509/7540) | permits | [RestartDebounce](sdl-source://s0/7374/7398) | [f0888](sdl-source://s0/67578/67624) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | permits | [CandidateRead](sdl-source://s0/667/689) | [f0923](sdl-source://s0/69466/69509) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | permits | [CandidateReadFailed](sdl-source://s0/690/718) | [f0924](sdl-source://s0/69510/69559) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | permits | [CurrentWritten](sdl-source://s0/1240/1263) | [f0925](sdl-source://s0/69560/69604) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | permits | [ReadCandidate](sdl-source://s0/6654/6676) | [f0926](sdl-source://s0/69605/69648) |
| [StorageCallsContract](sdl-source://s0/7982/8012) | permits | [WriteCurrent](sdl-source://s0/8927/8948) | [f0927](sdl-source://s0/69649/69691) |
| [UnsavedAdmissionContract](sdl-source://s0/8292/8326) | permits | [ChooseCancel](sdl-source://s0/839/860) | [f0952](sdl-source://s0/70878/70924) |
| [UnsavedAdmissionContract](sdl-source://s0/8292/8326) | permits | [ChooseSave](sdl-source://s0/889/908) | [f0953](sdl-source://s0/70925/70969) |
| [UnsavedAdmissionContract](sdl-source://s0/8292/8326) | permits | [ChooseUnsaved](sdl-source://s0/909/931) | [f0954](sdl-source://s0/70970/71017) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | permits | [CheckCurrentMapping](sdl-source://s0/776/804) | [f0976](sdl-source://s0/72264/72316) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | permits | [MapSourceAnchor](sdl-source://s0/4835/4859) | [f0977](sdl-source://s0/72317/72365) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | permits | [RetainDocumentAndAnchor](sdl-source://s0/7399/7431) | [f0978](sdl-source://s0/72366/72422) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | permits | [SetEditorAnchor](sdl-source://s0/7815/7839) | [f0979](sdl-source://s0/72423/72471) |
| [ViewportChangesContract](sdl-source://s0/8542/8575) | permits | [UserPreviewScroll](sdl-source://s0/8350/8376) | [f0980](sdl-source://s0/72472/72522) |
| [WindowPresentationContract](sdl-source://s0/8660/8696) | permits | [ToggleFullscreen](sdl-source://s0/8158/8183) | [f0995](sdl-source://s0/73593/73645) |
| [WorkspaceChangesContract](sdl-source://s0/8798/8832) | permits | [ActivateRootAndHistory](sdl-source://s0/150/181) | [f1009](sdl-source://s0/74517/74573) |
| [WorkspaceChangesContract](sdl-source://s0/8798/8832) | permits | [RequestTreeRefresh](sdl-source://s0/7279/7306) | [f1010](sdl-source://s0/74574/74626) |
| [WorkspaceChangesContract](sdl-source://s0/8798/8832) | permits | [SetChosenParentRoot](sdl-source://s0/7786/7814) | [f1011](sdl-source://s0/74627/74680) |
| [WorkspaceChangesContract](sdl-source://s0/8798/8832) | permits | [ValidateProposedRoot](sdl-source://s0/8411/8440) | [f1012](sdl-source://s0/74681/74735) |
| [WorkspaceIntentsContract](sdl-source://s0/8892/8926) | permits | [ActivateExternalTarget](sdl-source://s0/118/149) | [f1025](sdl-source://s0/75382/75438) |
| [WorkspaceIntentsContract](sdl-source://s0/8892/8926) | permits | [ChooseInternalFile](sdl-source://s0/861/888) | [f1026](sdl-source://s0/75439/75491) |
| [WorkspaceIntentsContract](sdl-source://s0/8892/8926) | permits | [FavoriteFileChosen](sdl-source://s0/3045/3072) | [f1027](sdl-source://s0/75492/75544) |
| [WorkspaceIntentsContract](sdl-source://s0/8892/8926) | permits | [FavoriteFolderChosen](sdl-source://s0/3137/3166) | [f1028](sdl-source://s0/75545/75599) |

## Projection responsibilities

Projects relates Functionality, Dataset and Datagram together; it does not imply a lossless conversion or a runtime call.

No projections are declared.

