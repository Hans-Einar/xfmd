# Functionality contributions to Feature: DocumentNavigation

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Functionality contributions to Feature: DocumentNavigation

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_DocumentNavigation["DocumentNavigation (feature)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_LocalFileStorage["LocalFileStorage (functionality)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_ApplicationPreferences -->|contributes-to| n_DocumentNavigation
    n_DocumentSession -->|contributes-to| n_DocumentNavigation
    n_FoxPresentation -->|contributes-to| n_DocumentNavigation
    n_LocalFileStorage -->|contributes-to| n_DocumentNavigation
    n_NavigationHistory -->|contributes-to| n_DocumentNavigation
    n_SourceAnchorMapping -->|contributes-to| n_DocumentNavigation
    n_WorkspaceControls -->|contributes-to| n_DocumentNavigation
```

Source facts: f0049, f0208, f0353, f0524, f0589, f0910, f1016.

