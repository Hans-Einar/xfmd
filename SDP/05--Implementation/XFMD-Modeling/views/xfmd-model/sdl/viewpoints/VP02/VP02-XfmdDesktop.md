# Logical decomposition: XfmdDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Logical decomposition: XfmdDesktop

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_Interpreter["Interpreter (unit)"]
    n_Renderer["Renderer (unit)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_XfmdDesktop -->|contains| n_Application
    n_XfmdDesktop -->|contains| n_Interpreter
    n_XfmdDesktop -->|contains| n_Renderer
```

Source facts: f1031, f1032, f1033.

