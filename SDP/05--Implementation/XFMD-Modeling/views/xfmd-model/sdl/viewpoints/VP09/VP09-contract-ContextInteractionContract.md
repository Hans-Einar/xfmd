# Contract structure: ContextInteractionContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ContextInteractionContract

```mermaid
flowchart LR
    n_CaptureContextTarget["CaptureContextTarget (message)"]
    n_ContextInteractionContract["ContextInteractionContract (contract)"]
    n_DismissContextMenu["DismissContextMenu (message)"]
    n_ReleaseKeyboardGrab["ReleaseKeyboardGrab (message)"]
    n_ContextInteractionContract -->|permits| n_CaptureContextTarget
    n_ContextInteractionContract -->|permits| n_DismissContextMenu
    n_ContextInteractionContract -->|permits| n_ReleaseKeyboardGrab
```

Source facts: f0110, f0111, f0112.

