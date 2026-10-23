# Contract structure: LayoutCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: LayoutCallsContract

```mermaid
flowchart LR
    n_LayoutCallsContract["LayoutCallsContract (contract)"]
    n_LayoutModel["LayoutModel (message)"]
    n_ModelLaidOut["ModelLaidOut (message)"]
    n_LayoutCallsContract -->|permits| n_LayoutModel
    n_LayoutCallsContract -->|permits| n_ModelLaidOut
```

Source facts: f0505, f0506.

