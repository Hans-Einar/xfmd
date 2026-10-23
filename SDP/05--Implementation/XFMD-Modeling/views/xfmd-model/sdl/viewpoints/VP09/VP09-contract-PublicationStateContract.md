# Contract structure: PublicationStateContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: PublicationStateContract

```mermaid
flowchart LR
    n_CancelUnpublishedJob["CancelUnpublishedJob (message)"]
    n_CommitPdfCandidate["CommitPdfCandidate (message)"]
    n_DiscardPdfCandidate["DiscardPdfCandidate (message)"]
    n_PublicationStateContract["PublicationStateContract (contract)"]
    n_PublicationStateContract -->|permits| n_CancelUnpublishedJob
    n_PublicationStateContract -->|permits| n_CommitPdfCandidate
    n_PublicationStateContract -->|permits| n_DiscardPdfCandidate
```

Source facts: f0775, f0776, f0777.

