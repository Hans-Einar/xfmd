# Use case: PublishPages

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: PublishPages

```mermaid
flowchart LR
    n_PagedPublication["PagedPublication (feature)"]
    n_PublishPages["PublishPages (usecase)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_PagedPublication -->|supports| n_PublishPages
    n_ReaderAuthor -->|pursues| n_PublishPages
```

Source facts: f0604, f0791.

