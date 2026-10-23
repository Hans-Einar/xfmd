# Logical decomposition: Application

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Logical decomposition: Application

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_DocumentIndexService["DocumentIndexService (unit)"]
    n_DocumentStorage["DocumentStorage (unit)"]
    n_Documents["Documents (unit)"]
    n_EventLoop["EventLoop (unit)"]
    n_GeneratedViews["GeneratedViews (unit)"]
    n_Identity["Identity (unit)"]
    n_NativeShell["NativeShell (unit)"]
    n_Navigation["Navigation (unit)"]
    n_Preferences["Preferences (unit)"]
    n_Preview["Preview (unit)"]
    n_Publication["Publication (unit)"]
    n_SdpProjects["SdpProjects (unit)"]
    n_Typography["Typography (unit)"]
    n_Viewport["Viewport (unit)"]
    n_VisualResources["VisualResources (unit)"]
    n_Workspace["Workspace (unit)"]
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
```

Source facts: f0019, f0020, f0021, f0022, f0023, f0024, f0025, f0026, f0027, f0028, f0029, f0030, f0031, f0032, f0033, f0034.

