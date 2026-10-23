# Data origin and holder: MainDocumentState

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Data origin and holder: MainDocumentState

```mermaid
flowchart LR
    n_DocumentStateShape["DocumentStateShape (contract)"]
    n_Documents["Documents (unit)"]
    n_MainDocumentState["MainDocumentState (dataset)"]
    n_Documents -->|holds| n_MainDocumentState
    n_MainDocumentState -->|upholds| n_DocumentStateShape
```

Source facts: f0231, f0526.

