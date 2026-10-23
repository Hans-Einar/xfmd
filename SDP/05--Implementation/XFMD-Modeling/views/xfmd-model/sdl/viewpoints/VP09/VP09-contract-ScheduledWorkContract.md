# Contract structure: ScheduledWorkContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ScheduledWorkContract

```mermaid
flowchart LR
    n_DebounceElapsed["DebounceElapsed (message)"]
    n_PollReleaseQueue["PollReleaseQueue (message)"]
    n_RestartDebounce["RestartDebounce (message)"]
    n_ScheduledWorkContract["ScheduledWorkContract (contract)"]
    n_ScheduledWorkContract -->|permits| n_DebounceElapsed
    n_ScheduledWorkContract -->|permits| n_PollReleaseQueue
    n_ScheduledWorkContract -->|permits| n_RestartDebounce
```

Source facts: f0886, f0887, f0888.

