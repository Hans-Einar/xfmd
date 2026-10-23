# Feature: DocumentNavigation — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Feature: DocumentNavigation — mode NormalDesktop

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_DocumentNavigation["DocumentNavigation (feature)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_LocalFileStorage["LocalFileStorage (functionality)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_Application -->|owns| n_DocumentSession
    n_Application -->|owns| n_FoxPresentation
    n_Application -->|owns| n_LocalFileStorage
    n_Application -->|owns| n_NavigationHistory
    n_Application -->|owns| n_SourceAnchorMapping
    n_Application -->|owns| n_WorkspaceControls
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_DocumentNavigation
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_DocumentNavigation
    n_LocalFileStorage -->|allocated-to| n_XfmdDesktop
    n_LocalFileStorage -->|contributes-to| n_DocumentNavigation
    n_NavigationHistory -->|allocated-to| n_XfmdDesktop
    n_NavigationHistory -->|contributes-to| n_DocumentNavigation
    n_SourceAnchorMapping -->|allocated-to| n_XfmdDesktop
    n_SourceAnchorMapping -->|contributes-to| n_DocumentNavigation
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_DocumentNavigation
```

Source facts: f0000, f0003, f0005, f0006, f0008, f0011, f0013, f0014, f0020, f0021, f0027, f0028, f0031, f0032, f0041, f0042, f0048, f0049.

