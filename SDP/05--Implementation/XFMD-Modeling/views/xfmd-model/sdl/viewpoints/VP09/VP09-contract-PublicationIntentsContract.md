# Contract structure: PublicationIntentsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: PublicationIntentsContract

```mermaid
flowchart LR
    n_CancelPdfRequest["CancelPdfRequest (message)"]
    n_PublicationIntentsContract["PublicationIntentsContract (contract)"]
    n_RequestPdfExport["RequestPdfExport (message)"]
    n_PublicationIntentsContract -->|permits| n_CancelPdfRequest
    n_PublicationIntentsContract -->|permits| n_RequestPdfExport
```

Source facts: f0771, f0772.

