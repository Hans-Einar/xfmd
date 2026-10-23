# Use case: BrowseWorkspace

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: BrowseWorkspace

```mermaid
flowchart LR
    n_BrowseWorkspace["BrowseWorkspace (usecase)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_WorkPathNavigation["WorkPathNavigation (feature)"]
    n_ReaderAuthor -->|pursues| n_BrowseWorkspace
    n_WorkPathNavigation -->|supports| n_BrowseWorkspace
```

Source facts: f0787, f0998.

