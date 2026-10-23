# Contract structure: NavigationCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: NavigationCallsContract

```mermaid
flowchart LR
    n_NavigationCallsContract["NavigationCallsContract (contract)"]
    n_OpenTarget["OpenTarget (message)"]
    n_TargetOpened["TargetOpened (message)"]
    n_NavigationCallsContract -->|permits| n_OpenTarget
    n_NavigationCallsContract -->|permits| n_TargetOpened
```

Source facts: f0585, f0586.

