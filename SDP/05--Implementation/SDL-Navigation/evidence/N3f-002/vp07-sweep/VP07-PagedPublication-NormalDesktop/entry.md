# VP07 — Features across the architecture

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

contributes-to, owns and explicit allocated-to per mode. Unspecified allocation is reported as a gap.

Selection: `{"viewpoint":"VP07","direction":"both","depth":2,"diagram":"VP07-PagedPublication-NormalDesktop"}`.

## Feature: PagedPublication — mode NormalDesktop

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_Documents["Documents (unit)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_Interpreter["Interpreter (unit)"]
    n_MarkdownInterpretation["MarkdownInterpretation (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_PageComposition["PageComposition (functionality)"]
    n_PagedPublication["PagedPublication (feature)"]
    n_PdfExport["PdfExport (functionality)"]
    n_Preferences["Preferences (unit)"]
    n_Preview["Preview (unit)"]
    n_PreviewPipeline["PreviewPipeline (functionality)"]
    n_Publication["Publication (unit)"]
    n_RenderLayout["RenderLayout (functionality)"]
    n_Renderer["Renderer (unit)"]
    n_SourceAnchorMapping["SourceAnchorMapping (functionality)"]
    n_Typography["Typography (unit)"]
    n_TypographyAndDrawing["TypographyAndDrawing (functionality)"]
    n_Viewport["Viewport (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_ApplicationPreferences -->|allocated-to| n_XfmdDesktop
    n_ApplicationPreferences -->|contributes-to| n_PagedPublication
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_PagedPublication
    n_Documents -->|owns| n_DocumentSession
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_PagedPublication
    n_Interpreter -->|owns| n_MarkdownInterpretation
    n_MarkdownInterpretation -->|allocated-to| n_XfmdDesktop
    n_MarkdownInterpretation -->|contributes-to| n_PagedPublication
    n_NativeShell -->|owns| n_FoxPresentation
    n_NativeShell -->|owns| n_WorkspaceControls
    n_PageComposition -->|allocated-to| n_XfmdDesktop
    n_PageComposition -->|contributes-to| n_PagedPublication
    n_PdfExport -->|allocated-to| n_XfmdDesktop
    n_PdfExport -->|contributes-to| n_PagedPublication
    n_Preferences -->|owns| n_ApplicationPreferences
    n_Preview -->|owns| n_PreviewPipeline
    n_PreviewPipeline -->|allocated-to| n_XfmdDesktop
    n_PreviewPipeline -->|contributes-to| n_PagedPublication
    n_Publication -->|owns| n_PdfExport
    n_RenderLayout -->|allocated-to| n_XfmdDesktop
    n_RenderLayout -->|contributes-to| n_PagedPublication
    n_Renderer -->|owns| n_PageComposition
    n_Renderer -->|owns| n_RenderLayout
    n_SourceAnchorMapping -->|allocated-to| n_XfmdDesktop
    n_SourceAnchorMapping -->|contributes-to| n_PagedPublication
    n_Typography -->|owns| n_TypographyAndDrawing
    n_TypographyAndDrawing -->|allocated-to| n_XfmdDesktop
    n_TypographyAndDrawing -->|contributes-to| n_PagedPublication
    n_Viewport -->|owns| n_SourceAnchorMapping
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_PagedPublication
```

Source facts: f0046, f0051, f0207, f0211, f0232, f0351, f0357, f0484, f0531, f0535, f0544, f0545, f0600, f0603, f0625, f0627, f0652, f0687, f0717, f0721, f0747, f0837, f0841, f0845, f0846, f0907, f0912, f0944, f0946, f0949, f0966, f1013, f1019.


