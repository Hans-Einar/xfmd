# Contract structure: PresentationEventsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: PresentationEventsContract

```mermaid
flowchart LR
    n_PresentFrame["PresentFrame (message)"]
    n_PresentationEventsContract["PresentationEventsContract (contract)"]
    n_ShowDirtyRejection["ShowDirtyRejection (message)"]
    n_PresentationEventsContract -->|permits| n_PresentFrame
    n_PresentationEventsContract -->|permits| n_ShowDirtyRejection
```

Source facts: f0681, f0682.

