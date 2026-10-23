# Contract structure: ViewportChangesContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ViewportChangesContract

```mermaid
flowchart LR
    n_CheckCurrentMapping["CheckCurrentMapping (message)"]
    n_MapSourceAnchor["MapSourceAnchor (message)"]
    n_RetainDocumentAndAnchor["RetainDocumentAndAnchor (message)"]
    n_SetEditorAnchor["SetEditorAnchor (message)"]
    n_UserPreviewScroll["UserPreviewScroll (message)"]
    n_ViewportChangesContract["ViewportChangesContract (contract)"]
    n_ViewportChangesContract -->|permits| n_CheckCurrentMapping
    n_ViewportChangesContract -->|permits| n_MapSourceAnchor
    n_ViewportChangesContract -->|permits| n_RetainDocumentAndAnchor
    n_ViewportChangesContract -->|permits| n_SetEditorAnchor
    n_ViewportChangesContract -->|permits| n_UserPreviewScroll
```

Source facts: f0976, f0977, f0978, f0979, f0980.

