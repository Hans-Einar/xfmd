# Use case: ReadDocuments

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: ReadDocuments

```mermaid
flowchart LR
    n_MarkdownPresentation["MarkdownPresentation (feature)"]
    n_MermaidDiagrams["MermaidDiagrams (feature)"]
    n_PagedPublication["PagedPublication (feature)"]
    n_ReadDocuments["ReadDocuments (usecase)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_MarkdownPresentation -->|supports| n_ReadDocuments
    n_MermaidDiagrams -->|supports| n_ReadDocuments
    n_PagedPublication -->|supports| n_ReadDocuments
    n_ReaderAuthor -->|pursues| n_ReadDocuments
```

Source facts: f0536, f0538, f0605, f0793.

