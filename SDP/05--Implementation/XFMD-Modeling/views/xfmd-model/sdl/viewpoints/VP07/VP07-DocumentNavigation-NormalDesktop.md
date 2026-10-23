# Feature: DocumentNavigation — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Feature: DocumentNavigation — mode NormalDesktop

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_DocumentNavigation["DocumentNavigation (feature)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_DocumentStorage["DocumentStorage (unit)"]
    n_Documents["Documents (unit)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_LocalFileStorage["LocalFileStorage (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_Navigation["Navigation (unit)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_Preferences["Preferences (unit)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_Viewport["Viewport (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_ApplicationPreferences -->|allocated-to| n_XfmdDesktop
    n_ApplicationPreferences -->|contributes-to| n_DocumentNavigation
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_DocumentNavigation
    n_DocumentStorage -->|owns| n_LocalFileStorage
    n_Documents -->|owns| n_DocumentSession
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_DocumentNavigation
    n_LocalFileStorage -->|allocated-to| n_XfmdDesktop
    n_LocalFileStorage -->|contributes-to| n_DocumentNavigation
    n_NativeShell -->|owns| n_FoxPresentation
    n_NativeShell -->|owns| n_WorkspaceControls
    n_Navigation -->|owns| n_NavigationHistory
    n_NavigationHistory -->|allocated-to| n_XfmdDesktop
    n_NavigationHistory -->|contributes-to| n_DocumentNavigation
    n_Preferences -->|owns| n_ApplicationPreferences
    n_SourceAnchorMapping -->|allocated-to| n_XfmdDesktop
    n_SourceAnchorMapping -->|contributes-to| n_DocumentNavigation
    n_Viewport -->|owns| n_SourceAnchorMapping
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_DocumentNavigation
```

Source facts: f0046, f0049, f0207, f0208, f0222, f0232, f0351, f0353, f0523, f0524, f0544, f0545, f0576, f0587, f0589, f0652, f0907, f0910, f0966, f1013, f1016.

