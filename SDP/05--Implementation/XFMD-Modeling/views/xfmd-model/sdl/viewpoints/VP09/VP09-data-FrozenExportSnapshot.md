# Data origin and holder: FrozenExportSnapshot

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Data origin and holder: FrozenExportSnapshot

```mermaid
flowchart LR
    n_ExportSnapshotShape["ExportSnapshotShape (contract)"]
    n_FrozenExportSnapshot["FrozenExportSnapshot (dataset)"]
    n_Publication["Publication (unit)"]
    n_FrozenExportSnapshot -->|upholds| n_ExportSnapshotShape
    n_Publication -->|holds| n_FrozenExportSnapshot
```

Source facts: f0361, f0746.

