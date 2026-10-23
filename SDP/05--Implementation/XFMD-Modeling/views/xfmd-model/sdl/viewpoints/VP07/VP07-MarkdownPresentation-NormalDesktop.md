# Feature: MarkdownPresentation — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Feature: MarkdownPresentation — mode NormalDesktop

```mermaid
flowchart LR
    n_DocumentSession["DocumentSession (functionality)"]
    n_DocumentStorage["DocumentStorage (unit)"]
    n_Documents["Documents (unit)"]
    n_EmbeddedVisuals["EmbeddedVisuals (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_Interpreter["Interpreter (unit)"]
    n_LocalFileStorage["LocalFileStorage (functionality)"]
    n_MarkdownInterpretation["MarkdownInterpretation (functionality)"]
    n_MarkdownPresentation["MarkdownPresentation (feature)"]
    n_NativeShell["NativeShell (unit)"]
    n_PageComposition["PageComposition (functionality)"]
    n_Preview["Preview (unit)"]
    n_PreviewPipeline["PreviewPipeline (functionality)"]
    n_PreviewTextSelection["PreviewTextSelection (functionality)"]
    n_RenderLayout["RenderLayout (functionality)"]
    n_Renderer["Renderer (unit)"]
    n_Typography["Typography (unit)"]
    n_TypographyAndDrawing["TypographyAndDrawing (functionality)"]
    n_VisualResources["VisualResources (unit)"]
    n_WorkspaceControls["WorkspaceControls (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_MarkdownPresentation
    n_DocumentStorage -->|owns| n_LocalFileStorage
    n_Documents -->|owns| n_DocumentSession
    n_EmbeddedVisuals -->|allocated-to| n_XfmdDesktop
    n_EmbeddedVisuals -->|contributes-to| n_MarkdownPresentation
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_MarkdownPresentation
    n_Interpreter -->|owns| n_MarkdownInterpretation
    n_LocalFileStorage -->|allocated-to| n_XfmdDesktop
    n_LocalFileStorage -->|contributes-to| n_MarkdownPresentation
    n_MarkdownInterpretation -->|allocated-to| n_XfmdDesktop
    n_MarkdownInterpretation -->|contributes-to| n_MarkdownPresentation
    n_NativeShell -->|owns| n_FoxPresentation
    n_NativeShell -->|owns| n_WorkspaceControls
    n_PageComposition -->|allocated-to| n_XfmdDesktop
    n_PageComposition -->|contributes-to| n_MarkdownPresentation
    n_Preview -->|owns| n_PreviewPipeline
    n_Preview -->|owns| n_PreviewTextSelection
    n_PreviewPipeline -->|allocated-to| n_XfmdDesktop
    n_PreviewPipeline -->|contributes-to| n_MarkdownPresentation
    n_PreviewTextSelection -->|allocated-to| n_XfmdDesktop
    n_PreviewTextSelection -->|contributes-to| n_MarkdownPresentation
    n_RenderLayout -->|allocated-to| n_XfmdDesktop
    n_RenderLayout -->|contributes-to| n_MarkdownPresentation
    n_Renderer -->|owns| n_PageComposition
    n_Renderer -->|owns| n_RenderLayout
    n_Typography -->|owns| n_TypographyAndDrawing
    n_TypographyAndDrawing -->|allocated-to| n_XfmdDesktop
    n_TypographyAndDrawing -->|contributes-to| n_MarkdownPresentation
    n_VisualResources -->|owns| n_EmbeddedVisuals
    n_WorkspaceControls -->|allocated-to| n_XfmdDesktop
    n_WorkspaceControls -->|contributes-to| n_MarkdownPresentation
```

Source facts: f0207, f0210, f0222, f0232, f0274, f0275, f0351, f0355, f0484, f0523, f0525, f0531, f0533, f0544, f0545, f0600, f0601, f0687, f0688, f0717, f0719, f0740, f0741, f0837, f0839, f0845, f0846, f0944, f0946, f0947, f0984, f1013, f1018.

