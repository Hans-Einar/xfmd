# Use case: NavigateDocuments

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: NavigateDocuments

```mermaid
flowchart LR
    n_ChapterNavigation["ChapterNavigation (feature)"]
    n_DocumentNavigation["DocumentNavigation (feature)"]
    n_DocumentReferences["DocumentReferences (feature)"]
    n_NavigateDocuments["NavigateDocuments (usecase)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_ChapterNavigation -->|supports| n_NavigateDocuments
    n_DocumentNavigation -->|supports| n_NavigateDocuments
    n_DocumentReferences -->|supports| n_NavigateDocuments
    n_ReaderAuthor -->|pursues| n_NavigateDocuments
```

Source facts: f0075, f0190, f0201, f0790.

