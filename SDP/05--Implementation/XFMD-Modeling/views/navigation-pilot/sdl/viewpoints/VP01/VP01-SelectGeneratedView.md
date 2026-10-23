# Use case: SelectGeneratedView

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Use case: SelectGeneratedView

```mermaid
flowchart LR
    n_GeneratedDocumentNavigation["GeneratedDocumentNavigation (functionality)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_SelectGeneratedView["SelectGeneratedView (usecase)"]
    n_GeneratedDocumentNavigation -->|contributes-to| n_SelectGeneratedView
    n_ReaderAuthor -->|pursues| n_SelectGeneratedView
```

Source facts: f0024, f0037.

