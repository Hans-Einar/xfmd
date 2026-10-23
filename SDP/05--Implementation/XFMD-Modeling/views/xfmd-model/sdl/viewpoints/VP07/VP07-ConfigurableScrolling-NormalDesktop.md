# Feature: ConfigurableScrolling — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Feature: ConfigurableScrolling — mode NormalDesktop

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_ConfigurableScrolling["ConfigurableScrolling (feature)"]
    n_Documents["Documents (unit)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_Preferences["Preferences (unit)"]
    n_ScrollMotion["ScrollMotion (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_TextEditing["TextEditing (functionality)"]
    n_Viewport["Viewport (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_ApplicationPreferences -->|allocated-to| n_XfmdDesktop
    n_ApplicationPreferences -->|contributes-to| n_ConfigurableScrolling
    n_Documents -->|owns| n_TextEditing
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_ConfigurableScrolling
    n_NativeShell -->|owns| n_FoxPresentation
    n_NativeShell -->|owns| n_WorkspaceControls
    n_Preferences -->|owns| n_ApplicationPreferences
    n_ScrollMotion -->|allocated-to| n_XfmdDesktop
    n_ScrollMotion -->|contributes-to| n_ConfigurableScrolling
    n_SourceAnchorMapping -->|allocated-to| n_XfmdDesktop
    n_SourceAnchorMapping -->|contributes-to| n_ConfigurableScrolling
    n_TextEditing -->|allocated-to| n_XfmdDesktop
    n_TextEditing -->|contributes-to| n_ConfigurableScrolling
    n_Viewport -->|owns| n_ScrollMotion
    n_Viewport -->|owns| n_SourceAnchorMapping
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_ConfigurableScrolling
```

Source facts: f0046, f0047, f0233, f0351, f0352, f0544, f0545, f0652, f0889, f0890, f0907, f0909, f0936, f0937, f0965, f0966, f1013, f1015.

