# Contract structure: StorageCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: StorageCallsContract

```mermaid
flowchart LR
    n_CandidateRead["CandidateRead (message)"]
    n_CandidateReadFailed["CandidateReadFailed (message)"]
    n_CurrentWritten["CurrentWritten (message)"]
    n_ReadCandidate["ReadCandidate (message)"]
    n_StorageCallsContract["StorageCallsContract (contract)"]
    n_WriteCurrent["WriteCurrent (message)"]
    n_StorageCallsContract -->|permits| n_CandidateRead
    n_StorageCallsContract -->|permits| n_CandidateReadFailed
    n_StorageCallsContract -->|permits| n_CurrentWritten
    n_StorageCallsContract -->|permits| n_ReadCandidate
    n_StorageCallsContract -->|permits| n_WriteCurrent
```

Source facts: f0923, f0924, f0925, f0926, f0927.

