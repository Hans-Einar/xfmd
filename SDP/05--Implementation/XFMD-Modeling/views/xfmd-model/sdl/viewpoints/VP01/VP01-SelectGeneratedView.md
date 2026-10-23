# Use case: SelectGeneratedView

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: SelectGeneratedView

```mermaid
flowchart LR
    n_GeneratedDocumentNavigation["GeneratedDocumentNavigation (functionality)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_SelectGeneratedView["SelectGeneratedView (usecase)"]
    n_GeneratedDocumentNavigation -->|contributes-to| n_SelectGeneratedView
    n_ReaderAuthor -->|pursues| n_SelectGeneratedView
```

Source facts: f0385, f0795.

