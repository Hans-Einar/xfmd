# Feature: WorkPathNavigation — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Feature: WorkPathNavigation — mode NormalDesktop

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_FilteredFileTree["FilteredFileTree (functionality)"]
    n_WorkPathHistory["WorkPathHistory (functionality)"]
    n_WorkPathNavigation["WorkPathNavigation (feature)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_Application -->|owns| n_FilteredFileTree
    n_Application -->|owns| n_WorkPathHistory
    n_Application -->|owns| n_WorkspaceControls
    n_FilteredFileTree -->|allocated-to| n_XfmdDesktop
    n_FilteredFileTree -->|contributes-to| n_WorkPathNavigation
    n_WorkPathHistory -->|allocated-to| n_XfmdDesktop
    n_WorkPathHistory -->|contributes-to| n_WorkPathNavigation
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_WorkPathNavigation
```

Source facts: f0002, f0010, f0011, f0018, f0019, f0045, f0046, f0048, f0050.

