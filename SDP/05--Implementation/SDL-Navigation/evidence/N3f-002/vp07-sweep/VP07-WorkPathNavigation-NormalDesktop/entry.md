# VP07 — Features across the architecture

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

contributes-to, owns and explicit allocated-to per mode. Unspecified allocation is reported as a gap.

Selection: `{"viewpoint":"VP07","direction":"both","depth":2,"diagram":"VP07-WorkPathNavigation-NormalDesktop"}`.

## Feature: WorkPathNavigation — mode NormalDesktop

```mermaid
flowchart LR
    n_FilteredFileTree["FilteredFileTree (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_WorkPathHistory["WorkPathHistory (functionality)"]
    n_WorkPathNavigation["WorkPathNavigation (feature)"]
    n_Workspace["Workspace (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_FilteredFileTree -->|allocated-to| n_XfmdDesktop
    n_FilteredFileTree -->|contributes-to| n_WorkPathNavigation
    n_NativeShell -->|owns| n_WorkspaceControls
    n_WorkPathHistory -->|allocated-to| n_XfmdDesktop
    n_WorkPathHistory -->|contributes-to| n_WorkPathNavigation
    n_Workspace -->|owns| n_FilteredFileTree
    n_Workspace -->|owns| n_WorkPathHistory
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_WorkPathNavigation
```

Source facts: f0349, f0350, f0545, f0996, f0997, f0999, f1000, f1013, f1022.


