# Contract structure: ExportInterpretationContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ExportInterpretationContract

```mermaid
flowchart LR
    n_ExportInterpretationContract["ExportInterpretationContract (contract)"]
    n_ExportSnapshotParsed["ExportSnapshotParsed (message)"]
    n_ParseExportSnapshot["ParseExportSnapshot (message)"]
    n_ExportInterpretationContract -->|permits| n_ExportSnapshotParsed
    n_ExportInterpretationContract -->|permits| n_ParseExportSnapshot
```

Source facts: f0290, f0291.

