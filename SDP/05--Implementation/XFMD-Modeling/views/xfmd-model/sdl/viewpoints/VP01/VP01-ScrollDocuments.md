# Use case: ScrollDocuments

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: ScrollDocuments

```mermaid
flowchart LR
    n_ConfigurableScrolling["ConfigurableScrolling (feature)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_ScrollDocuments["ScrollDocuments (usecase)"]
    n_SynchronizedScrolling["SynchronizedScrolling (feature)"]
    n_ConfigurableScrolling -->|supports| n_ScrollDocuments
    n_ReaderAuthor -->|pursues| n_ScrollDocuments
    n_SynchronizedScrolling -->|supports| n_ScrollDocuments
```

Source facts: f0107, f0794, f0928.

