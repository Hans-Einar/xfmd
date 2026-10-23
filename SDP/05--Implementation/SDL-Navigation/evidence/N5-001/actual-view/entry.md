# VP02 — Architecture and logical decomposition

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

Container/Unit and contains; library structure is not deployment allocation.

## Complete logical architecture

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_DocumentIndexService["DocumentIndexService (unit)"]
    n_DocumentStorage["DocumentStorage (unit)"]
    n_Documents["Documents (unit)"]
    n_EventLoop["EventLoop (unit)"]
    n_ExternalLauncher["ExternalLauncher (unit)"]
    n_ExternalViewBroker["ExternalViewBroker (unit)"]
    n_ExternalViewGenerator["ExternalViewGenerator (unit)"]
    n_GeneratedViews["GeneratedViews (unit)"]
    n_Identity["Identity (unit)"]
    n_Interpreter["Interpreter (unit)"]
    n_NativeShell["NativeShell (unit)"]
    n_Navigation["Navigation (unit)"]
    n_Preferences["Preferences (unit)"]
    n_Preview["Preview (unit)"]
    n_Publication["Publication (unit)"]
    n_Renderer["Renderer (unit)"]
    n_SdpProjects["SdpProjects (unit)"]
    n_Typography["Typography (unit)"]
    n_Viewport["Viewport (unit)"]
    n_VisualResources["VisualResources (unit)"]
    n_Workspace["Workspace (unit)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_Application -->|contains| n_DocumentIndexService
    n_Application -->|contains| n_DocumentStorage
    n_Application -->|contains| n_Documents
    n_Application -->|contains| n_EventLoop
    n_Application -->|contains| n_GeneratedViews
    n_Application -->|contains| n_Identity
    n_Application -->|contains| n_NativeShell
    n_Application -->|contains| n_Navigation
    n_Application -->|contains| n_Preferences
    n_Application -->|contains| n_Preview
    n_Application -->|contains| n_Publication
    n_Application -->|contains| n_SdpProjects
    n_Application -->|contains| n_Typography
    n_Application -->|contains| n_Viewport
    n_Application -->|contains| n_VisualResources
    n_Application -->|contains| n_Workspace
    n_XfmdDesktop -->|contains| n_Application
    n_XfmdDesktop -->|contains| n_Interpreter
    n_XfmdDesktop -->|contains| n_Renderer
```

Source facts: [f0019](sdl-source://s0/9962/10004), [f0020](sdl-source://s0/10005/10042), [f0021](sdl-source://s0/10043/10074), [f0022](sdl-source://s0/10075/10106), [f0023](sdl-source://s0/10107/10143), [f0024](sdl-source://s0/10144/10174), [f0025](sdl-source://s0/10175/10208), [f0026](sdl-source://s0/10209/10241), [f0027](sdl-source://s0/10242/10275), [f0028](sdl-source://s0/10276/10305), [f0029](sdl-source://s0/10306/10339), [f0030](sdl-source://s0/10340/10373), [f0031](sdl-source://s0/10374/10406), [f0032](sdl-source://s0/10407/10437), [f0033](sdl-source://s0/10438/10475), [f0034](sdl-source://s0/10476/10507), [f1031](sdl-source://s0/75691/75724), [f1032](sdl-source://s0/75725/75758), [f1033](sdl-source://s0/75759/75789).

Declarations: [Application](sdl-source://s0/303/320), [DocumentIndexService](sdl-source://s0/1928/1954), [DocumentStorage](sdl-source://s0/2305/2326), [Documents](sdl-source://s0/2327/2342), [EventLoop](sdl-source://s0/2481/2496), [ExternalLauncher](sdl-source://s0/2844/2866), [ExternalViewBroker](sdl-source://s0/2992/3016), [ExternalViewGenerator](sdl-source://s0/3017/3044), [GeneratedViews](sdl-source://s0/3908/3928), [Identity](sdl-source://s0/4053/4067), [Interpreter](sdl-source://s0/4330/4347), [NativeShell](sdl-source://s0/5038/5055), [Navigation](sdl-source://s0/5083/5099), [Preferences](sdl-source://s0/5774/5791), [Preview](sdl-source://s0/6079/6092), [Publication](sdl-source://s0/6298/6315), [Renderer](sdl-source://s0/7189/7203), [SdpProjects](sdl-source://s0/7655/7672), [Typography](sdl-source://s0/8213/8229), [Viewport](sdl-source://s0/8502/8516), [VisualResources](sdl-source://s0/8576/8597), [Workspace](sdl-source://s0/8756/8771), [XfmdDesktop](sdl-source://s0/8949/8971).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [XfmdDesktop](sdl-source://s0/8949/8971) | container | Root (no declared parent) | — |
| [Application](sdl-source://s0/303/320) | unit | XfmdDesktop | [f1031](sdl-source://s0/75691/75724) |
| [DocumentIndexService](sdl-source://s0/1928/1954) | unit | Application | [f0019](sdl-source://s0/9962/10004) |
| [DocumentStorage](sdl-source://s0/2305/2326) | unit | Application | [f0020](sdl-source://s0/10005/10042) |
| [Documents](sdl-source://s0/2327/2342) | unit | Application | [f0021](sdl-source://s0/10043/10074) |
| [EventLoop](sdl-source://s0/2481/2496) | unit | Application | [f0022](sdl-source://s0/10075/10106) |
| [ExternalLauncher](sdl-source://s0/2844/2866) | unit | Root (no declared parent) | — |
| [ExternalViewBroker](sdl-source://s0/2992/3016) | unit | Root (no declared parent) | — |
| [ExternalViewGenerator](sdl-source://s0/3017/3044) | unit | Root (no declared parent) | — |
| [GeneratedViews](sdl-source://s0/3908/3928) | unit | Application | [f0023](sdl-source://s0/10107/10143) |
| [Identity](sdl-source://s0/4053/4067) | unit | Application | [f0024](sdl-source://s0/10144/10174) |
| [Interpreter](sdl-source://s0/4330/4347) | unit | XfmdDesktop | [f1032](sdl-source://s0/75725/75758) |
| [NativeShell](sdl-source://s0/5038/5055) | unit | Application | [f0025](sdl-source://s0/10175/10208) |
| [Navigation](sdl-source://s0/5083/5099) | unit | Application | [f0026](sdl-source://s0/10209/10241) |
| [Preferences](sdl-source://s0/5774/5791) | unit | Application | [f0027](sdl-source://s0/10242/10275) |
| [Preview](sdl-source://s0/6079/6092) | unit | Application | [f0028](sdl-source://s0/10276/10305) |
| [Publication](sdl-source://s0/6298/6315) | unit | Application | [f0029](sdl-source://s0/10306/10339) |
| [Renderer](sdl-source://s0/7189/7203) | unit | XfmdDesktop | [f1033](sdl-source://s0/75759/75789) |
| [SdpProjects](sdl-source://s0/7655/7672) | unit | Application | [f0030](sdl-source://s0/10340/10373) |
| [Typography](sdl-source://s0/8213/8229) | unit | Application | [f0031](sdl-source://s0/10374/10406) |
| [Viewport](sdl-source://s0/8502/8516) | unit | Application | [f0032](sdl-source://s0/10407/10437) |
| [VisualResources](sdl-source://s0/8576/8597) | unit | Application | [f0033](sdl-source://s0/10438/10475) |
| [Workspace](sdl-source://s0/8756/8771) | unit | Application | [f0034](sdl-source://s0/10476/10507) |


