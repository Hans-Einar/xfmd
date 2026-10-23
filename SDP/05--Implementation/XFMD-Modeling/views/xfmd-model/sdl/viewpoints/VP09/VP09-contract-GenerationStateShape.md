# Contract structure: GenerationStateShape

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GenerationStateShape

```mermaid
flowchart LR
    n_GenerationSequence["GenerationSequence (field)"]
    n_GenerationStateShape["GenerationStateShape (contract)"]
    n_RegisteredProject["RegisteredProject (field)"]
    n_TargetPane["TargetPane (field)"]
    n_TargetWindow["TargetWindow (field)"]
    n_GenerationStateShape -->|has-field| n_GenerationSequence
    n_GenerationStateShape -->|has-field| n_RegisteredProject
    n_GenerationStateShape -->|has-field| n_TargetPane
    n_GenerationStateShape -->|has-field| n_TargetWindow
```

Source facts: f0460, f0461, f0462, f0463.

