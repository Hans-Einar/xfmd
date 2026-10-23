# Contract structure: DocumentCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: DocumentCallsContract

```mermaid
flowchart LR
    n_AdmitDocument["AdmitDocument (message)"]
    n_DocumentCallsContract["DocumentCallsContract (contract)"]
    n_DocumentOpened["DocumentOpened (message)"]
    n_DocumentRejected["DocumentRejected (message)"]
    n_DocumentCallsContract -->|permits| n_AdmitDocument
    n_DocumentCallsContract -->|permits| n_DocumentOpened
    n_DocumentCallsContract -->|permits| n_DocumentRejected
```

Source facts: f0169, f0170, f0171.

