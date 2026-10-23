# Functionality contributions to Feature: DocumentReferences

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Functionality contributions to Feature: DocumentReferences

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_DocumentIndex["DocumentIndex (functionality)"]
    n_DocumentReferences["DocumentReferences (feature)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_ApplicationPreferences -->|contributes-to| n_DocumentReferences
    n_DocumentIndex -->|contributes-to| n_DocumentReferences
    n_NavigationHistory -->|contributes-to| n_DocumentReferences
    n_SourceAnchorMapping -->|contributes-to| n_DocumentReferences
    n_WorkspaceControls -->|contributes-to| n_DocumentReferences
```

Source facts: f0050, f0178, f0590, f0911, f1017.

