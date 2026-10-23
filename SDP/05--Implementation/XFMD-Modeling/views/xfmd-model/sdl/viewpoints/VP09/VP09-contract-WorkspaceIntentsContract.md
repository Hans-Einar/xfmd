# Contract structure: WorkspaceIntentsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: WorkspaceIntentsContract

```mermaid
flowchart LR
    n_ActivateExternalTarget["ActivateExternalTarget (message)"]
    n_ChooseInternalFile["ChooseInternalFile (message)"]
    n_FavoriteFileChosen["FavoriteFileChosen (message)"]
    n_FavoriteFolderChosen["FavoriteFolderChosen (message)"]
    n_WorkspaceIntentsContract["WorkspaceIntentsContract (contract)"]
    n_WorkspaceIntentsContract -->|permits| n_ActivateExternalTarget
    n_WorkspaceIntentsContract -->|permits| n_ChooseInternalFile
    n_WorkspaceIntentsContract -->|permits| n_FavoriteFileChosen
    n_WorkspaceIntentsContract -->|permits| n_FavoriteFolderChosen
```

Source facts: f1025, f1026, f1027, f1028.

