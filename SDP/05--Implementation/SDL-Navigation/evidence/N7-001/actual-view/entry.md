# VP04 — Interfaces and collaboration

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

consumes records use; no provider, Channel or executable message flow is inferred.

## All declared interfaces and explicit consumers

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_DiagramInterpreterPort("DiagramInterpreterPort (interface)")
    n_DiagramLayoutPort("DiagramLayoutPort (interface)")
    n_DocumentIndexService["DocumentIndexService (unit)"]
    n_Documents["Documents (unit)"]
    n_ExternalOpenProtocol("ExternalOpenProtocol (interface)")
    n_FileStorePort("FileStorePort (interface)")
    n_GeneratedViews["GeneratedViews (unit)"]
    n_Interpreter["Interpreter (unit)"]
    n_InterpreterPort("InterpreterPort (interface)")
    n_LeaseReleaseProtocol("LeaseReleaseProtocol (interface)")
    n_Preferences["Preferences (unit)"]
    n_PreferencesStorePort("PreferencesStorePort (interface)")
    n_Preview["Preview (unit)"]
    n_Publication["Publication (unit)"]
    n_RenderHostPort("RenderHostPort (interface)")
    n_Renderer["Renderer (unit)"]
    n_RendererPort("RendererPort (interface)")
    n_SchedulerPort("SchedulerPort (interface)")
    n_SdpProjects["SdpProjects (unit)"]
    n_SdpToolProtocol("SdpToolProtocol (interface)")
    n_TextMetricsPort("TextMetricsPort (interface)")
    n_ViewGenerationProtocol("ViewGenerationProtocol (interface)")
    n_Viewport["Viewport (unit)"]
    n_VisualResources["VisualResources (unit)"]
    n_WindowDeliveryProtocol("WindowDeliveryProtocol (interface)")
    n_Application -.->|consumes| n_ExternalOpenProtocol
    n_DocumentIndexService -.->|consumes| n_FileStorePort
    n_DocumentIndexService -.->|consumes| n_InterpreterPort
    n_Documents -.->|consumes| n_FileStorePort
    n_GeneratedViews -.->|consumes| n_LeaseReleaseProtocol
    n_GeneratedViews -.->|consumes| n_SchedulerPort
    n_GeneratedViews -.->|consumes| n_ViewGenerationProtocol
    n_GeneratedViews -.->|consumes| n_WindowDeliveryProtocol
    n_Interpreter -.->|consumes| n_DiagramInterpreterPort
    n_Preferences -.->|consumes| n_PreferencesStorePort
    n_Preview -.->|consumes| n_InterpreterPort
    n_Preview -.->|consumes| n_RenderHostPort
    n_Preview -.->|consumes| n_RendererPort
    n_Preview -.->|consumes| n_SchedulerPort
    n_Publication -.->|consumes| n_InterpreterPort
    n_Publication -.->|consumes| n_RendererPort
    n_Publication -.->|consumes| n_TextMetricsPort
    n_Renderer -.->|consumes| n_TextMetricsPort
    n_SdpProjects -.->|consumes| n_SchedulerPort
    n_SdpProjects -.->|consumes| n_SdpToolProtocol
    n_Viewport -.->|consumes| n_RenderHostPort
    n_Viewport -.->|consumes| n_SchedulerPort
    n_VisualResources -.->|consumes| n_DiagramLayoutPort
    n_VisualResources -.->|consumes| n_TextMetricsPort
```

Source facts: [f0018](sdl-source://s0/9919/9961), [f0179](sdl-source://s0/19545/19589), [f0180](sdl-source://s0/19590/19636), [f0230](sdl-source://s0/22692/22725), [f0419](sdl-source://s0/36185/36230), [f0420](sdl-source://s0/36231/36269), [f0421](sdl-source://s0/36270/36317), [f0422](sdl-source://s0/36318/36365), [f0483](sdl-source://s0/40583/40627), [f0651](sdl-source://s0/51556/51598), [f0683](sdl-source://s0/53937/53970), [f0684](sdl-source://s0/53971/54003), [f0685](sdl-source://s0/54004/54034), [f0686](sdl-source://s0/54035/54066), [f0743](sdl-source://s0/58379/58416), [f0744](sdl-source://s0/58417/58451), [f0745](sdl-source://s0/58452/58489), [f0843](sdl-source://s0/64725/64759), [f0894](sdl-source://s0/67911/67946), [f0895](sdl-source://s0/67947/67984), [f0963](sdl-source://s0/71434/71467), [f0964](sdl-source://s0/71468/71500), [f0981](sdl-source://s0/72523/72566), [f0982](sdl-source://s0/72567/72608).

Declarations: [Application](sdl-source://s0/303/320), [DiagramInterpreterPort](sdl-source://s0/1390/1423), [DiagramLayoutPort](sdl-source://s0/1518/1546), [DocumentIndexService](sdl-source://s0/1928/1954), [Documents](sdl-source://s0/2327/2342), [ExternalOpenProtocol](sdl-source://s0/2867/2898), [FileStorePort](sdl-source://s0/3167/3191), [GeneratedViews](sdl-source://s0/3908/3928), [InterpreterPort](sdl-source://s0/4348/4374), [Interpreter](sdl-source://s0/4330/4347), [LeaseReleaseProtocol](sdl-source://s0/4723/4754), [PreferencesStorePort](sdl-source://s0/5835/5866), [Preferences](sdl-source://s0/5774/5791), [Preview](sdl-source://s0/6079/6092), [Publication](sdl-source://s0/6298/6315), [RenderHostPort](sdl-source://s0/7135/7160), [RendererPort](sdl-source://s0/7204/7227), [Renderer](sdl-source://s0/7189/7203), [SchedulerPort](sdl-source://s0/7541/7565), [SdpProjects](sdl-source://s0/7655/7672), [SdpToolProtocol](sdl-source://s0/7673/7699), [TextMetricsPort](sdl-source://s0/8131/8157), [ViewGenerationProtocol](sdl-source://s0/8468/8501), [Viewport](sdl-source://s0/8502/8516), [VisualResources](sdl-source://s0/8576/8597), [WindowDeliveryProtocol](sdl-source://s0/8598/8631).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [DiagramInterpreterPort](sdl-source://s0/1390/1423) | [Interpreter](sdl-source://s0/4330/4347) | [f0483](sdl-source://s0/40583/40627) |
| [DiagramLayoutPort](sdl-source://s0/1518/1546) | [VisualResources](sdl-source://s0/8576/8597) | [f0981](sdl-source://s0/72523/72566) |
| [ExternalOpenProtocol](sdl-source://s0/2867/2898) | [Application](sdl-source://s0/303/320) | [f0018](sdl-source://s0/9919/9961) |
| [FileStorePort](sdl-source://s0/3167/3191) | [DocumentIndexService](sdl-source://s0/1928/1954), [Documents](sdl-source://s0/2327/2342) | [f0179](sdl-source://s0/19545/19589), [f0230](sdl-source://s0/22692/22725) |
| [InterpreterPort](sdl-source://s0/4348/4374) | [DocumentIndexService](sdl-source://s0/1928/1954), [Preview](sdl-source://s0/6079/6092), [Publication](sdl-source://s0/6298/6315) | [f0180](sdl-source://s0/19590/19636), [f0683](sdl-source://s0/53937/53970), [f0743](sdl-source://s0/58379/58416) |
| [LeaseReleaseProtocol](sdl-source://s0/4723/4754) | [GeneratedViews](sdl-source://s0/3908/3928) | [f0419](sdl-source://s0/36185/36230) |
| [PreferencesStorePort](sdl-source://s0/5835/5866) | [Preferences](sdl-source://s0/5774/5791) | [f0651](sdl-source://s0/51556/51598) |
| [RenderHostPort](sdl-source://s0/7135/7160) | [Preview](sdl-source://s0/6079/6092), [Viewport](sdl-source://s0/8502/8516) | [f0684](sdl-source://s0/53971/54003), [f0963](sdl-source://s0/71434/71467) |
| [RendererPort](sdl-source://s0/7204/7227) | [Preview](sdl-source://s0/6079/6092), [Publication](sdl-source://s0/6298/6315) | [f0685](sdl-source://s0/54004/54034), [f0744](sdl-source://s0/58417/58451) |
| [SchedulerPort](sdl-source://s0/7541/7565) | [GeneratedViews](sdl-source://s0/3908/3928), [Preview](sdl-source://s0/6079/6092), [SdpProjects](sdl-source://s0/7655/7672), [Viewport](sdl-source://s0/8502/8516) | [f0420](sdl-source://s0/36231/36269), [f0686](sdl-source://s0/54035/54066), [f0894](sdl-source://s0/67911/67946), [f0964](sdl-source://s0/71468/71500) |
| [SdpToolProtocol](sdl-source://s0/7673/7699) | [SdpProjects](sdl-source://s0/7655/7672) | [f0895](sdl-source://s0/67947/67984) |
| [TextMetricsPort](sdl-source://s0/8131/8157) | [Publication](sdl-source://s0/6298/6315), [Renderer](sdl-source://s0/7189/7203), [VisualResources](sdl-source://s0/8576/8597) | [f0745](sdl-source://s0/58452/58489), [f0843](sdl-source://s0/64725/64759), [f0982](sdl-source://s0/72567/72608) |
| [ViewGenerationProtocol](sdl-source://s0/8468/8501) | [GeneratedViews](sdl-source://s0/3908/3928) | [f0421](sdl-source://s0/36270/36317) |
| [WindowDeliveryProtocol](sdl-source://s0/8598/8631) | [GeneratedViews](sdl-source://s0/3908/3928) | [f0422](sdl-source://s0/36318/36365) |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Application | ExternalOpenProtocol | [f0018](sdl-source://s0/9919/9961) | 332 |
| DocumentIndexService | FileStorePort | [f0179](sdl-source://s0/19545/19589) | 493 |
| DocumentIndexService | InterpreterPort | [f0180](sdl-source://s0/19590/19636) | 494 |
| Documents | FileStorePort | [f0230](sdl-source://s0/22692/22725) | 544 |
| GeneratedViews | LeaseReleaseProtocol | [f0419](sdl-source://s0/36185/36230) | 733 |
| GeneratedViews | SchedulerPort | [f0420](sdl-source://s0/36231/36269) | 734 |
| GeneratedViews | ViewGenerationProtocol | [f0421](sdl-source://s0/36270/36317) | 735 |
| GeneratedViews | WindowDeliveryProtocol | [f0422](sdl-source://s0/36318/36365) | 736 |
| Interpreter | DiagramInterpreterPort | [f0483](sdl-source://s0/40583/40627) | 797 |
| Preferences | PreferencesStorePort | [f0651](sdl-source://s0/51556/51598) | 965 |
| Preview | InterpreterPort | [f0683](sdl-source://s0/53937/53970) | 997 |
| Preview | RenderHostPort | [f0684](sdl-source://s0/53971/54003) | 998 |
| Preview | RendererPort | [f0685](sdl-source://s0/54004/54034) | 999 |
| Preview | SchedulerPort | [f0686](sdl-source://s0/54035/54066) | 1000 |
| Publication | InterpreterPort | [f0743](sdl-source://s0/58379/58416) | 1057 |
| Publication | RendererPort | [f0744](sdl-source://s0/58417/58451) | 1058 |
| Publication | TextMetricsPort | [f0745](sdl-source://s0/58452/58489) | 1059 |
| Renderer | TextMetricsPort | [f0843](sdl-source://s0/64725/64759) | 1157 |
| SdpProjects | SchedulerPort | [f0894](sdl-source://s0/67911/67946) | 1208 |
| SdpProjects | SdpToolProtocol | [f0895](sdl-source://s0/67947/67984) | 1209 |
| Viewport | RenderHostPort | [f0963](sdl-source://s0/71434/71467) | 1277 |
| Viewport | SchedulerPort | [f0964](sdl-source://s0/71468/71500) | 1278 |
| VisualResources | DiagramLayoutPort | [f0981](sdl-source://s0/72523/72566) | 1295 |
| VisualResources | TextMetricsPort | [f0982](sdl-source://s0/72567/72608) | 1296 |

