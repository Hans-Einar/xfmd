# Contract structure: GenerationLifecycleContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GenerationLifecycleContract

```mermaid
flowchart LR
    n_CancelPreviousChild["CancelPreviousChild (message)"]
    n_GenerationLifecycleContract["GenerationLifecycleContract (contract)"]
    n_StartGeneration["StartGeneration (message)"]
    n_GenerationLifecycleContract -->|permits| n_CancelPreviousChild
    n_GenerationLifecycleContract -->|permits| n_StartGeneration
```

Source facts: f0455, f0456.

