# Feature: LivePreview — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Feature: LivePreview — mode NormalDesktop

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_DocumentSession["DocumentSession (functionality)"]
    n_EventScheduling["EventScheduling (functionality)"]
    n_FoxPresentation["FoxPresentation (functionality)"]
    n_Interpreter["Interpreter (unit)"]
    n_LivePreview["LivePreview (feature)"]
    n_MarkdownInterpretation["MarkdownInterpretation (functionality)"]
    n_PreviewPipeline["PreviewPipeline (functionality)"]
    n_RenderLayout["RenderLayout (functionality)"]
    n_Renderer["Renderer (unit)"]
    n_TextEditing["TextEditing (functionality)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_Application -->|owns| n_DocumentSession
    n_Application -->|owns| n_EventScheduling
    n_Application -->|owns| n_FoxPresentation
    n_Application -->|owns| n_PreviewPipeline
    n_Application -->|owns| n_TextEditing
    n_DocumentSession -->|allocated-to| n_XfmdDesktop
    n_DocumentSession -->|contributes-to| n_LivePreview
    n_EventScheduling -->|allocated-to| n_XfmdDesktop
    n_EventScheduling -->|contributes-to| n_LivePreview
    n_FoxPresentation -->|allocated-to| n_XfmdDesktop
    n_FoxPresentation -->|contributes-to| n_LivePreview
    n_Interpreter -->|owns| n_MarkdownInterpretation
    n_MarkdownInterpretation -->|allocated-to| n_XfmdDesktop
    n_MarkdownInterpretation -->|contributes-to| n_LivePreview
    n_PreviewPipeline -->|allocated-to| n_XfmdDesktop
    n_PreviewPipeline -->|contributes-to| n_LivePreview
    n_RenderLayout -->|allocated-to| n_XfmdDesktop
    n_RenderLayout -->|contributes-to| n_LivePreview
    n_Renderer -->|owns| n_RenderLayout
    n_TextEditing -->|allocated-to| n_XfmdDesktop
    n_TextEditing -->|contributes-to| n_LivePreview
```

Source facts: f0000, f0001, f0003, f0007, f0009, f0013, f0015, f0016, f0017, f0020, f0022, f0025, f0029, f0030, f0033, f0034, f0038, f0039, f0040, f0043, f0044.

