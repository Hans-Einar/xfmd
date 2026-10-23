# Contract structure: DocumentStateChangesContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: DocumentStateChangesContract

```mermaid
flowchart LR
    n_DocumentStateChangesContract["DocumentStateChangesContract (contract)"]
    n_MarkCurrentSaved["MarkCurrentSaved (message)"]
    n_ReplaceDocument["ReplaceDocument (message)"]
    n_DocumentStateChangesContract -->|permits| n_MarkCurrentSaved
    n_DocumentStateChangesContract -->|permits| n_ReplaceDocument
```

Source facts: f0214, f0215.

