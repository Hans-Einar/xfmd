# VP07 — Features across the architecture

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

contributes-to, owns and explicit allocated-to per mode. Unspecified allocation is reported as a gap.

Selection: `{"viewpoint":"VP07","direction":"both","depth":2,"diagram":"VP07-ChapterNavigation-NormalDesktop"}`.

## Feature: ChapterNavigation — mode NormalDesktop

```mermaid
flowchart LR
    n_ChapterNavigation["ChapterNavigation (feature)"]
    n_DocumentIndex["DocumentIndex (functionality)"]
    n_DocumentIndexService["DocumentIndexService (unit)"]
    n_NativeShell["NativeShell (unit)"]
    n_Navigation["Navigation (unit)"]
    n_NavigationHistory["NavigationHistory (functionality)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_Viewport["Viewport (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_DocumentIndex -->|allocated-to| n_XfmdDesktop
    n_DocumentIndex -->|contributes-to| n_ChapterNavigation
    n_DocumentIndexService -->|owns| n_DocumentIndex
    n_NativeShell -->|owns| n_WorkspaceControls
    n_Navigation -->|owns| n_NavigationHistory
    n_NavigationHistory -->|allocated-to| n_XfmdDesktop
    n_NavigationHistory -->|contributes-to| n_ChapterNavigation
    n_SourceAnchorMapping -->|allocated-to| n_XfmdDesktop
    n_SourceAnchorMapping -->|contributes-to| n_ChapterNavigation
    n_Viewport -->|owns| n_SourceAnchorMapping
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_ChapterNavigation
```

Source facts: f0176, f0177, f0181, f0545, f0576, f0587, f0588, f0907, f0908, f0966, f1013, f1014.


