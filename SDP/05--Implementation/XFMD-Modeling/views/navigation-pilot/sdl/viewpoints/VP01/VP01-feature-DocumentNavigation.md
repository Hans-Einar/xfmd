# Functionality contributions to Feature: DocumentNavigation

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Functionality contributions to Feature: DocumentNavigation

```mermaid
flowchart LR
    n_DocumentNavigation["DocumentNavigation (feature)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_LocalFileStorage["LocalFileStorage (functionality)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_DocumentSession -->|contributes-to| n_DocumentNavigation
    n_FoxPresentation -->|contributes-to| n_DocumentNavigation
    n_LocalFileStorage -->|contributes-to| n_DocumentNavigation
    n_NavigationHistory -->|contributes-to| n_DocumentNavigation
    n_SourceAnchorMapping -->|contributes-to| n_DocumentNavigation
    n_WorkspaceControls -->|contributes-to| n_DocumentNavigation
```

Source facts: f0014, f0021, f0028, f0032, f0042, f0049.

