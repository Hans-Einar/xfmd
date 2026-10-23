# Contract structure: PreviewAdmissionContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: PreviewAdmissionContract

```mermaid
flowchart LR
    n_AcceptCurrentFrame["AcceptCurrentFrame (message)"]
    n_AcceptCurrentModel["AcceptCurrentModel (message)"]
    n_AcceptWorkerTicket["AcceptWorkerTicket (message)"]
    n_CompleteObsoleteWorker["CompleteObsoleteWorker (message)"]
    n_DiscardObsoleteCompletion["DiscardObsoleteCompletion (message)"]
    n_PreviewAdmissionContract["PreviewAdmissionContract (contract)"]
    n_PreviewAdmissionContract -->|permits| n_AcceptCurrentFrame
    n_PreviewAdmissionContract -->|permits| n_AcceptCurrentModel
    n_PreviewAdmissionContract -->|permits| n_AcceptWorkerTicket
    n_PreviewAdmissionContract -->|permits| n_CompleteObsoleteWorker
    n_PreviewAdmissionContract -->|permits| n_DiscardObsoleteCompletion
```

Source facts: f0712, f0713, f0714, f0715, f0716.

