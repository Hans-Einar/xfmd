# Contract structure: IndexChangesContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: IndexChangesContract

```mermaid
flowchart LR
    n_CheckReferenceGeneration["CheckReferenceGeneration (message)"]
    n_IndexChangesContract["IndexChangesContract (contract)"]
    n_PublishReferenceHeadings["PublishReferenceHeadings (message)"]
    n_IndexChangesContract -->|permits| n_CheckReferenceGeneration
    n_IndexChangesContract -->|permits| n_PublishReferenceHeadings
```

Source facts: f0468, f0469.

