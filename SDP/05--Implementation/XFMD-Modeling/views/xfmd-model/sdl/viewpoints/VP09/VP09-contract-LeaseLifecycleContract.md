# Contract structure: LeaseLifecycleContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: LeaseLifecycleContract

```mermaid
flowchart LR
    n_InstallNewLease["InstallNewLease (message)"]
    n_KeepUnsentRelease["KeepUnsentRelease (message)"]
    n_LeaseLifecycleContract["LeaseLifecycleContract (contract)"]
    n_QueuePreviousLease["QueuePreviousLease (message)"]
    n_RemoveSentRelease["RemoveSentRelease (message)"]
    n_LeaseLifecycleContract -->|permits| n_InstallNewLease
    n_LeaseLifecycleContract -->|permits| n_KeepUnsentRelease
    n_LeaseLifecycleContract -->|permits| n_QueuePreviousLease
    n_LeaseLifecycleContract -->|permits| n_RemoveSentRelease
```

Source facts: f0515, f0516, f0517, f0518.

