# Contract structure: ResourcePreparationContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ResourcePreparationContract

```mermaid
flowchart LR
    n_CacheCompatibleScene["CacheCompatibleScene (message)"]
    n_PrepareDiagramScenes["PrepareDiagramScenes (message)"]
    n_PrepareEmbeddedResources["PrepareEmbeddedResources (message)"]
    n_PrepareExportResources["PrepareExportResources (message)"]
    n_PrepareTypedDiagram["PrepareTypedDiagram (message)"]
    n_ResourcePreparationContract["ResourcePreparationContract (contract)"]
    n_ResourcePreparationContract -->|permits| n_CacheCompatibleScene
    n_ResourcePreparationContract -->|permits| n_PrepareDiagramScenes
    n_ResourcePreparationContract -->|permits| n_PrepareEmbeddedResources
    n_ResourcePreparationContract -->|permits| n_PrepareExportResources
    n_ResourcePreparationContract -->|permits| n_PrepareTypedDiagram
```

Source facts: f0861, f0862, f0863, f0864, f0865.

