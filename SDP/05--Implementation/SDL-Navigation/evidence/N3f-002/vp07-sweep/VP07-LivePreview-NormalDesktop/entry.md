# VP07 — Features across the architecture

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

contributes-to, owns and explicit allocated-to per mode. Unspecified allocation is reported as a gap.

Selection: `{"viewpoint":"VP07","direction":"both","depth":2,"diagram":"VP07-LivePreview-NormalDesktop"}`.

## Feature: LivePreview — mode NormalDesktop

```mermaid
flowchart LR
    n_DocumentSession["DocumentSession (functionality)"]
    n_Documents["Documents (unit)"]
    n_EventLoop["EventLoop (unit)"]
    n_EventScheduling["EventScheduling (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_Interpreter["Interpreter (unit)"]
    n_LivePreview["LivePreview (feature)"]
    n_MarkdownInterpretation["MarkdownInterpretation (functionality)"]
    n_NativeShell["NativeShell (unit)"]
    n_Preview["Preview (unit)"]
    n_PreviewPipeline["PreviewPipeline (functionality)"]
    n_RenderLayout["RenderLayout (functionality)"]
    n_Renderer["Renderer (unit)"]
    n_TextEditing["TextEditing (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_LivePreview
    n_Documents -->|owns| n_DocumentSession
    n_Documents -->|owns| n_TextEditing
    n_EventLoop -->|owns| n_EventScheduling
    n_EventScheduling -->|allocated-to| n_XfmdDesktop
    n_EventScheduling -->|contributes-to| n_LivePreview
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_LivePreview
    n_Interpreter -->|owns| n_MarkdownInterpretation
    n_MarkdownInterpretation -->|allocated-to| n_XfmdDesktop
    n_MarkdownInterpretation -->|contributes-to| n_LivePreview
    n_NativeShell -->|owns| n_FoxPresentation
    n_Preview -->|owns| n_PreviewPipeline
    n_PreviewPipeline -->|allocated-to| n_XfmdDesktop
    n_PreviewPipeline -->|contributes-to| n_LivePreview
    n_RenderLayout -->|allocated-to| n_XfmdDesktop
    n_RenderLayout -->|contributes-to| n_LivePreview
    n_Renderer -->|owns| n_RenderLayout
    n_TextEditing -->|allocated-to| n_XfmdDesktop
    n_TextEditing -->|contributes-to| n_LivePreview
```

Source facts: f0207, f0209, f0232, f0233, f0276, f0280, f0281, f0351, f0354, f0484, f0531, f0532, f0544, f0687, f0717, f0718, f0837, f0838, f0846, f0936, f0938.


