# Use case: EditDocuments

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: EditDocuments

```mermaid
flowchart LR
    n_EditDocuments["EditDocuments (usecase)"]
    n_LivePreview["LivePreview (feature)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_LivePreview -->|supports| n_EditDocuments
    n_ReaderAuthor -->|pursues| n_EditDocuments
```

Source facts: f0522, f0789.

