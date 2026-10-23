# Contract structure: ReferenceStorageContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ReferenceStorageContract

```mermaid
flowchart LR
    n_ReadReference["ReadReference (message)"]
    n_ReferenceRead["ReferenceRead (message)"]
    n_ReferenceStorageContract["ReferenceStorageContract (contract)"]
    n_ReferenceStorageContract -->|permits| n_ReadReference
    n_ReferenceStorageContract -->|permits| n_ReferenceRead
```

Source facts: f0819, f0820.

