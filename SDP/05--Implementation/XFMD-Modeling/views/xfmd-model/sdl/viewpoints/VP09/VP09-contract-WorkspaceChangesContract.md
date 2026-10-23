# Contract structure: WorkspaceChangesContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: WorkspaceChangesContract

```mermaid
flowchart LR
    n_ActivateRootAndHistory["ActivateRootAndHistory (message)"]
    n_RequestTreeRefresh["RequestTreeRefresh (message)"]
    n_SetChosenParentRoot["SetChosenParentRoot (message)"]
    n_ValidateProposedRoot["ValidateProposedRoot (message)"]
    n_WorkspaceChangesContract["WorkspaceChangesContract (contract)"]
    n_WorkspaceChangesContract -->|permits| n_ActivateRootAndHistory
    n_WorkspaceChangesContract -->|permits| n_RequestTreeRefresh
    n_WorkspaceChangesContract -->|permits| n_SetChosenParentRoot
    n_WorkspaceChangesContract -->|permits| n_ValidateProposedRoot
```

Source facts: f1009, f1010, f1011, f1012.

