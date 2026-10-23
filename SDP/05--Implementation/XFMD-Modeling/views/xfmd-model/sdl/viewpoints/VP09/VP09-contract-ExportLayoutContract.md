# Contract structure: ExportLayoutContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ExportLayoutContract

```mermaid
flowchart LR
    n_ExportLayoutContract["ExportLayoutContract (contract)"]
    n_ExportPagesLaidOut["ExportPagesLaidOut (message)"]
    n_LayoutExportPages["LayoutExportPages (message)"]
    n_ExportLayoutContract -->|permits| n_ExportPagesLaidOut
    n_ExportLayoutContract -->|permits| n_LayoutExportPages
```

Source facts: f0294, f0295.

