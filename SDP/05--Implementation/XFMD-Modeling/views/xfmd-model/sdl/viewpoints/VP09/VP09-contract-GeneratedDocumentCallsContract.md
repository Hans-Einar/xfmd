# Contract structure: GeneratedDocumentCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GeneratedDocumentCallsContract

```mermaid
flowchart LR
    n_AdmitGeneratedDocument["AdmitGeneratedDocument (message)"]
    n_GeneratedDocumentCallsContract["GeneratedDocumentCallsContract (contract)"]
    n_GeneratedDocumentOpened["GeneratedDocumentOpened (message)"]
    n_GeneratedDocumentCallsContract -->|permits| n_AdmitGeneratedDocument
    n_GeneratedDocumentCallsContract -->|permits| n_GeneratedDocumentOpened
```

Source facts: f0380, f0381.

