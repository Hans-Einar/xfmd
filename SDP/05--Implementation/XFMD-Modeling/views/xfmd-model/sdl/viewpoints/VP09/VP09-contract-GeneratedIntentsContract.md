# Contract structure: GeneratedIntentsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GeneratedIntentsContract

```mermaid
flowchart LR
    n_GeneratedIntentsContract["GeneratedIntentsContract (contract)"]
    n_SelectInvalidView["SelectInvalidView (message)"]
    n_SelectRegisteredView["SelectRegisteredView (message)"]
    n_GeneratedIntentsContract -->|permits| n_SelectInvalidView
    n_GeneratedIntentsContract -->|permits| n_SelectRegisteredView
```

Source facts: f0391, f0392.

