# VP07 — Features across the architecture

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

contributes-to, owns and explicit allocated-to per mode. Unspecified allocation is reported as a gap.

Selection: `{"viewpoint":"VP07","direction":"both","depth":2,"diagram":"VP07-MermaidDiagrams-NormalDesktop"}`.

## Feature: MermaidDiagrams — mode NormalDesktop

```mermaid
flowchart LR
    n_DiagramLayout["DiagramLayout (functionality)"]
    n_DiagramPreparation["DiagramPreparation (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_Interpreter["Interpreter (unit)"]
    n_MarkdownInterpretation["MarkdownInterpretation (functionality)"]
    n_MermaidDiagrams["MermaidDiagrams (feature)"]
    n_MermaidInterpretation["MermaidInterpretation (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_PageComposition["PageComposition (functionality)"]
    n_PdfExport["PdfExport (functionality)"]
    n_Preview["Preview (unit)"]
    n_PreviewPipeline["PreviewPipeline (functionality)"]
    n_PreviewTextSelection["PreviewTextSelection (functionality)"]
    n_Publication["Publication (unit)"]
    n_RenderLayout["RenderLayout (functionality)"]
    n_Renderer["Renderer (unit)"]
    n_Typography["Typography (unit)"]
    n_TypographyAndDrawing["TypographyAndDrawing (functionality)"]
    n_VisualResources["VisualResources (unit)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_DiagramLayout -->|allocated-to| n_XfmdDesktop
    n_DiagramLayout -->|contributes-to| n_MermaidDiagrams
    n_DiagramPreparation -->|allocated-to| n_XfmdDesktop
    n_DiagramPreparation -->|contributes-to| n_MermaidDiagrams
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_MermaidDiagrams
    n_Interpreter -->|owns| n_MarkdownInterpretation
    n_Interpreter -->|owns| n_MermaidInterpretation
    n_MarkdownInterpretation -->|allocated-to| n_XfmdDesktop
    n_MarkdownInterpretation -->|contributes-to| n_MermaidDiagrams
    n_MermaidInterpretation -->|allocated-to| n_XfmdDesktop
    n_MermaidInterpretation -->|contributes-to| n_MermaidDiagrams
    n_NativeShell -->|owns| n_FoxPresentation
    n_PageComposition -->|allocated-to| n_XfmdDesktop
    n_PageComposition -->|contributes-to| n_MermaidDiagrams
    n_PdfExport -->|allocated-to| n_XfmdDesktop
    n_PdfExport -->|contributes-to| n_MermaidDiagrams
    n_Preview -->|owns| n_PreviewPipeline
    n_Preview -->|owns| n_PreviewTextSelection
    n_PreviewPipeline -->|allocated-to| n_XfmdDesktop
    n_PreviewPipeline -->|contributes-to| n_MermaidDiagrams
    n_PreviewTextSelection -->|allocated-to| n_XfmdDesktop
    n_PreviewTextSelection -->|contributes-to| n_MermaidDiagrams
    n_Publication -->|owns| n_PdfExport
    n_RenderLayout -->|allocated-to| n_XfmdDesktop
    n_RenderLayout -->|contributes-to| n_MermaidDiagrams
    n_Renderer -->|owns| n_DiagramLayout
    n_Renderer -->|owns| n_PageComposition
    n_Renderer -->|owns| n_RenderLayout
    n_Typography -->|owns| n_TypographyAndDrawing
    n_TypographyAndDrawing -->|allocated-to| n_XfmdDesktop
    n_TypographyAndDrawing -->|contributes-to| n_MermaidDiagrams
    n_VisualResources -->|owns| n_DiagramPreparation
```

Source facts: f0129, f0130, f0135, f0136, f0351, f0356, f0484, f0485, f0531, f0534, f0539, f0540, f0544, f0600, f0602, f0625, f0626, f0687, f0688, f0717, f0720, f0740, f0742, f0747, f0837, f0840, f0844, f0845, f0846, f0944, f0946, f0948, f0983.


