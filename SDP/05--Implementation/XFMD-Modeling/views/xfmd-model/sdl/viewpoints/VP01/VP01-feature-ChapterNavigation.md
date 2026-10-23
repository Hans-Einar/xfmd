# Functionality contributions to Feature: ChapterNavigation

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Functionality contributions to Feature: ChapterNavigation

```mermaid
flowchart LR
    n_ChapterNavigation["ChapterNavigation (feature)"]
    n_DocumentIndex["DocumentIndex (functionality)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_DocumentIndex -->|contributes-to| n_ChapterNavigation
    n_NavigationHistory -->|contributes-to| n_ChapterNavigation
    n_SourceAnchorMapping -->|contributes-to| n_ChapterNavigation
    n_WorkspaceControls -->|contributes-to| n_ChapterNavigation
```

Source facts: f0177, f0588, f0908, f1014.

