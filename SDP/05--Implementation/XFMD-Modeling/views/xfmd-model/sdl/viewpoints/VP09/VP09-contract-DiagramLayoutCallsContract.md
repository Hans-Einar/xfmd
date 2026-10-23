# Contract structure: DiagramLayoutCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: DiagramLayoutCallsContract

```mermaid
flowchart LR
    n_DiagramLayoutCallsContract["DiagramLayoutCallsContract (contract)"]
    n_LayoutTypedDiagram["LayoutTypedDiagram (message)"]
    n_TypedDiagramLaidOut["TypedDiagramLaidOut (message)"]
    n_DiagramLayoutCallsContract -->|permits| n_LayoutTypedDiagram
    n_DiagramLayoutCallsContract -->|permits| n_TypedDiagramLaidOut
```

Source facts: f0133, f0134.

