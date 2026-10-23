# Contract structure: DocumentNotificationsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: DocumentNotificationsContract

```mermaid
flowchart LR
    n_CommitVisit["CommitVisit (message)"]
    n_DocumentChanged["DocumentChanged (message)"]
    n_DocumentNotificationsContract["DocumentNotificationsContract (contract)"]
    n_GeneratedDocumentChanged["GeneratedDocumentChanged (message)"]
    n_DocumentNotificationsContract -->|permits| n_CommitVisit
    n_DocumentNotificationsContract -->|permits| n_DocumentChanged
    n_DocumentNotificationsContract -->|permits| n_GeneratedDocumentChanged
```

Source facts: f0193, f0194, f0195.

