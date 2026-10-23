# Use case: EditDocuments

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Use case: EditDocuments

```mermaid
flowchart LR
    n_EditDocuments["EditDocuments (usecase)"]
    n_LivePreview["LivePreview (feature)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_LivePreview -->|supports| n_EditDocuments
    n_ReaderAuthor -->|pursues| n_EditDocuments
```

Source facts: f0026, f0035.

